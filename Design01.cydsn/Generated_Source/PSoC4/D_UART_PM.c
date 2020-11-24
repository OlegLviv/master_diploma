/***************************************************************************//**
* \file D_UART_PM.c
* \version 4.0
*
* \brief
*  This file provides the source code to the Power Management support for
*  the SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "D_UART.h"
#include "D_UART_PVT.h"

#if(D_UART_SCB_MODE_I2C_INC)
    #include "D_UART_I2C_PVT.h"
#endif /* (D_UART_SCB_MODE_I2C_INC) */

#if(D_UART_SCB_MODE_EZI2C_INC)
    #include "D_UART_EZI2C_PVT.h"
#endif /* (D_UART_SCB_MODE_EZI2C_INC) */

#if(D_UART_SCB_MODE_SPI_INC || D_UART_SCB_MODE_UART_INC)
    #include "D_UART_SPI_UART_PVT.h"
#endif /* (D_UART_SCB_MODE_SPI_INC || D_UART_SCB_MODE_UART_INC) */


/***************************************
*   Backup Structure declaration
***************************************/

#if(D_UART_SCB_MODE_UNCONFIG_CONST_CFG || \
   (D_UART_SCB_MODE_I2C_CONST_CFG   && (!D_UART_I2C_WAKE_ENABLE_CONST))   || \
   (D_UART_SCB_MODE_EZI2C_CONST_CFG && (!D_UART_EZI2C_WAKE_ENABLE_CONST)) || \
   (D_UART_SCB_MODE_SPI_CONST_CFG   && (!D_UART_SPI_WAKE_ENABLE_CONST))   || \
   (D_UART_SCB_MODE_UART_CONST_CFG  && (!D_UART_UART_WAKE_ENABLE_CONST)))

    D_UART_BACKUP_STRUCT D_UART_backup =
    {
        0u, /* enableState */
    };
#endif


/*******************************************************************************
* Function Name: D_UART_Sleep
****************************************************************************//**
*
*  Prepares the D_UART component to enter Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has an influence on this 
*  function implementation:
*  - Checked: configures the component to be wakeup source from Deep Sleep.
*  - Unchecked: stores the current component state (enabled or disabled) and 
*    disables the component. See SCB_Stop() function for details about component 
*    disabling.
*
*  Call the D_UART_Sleep() function before calling the 
*  CyPmSysDeepSleep() function. 
*  Refer to the PSoC Creator System Reference Guide for more information about 
*  power management functions and Low power section of this document for the 
*  selected mode.
*
*  This function should not be called before entering Sleep.
*
*******************************************************************************/
void D_UART_Sleep(void)
{
#if(D_UART_SCB_MODE_UNCONFIG_CONST_CFG)

    if(D_UART_SCB_WAKE_ENABLE_CHECK)
    {
        if(D_UART_SCB_MODE_I2C_RUNTM_CFG)
        {
            D_UART_I2CSaveConfig();
        }
        else if(D_UART_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            D_UART_EzI2CSaveConfig();
        }
    #if(!D_UART_CY_SCBIP_V1)
        else if(D_UART_SCB_MODE_SPI_RUNTM_CFG)
        {
            D_UART_SpiSaveConfig();
        }
        else if(D_UART_SCB_MODE_UART_RUNTM_CFG)
        {
            D_UART_UartSaveConfig();
        }
    #endif /* (!D_UART_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        D_UART_backup.enableState = (uint8) D_UART_GET_CTRL_ENABLED;

        if(0u != D_UART_backup.enableState)
        {
            D_UART_Stop();
        }
    }

#else

    #if (D_UART_SCB_MODE_I2C_CONST_CFG && D_UART_I2C_WAKE_ENABLE_CONST)
        D_UART_I2CSaveConfig();

    #elif (D_UART_SCB_MODE_EZI2C_CONST_CFG && D_UART_EZI2C_WAKE_ENABLE_CONST)
        D_UART_EzI2CSaveConfig();

    #elif (D_UART_SCB_MODE_SPI_CONST_CFG && D_UART_SPI_WAKE_ENABLE_CONST)
        D_UART_SpiSaveConfig();

    #elif (D_UART_SCB_MODE_UART_CONST_CFG && D_UART_UART_WAKE_ENABLE_CONST)
        D_UART_UartSaveConfig();

    #else

        D_UART_backup.enableState = (uint8) D_UART_GET_CTRL_ENABLED;

        if(0u != D_UART_backup.enableState)
        {
            D_UART_Stop();
        }

    #endif /* defined (D_UART_SCB_MODE_I2C_CONST_CFG) && (D_UART_I2C_WAKE_ENABLE_CONST) */

#endif /* (D_UART_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: D_UART_Wakeup
****************************************************************************//**
*
*  Prepares the D_UART component for Active mode operation after 
*  Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has influence on this 
*  function implementation:
*  - Checked: restores the component Active mode configuration.
*  - Unchecked: enables the component if it was enabled before enter Deep Sleep.
*
*  This function should not be called after exiting Sleep.
*
*  \sideeffect
*   Calling the D_UART_Wakeup() function without first calling the 
*   D_UART_Sleep() function may produce unexpected behavior.
*
*******************************************************************************/
void D_UART_Wakeup(void)
{
#if(D_UART_SCB_MODE_UNCONFIG_CONST_CFG)

    if(D_UART_SCB_WAKE_ENABLE_CHECK)
    {
        if(D_UART_SCB_MODE_I2C_RUNTM_CFG)
        {
            D_UART_I2CRestoreConfig();
        }
        else if(D_UART_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            D_UART_EzI2CRestoreConfig();
        }
    #if(!D_UART_CY_SCBIP_V1)
        else if(D_UART_SCB_MODE_SPI_RUNTM_CFG)
        {
            D_UART_SpiRestoreConfig();
        }
        else if(D_UART_SCB_MODE_UART_RUNTM_CFG)
        {
            D_UART_UartRestoreConfig();
        }
    #endif /* (!D_UART_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        if(0u != D_UART_backup.enableState)
        {
            D_UART_Enable();
        }
    }

#else

    #if (D_UART_SCB_MODE_I2C_CONST_CFG  && D_UART_I2C_WAKE_ENABLE_CONST)
        D_UART_I2CRestoreConfig();

    #elif (D_UART_SCB_MODE_EZI2C_CONST_CFG && D_UART_EZI2C_WAKE_ENABLE_CONST)
        D_UART_EzI2CRestoreConfig();

    #elif (D_UART_SCB_MODE_SPI_CONST_CFG && D_UART_SPI_WAKE_ENABLE_CONST)
        D_UART_SpiRestoreConfig();

    #elif (D_UART_SCB_MODE_UART_CONST_CFG && D_UART_UART_WAKE_ENABLE_CONST)
        D_UART_UartRestoreConfig();

    #else

        if(0u != D_UART_backup.enableState)
        {
            D_UART_Enable();
        }

    #endif /* (D_UART_I2C_WAKE_ENABLE_CONST) */

#endif /* (D_UART_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/* [] END OF FILE */
