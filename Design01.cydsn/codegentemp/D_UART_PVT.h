/***************************************************************************//**
* \file .h
* \version 4.0
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component.
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

#if !defined(CY_SCB_PVT_D_UART_H)
#define CY_SCB_PVT_D_UART_H

#include "D_UART.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define D_UART_SetI2CExtClkInterruptMode(interruptMask) D_UART_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define D_UART_ClearI2CExtClkInterruptSource(interruptMask) D_UART_CLEAR_INTR_I2C_EC(interruptMask)
#define D_UART_GetI2CExtClkInterruptSource()                (D_UART_INTR_I2C_EC_REG)
#define D_UART_GetI2CExtClkInterruptMode()                  (D_UART_INTR_I2C_EC_MASK_REG)
#define D_UART_GetI2CExtClkInterruptSourceMasked()          (D_UART_INTR_I2C_EC_MASKED_REG)

#if (!D_UART_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define D_UART_SetSpiExtClkInterruptMode(interruptMask) \
                                                                D_UART_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define D_UART_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                D_UART_CLEAR_INTR_SPI_EC(interruptMask)
    #define D_UART_GetExtSpiClkInterruptSource()                 (D_UART_INTR_SPI_EC_REG)
    #define D_UART_GetExtSpiClkInterruptMode()                   (D_UART_INTR_SPI_EC_MASK_REG)
    #define D_UART_GetExtSpiClkInterruptSourceMasked()           (D_UART_INTR_SPI_EC_MASKED_REG)
#endif /* (!D_UART_CY_SCBIP_V1) */

#if(D_UART_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void D_UART_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (D_UART_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (D_UART_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_D_UART_CUSTOM_INTR_HANDLER)
    extern cyisraddress D_UART_customIntrHandler;
#endif /* !defined (CY_REMOVE_D_UART_CUSTOM_INTR_HANDLER) */
#endif /* (D_UART_SCB_IRQ_INTERNAL) */

extern D_UART_BACKUP_STRUCT D_UART_backup;

#if(D_UART_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 D_UART_scbMode;
    extern uint8 D_UART_scbEnableWake;
    extern uint8 D_UART_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 D_UART_mode;
    extern uint8 D_UART_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * D_UART_rxBuffer;
    extern uint8   D_UART_rxDataBits;
    extern uint32  D_UART_rxBufferSize;

    extern volatile uint8 * D_UART_txBuffer;
    extern uint8   D_UART_txDataBits;
    extern uint32  D_UART_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 D_UART_numberOfAddr;
    extern uint8 D_UART_subAddrSize;
#endif /* (D_UART_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (D_UART_SCB_MODE_I2C_CONST_CFG || \
        D_UART_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 D_UART_IntrTxMask;
#endif /* (! (D_UART_SCB_MODE_I2C_CONST_CFG || \
              D_UART_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(D_UART_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define D_UART_SCB_MODE_I2C_RUNTM_CFG     (D_UART_SCB_MODE_I2C      == D_UART_scbMode)
    #define D_UART_SCB_MODE_SPI_RUNTM_CFG     (D_UART_SCB_MODE_SPI      == D_UART_scbMode)
    #define D_UART_SCB_MODE_UART_RUNTM_CFG    (D_UART_SCB_MODE_UART     == D_UART_scbMode)
    #define D_UART_SCB_MODE_EZI2C_RUNTM_CFG   (D_UART_SCB_MODE_EZI2C    == D_UART_scbMode)
    #define D_UART_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (D_UART_SCB_MODE_UNCONFIG == D_UART_scbMode)

    /* Defines wakeup enable */
    #define D_UART_SCB_WAKE_ENABLE_CHECK       (0u != D_UART_scbEnableWake)
#endif /* (D_UART_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!D_UART_CY_SCBIP_V1)
    #define D_UART_SCB_PINS_NUMBER    (7u)
#else
    #define D_UART_SCB_PINS_NUMBER    (2u)
#endif /* (!D_UART_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_D_UART_H) */


/* [] END OF FILE */
