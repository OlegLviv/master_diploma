
#include <project.h>
#include <stdio.h>

/*******************************************************************************
*            Global variables
*******************************************************************************/
uint8 pressCount  = 0; // Keeps track of button presses
uint8 printStatus = 0; // Flag for printing

/*******************************************************************************
* Function Name: _write
********************************************************************************
*
* Summary: 
*   This version of _write sends text via UART. printf is redirected to 
*   this function when GCC compiler is used to print data to terminal using UART. 
*   Note that depending on your compiler you may need to rewrite a different
*   function that is linked to printf.
* 
* Parameters:
*   file: This variable is not used.
*   *ptr: Pointer to the data which will be transfered through UART.
*   len: Length of the data to be transfered through UART.
* 
* Returns: 
*   returns the number of characters transferred using UART.
*
*******************************************************************************/
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
            UART_UartPutChar(*(ptr++)); 
        }          
        return(len); 
    } 
#endif /* (__GNUC__) */

/***************************************************************************//**
* Function Name: ISR_SWITCH
********************************************************************************
*
* Summary:
* This function is executed when the GPIO interrupt is triggered by the switch
* This ISR
*   1. Clears the interrupt source.
*   2. Increments the count and sets printStatus flag.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
CY_ISR(ISR_SWITCH)
{
	/* Clears interrupt for button */
    switch_pin_ClearInterrupt();
	
	/* Clears pending interrupts */
    InputInterrupt_ClearPending();
	
	/* Updates button count and printing state */
    pressCount++;
    printStatus = 1;
    
}

/*******************************************************************************
* Function Name: main
********************************************************************************
*
*  The main function performs the following actions:
*   1. Input/switch interrupt is set up and enabled.
*   2. Initializes the UART components.
*   3. Global interrupt enabled.
*   4. UART sends the header to the terminal.
*   5. printf is tested once.
*   6. Each time interrupt occurs/switch is pressed the count and flags are set.
*   7. When printStatus is set high, printf is used to indicate number pressed.
*
*******************************************************************************/
int main(void)
{
    int32 integerNum = -212121; 
    float floatNum = 37.37f;

    /* Input interrupt and sets the interrupt service routine*/
    InputInterrupt_StartEx(ISR_SWITCH);
    
    /* Ensure that interrupts are clear */
    InputInterrupt_ClearPending();
    
    /* Start UART operation */
    UART_Start();
    
    /* Enable global interrupts */
    CyGlobalIntEnable; 
    
    /* Transmit header to the terminal */
    UART_UartPutString("\r\n**********************************************************************************\r\n");
    UART_UartPutString("This example demonstrates UART operation with printf \r\n");
    UART_UartPutString("If you see this text the terminal connection is configured correctly.\r\n");
    UART_UartPutString("Push button to see printf function used to print on terminal.\r\n");
    UART_UartPutString("\r\n");
    
    printf("Testing printf function. long: %ld , float: %f \r\n", integerNum, floatNum);
    
    for(;;)
    {
        CyDelay(1000);
        printf("Number of times button has been pressed = %d\r\n", pressCount);
        
    }
}

/* [] END OF FILE */
