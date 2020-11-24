/***************************************************************************//**
* \file D_UART_SPI_UART_PVT.h
* \version 4.0
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component in SPI and UART modes.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_SPI_UART_PVT_D_UART_H)
#define CY_SCB_SPI_UART_PVT_D_UART_H

#include "D_UART_SPI_UART.h"


/***************************************
*     Internal Global Vars
***************************************/

#if (D_UART_INTERNAL_RX_SW_BUFFER_CONST)
    extern volatile uint32  D_UART_rxBufferHead;
    extern volatile uint32  D_UART_rxBufferTail;
    
    /**
    * \addtogroup group_globals
    * @{
    */
    
    /** Sets when internal software receive buffer overflow
     *  was occurred.
    */  
    extern volatile uint8   D_UART_rxBufferOverflow;
    /** @} globals */
#endif /* (D_UART_INTERNAL_RX_SW_BUFFER_CONST) */

#if (D_UART_INTERNAL_TX_SW_BUFFER_CONST)
    extern volatile uint32  D_UART_txBufferHead;
    extern volatile uint32  D_UART_txBufferTail;
#endif /* (D_UART_INTERNAL_TX_SW_BUFFER_CONST) */

#if (D_UART_INTERNAL_RX_SW_BUFFER)
    extern volatile uint8 D_UART_rxBufferInternal[D_UART_INTERNAL_RX_BUFFER_SIZE];
#endif /* (D_UART_INTERNAL_RX_SW_BUFFER) */

#if (D_UART_INTERNAL_TX_SW_BUFFER)
    extern volatile uint8 D_UART_txBufferInternal[D_UART_TX_BUFFER_SIZE];
#endif /* (D_UART_INTERNAL_TX_SW_BUFFER) */


/***************************************
*     Private Function Prototypes
***************************************/

void D_UART_SpiPostEnable(void);
void D_UART_SpiStop(void);

#if (D_UART_SCB_MODE_SPI_CONST_CFG)
    void D_UART_SpiInit(void);
#endif /* (D_UART_SCB_MODE_SPI_CONST_CFG) */

#if (D_UART_SPI_WAKE_ENABLE_CONST)
    void D_UART_SpiSaveConfig(void);
    void D_UART_SpiRestoreConfig(void);
#endif /* (D_UART_SPI_WAKE_ENABLE_CONST) */

void D_UART_UartPostEnable(void);
void D_UART_UartStop(void);

#if (D_UART_SCB_MODE_UART_CONST_CFG)
    void D_UART_UartInit(void);
#endif /* (D_UART_SCB_MODE_UART_CONST_CFG) */

#if (D_UART_UART_WAKE_ENABLE_CONST)
    void D_UART_UartSaveConfig(void);
    void D_UART_UartRestoreConfig(void);
#endif /* (D_UART_UART_WAKE_ENABLE_CONST) */


/***************************************
*         UART API Constants
***************************************/

/* UART RX and TX position to be used in D_UART_SetPins() */
#define D_UART_UART_RX_PIN_ENABLE    (D_UART_UART_RX)
#define D_UART_UART_TX_PIN_ENABLE    (D_UART_UART_TX)

/* UART RTS and CTS position to be used in  D_UART_SetPins() */
#define D_UART_UART_RTS_PIN_ENABLE    (0x10u)
#define D_UART_UART_CTS_PIN_ENABLE    (0x20u)


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Interrupt processing */
#define D_UART_SpiUartEnableIntRx(intSourceMask)  D_UART_SetRxInterruptMode(intSourceMask)
#define D_UART_SpiUartEnableIntTx(intSourceMask)  D_UART_SetTxInterruptMode(intSourceMask)
uint32  D_UART_SpiUartDisableIntRx(void);
uint32  D_UART_SpiUartDisableIntTx(void);


#endif /* (CY_SCB_SPI_UART_PVT_D_UART_H) */


/* [] END OF FILE */
