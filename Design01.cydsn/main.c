#include <project.h>
#include <stdio.h>
/* Order of Digital Filter */
#define DF_ORDER (2)

/* RTC config */
/* Time: 02:59:50 */
#define TIME_HOUR           (0x02u)
#define TIME_MIN            (0x59u)
#define TIME_SEC            (0x50u)
#define TIME_HR_MIN_SEC     ((uint32)(TIME_HOUR << RTC_HOURS_OFFSET) | \
                            (uint32)(TIME_MIN << RTC_MINUTES_OFFSET)    | \
                             TIME_SEC)
/* Date: 03/22/2014 */
#define DATE_MONTH          (RTC_MARCH)
#define DATE_DAY            (0x22u)
#define DATE_YEAR           (0x2014u)
#define DATE_MONTH_DAY_YEAR ((uint32)(DATE_MONTH << RTC_MONTH_OFFSET)   | \
                            (uint32)(DATE_DAY << RTC_DAY_OFFSET)        | \
                             DATE_YEAR)

#define SYSTICK_EACH_10_HZ  (10u)
#define SYSTICK_RELOAD      (CYDEV_BCLK__SYSCLK__HZ / SYSTICK_EACH_10_HZ)

/* Digital Filter Denumerator coefficients */
float DigitalFilter_ACoefs[DF_ORDER + 1] = {1.0000, -1.1429, 0.4128};
/* Digital Filter Numerator coefficients */
float DigitalFilter_BCoefs[DF_ORDER + 1] = {0.0674, 0.1349, 0.0674};
/* Digital Filter internal buffer */
float DigitalFilter_Buffer[DF_ORDER + 1];

int16 result = 0;
float DigitalFilter_ProcessOneSample(float NewSample);
void print_time();
uint32 time;
uint32 cnt = 0;

#if defined (__GNUC__) 
    
    /* Add an explicit reference to the floating point printf library to allow 
       use of floating point conversion specifier. */     
    asm (".global _printf_float"); 
 
    /* For GCC compiler revise _write() function for printf functionality */     
	int _write(int file, char *ptr, int len) 
    {       
        /* Warning suppression for unused function parameter */
		file = file;
		
		int i; 
        /* Character is sent via UART */
        for (i = 0; i < len; i++) 
        { 
            D_UART_UartPutChar(*(ptr++)); 
        }          
        return(len); 
    } 
#endif /* (__GNUC__) */

CY_ISR( MyADC )
{
    cnt++;
    uint32 intr_status;
    /* Read interrupt status register */
    intr_status = ADC_SAR_INTR_REG;

    /* `#START MAIN_ADC_ISR` */
    result = ADC_GetResult16(0);
    //printf("adc measure = %d\r\n", result);
    float accumulator = DigitalFilter_ProcessOneSample(result);
    //printf("accum measure = %f\r\n", accumulator);
    IDAC_SetValue(result);
    /* Clear handled interrupt */
    ADC_SAR_INTR_REG = intr_status;
}


int main(void)
{

    D_UART_Start();
    
    isr_adc_StartEx(MyADC);
    
    ADC_Timer_Start();
    ADC_Enable();
    ADC_Init();
    ADC_Start(); /* Initialize ADC */
    ADC_IRQ_Enable(); /* Enable ADC interrupts */
    ADC_StartConvert(); /* Start ADC conversions */
    IDAC_Start();
    
    printf("Debug info start...\r\n");
    
    /* Starts SysTick component */
    //CySysTickStart();

    /* Starts RTC component */
    RTC_Start();

    /* Set Date and Time */
    RTC_SetDateAndTime(TIME_HR_MIN_SEC,DATE_MONTH_DAY_YEAR);

    /* Set RTC time update period */
    //RTC_SetPeriod(1u, SYSTICK_EACH_10_HZ);

    /* Enable global interrupts */
    CyGlobalIntEnable;
    print_time();
    for(;;){
        if (cnt==1000)
        {
            print_time();
        }
    }
}

/***************************************************************
* \fn DigitalFilter_ProcessOneSample
*  
* This function to process one sample in digital filter.
* Note: One sample processing at DF_ORDER = 7 and CPU frequency 
*       24 MHz takes about 160 us 
*
* \param NewSample
*  Sample value to be processed
*
* \return 
*  Digital filter output sample
****************************************************************/
float DigitalFilter_ProcessOneSample(float NewSample)
{
    uint32 idx; 
    float  Accumulator;
    /* Shift the old values in the internal buffer DF_Buffer */
    for (idx = DF_ORDER; idx > 0; idx--)
    {
        DigitalFilter_Buffer[idx] = DigitalFilter_Buffer[idx - 1];
    }    
    /* Sum in Denumerator */
    Accumulator = NewSample;
    for (idx = 1; idx <= DF_ORDER; idx++)
    {
        Accumulator = Accumulator - DigitalFilter_ACoefs[idx] * DigitalFilter_Buffer[idx];
    }
    DigitalFilter_Buffer[0] = Accumulator;
    /* Sum in Numerator */
    Accumulator = 0;
    for (idx = 0; idx <= DF_ORDER; idx++)
    {
        Accumulator = Accumulator + DigitalFilter_BCoefs[idx] * DigitalFilter_Buffer[idx];
    }
    return (Accumulator);
}

void print_time(){
    RTC_Update();
    time = RTC_GetTime();
    printf("%02lu:%02lu\r\n",RTC_GetMinutes(time), RTC_GetSecond(time));
}
    