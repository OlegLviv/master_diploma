/*******************************************************************************
* File Name: switch_pin.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "switch_pin.h"

static switch_pin_BACKUP_STRUCT  switch_pin_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: switch_pin_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet switch_pin_SUT.c usage_switch_pin_Sleep_Wakeup
*******************************************************************************/
void switch_pin_Sleep(void)
{
    #if defined(switch_pin__PC)
        switch_pin_backup.pcState = switch_pin_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            switch_pin_backup.usbState = switch_pin_CR1_REG;
            switch_pin_USB_POWER_REG |= switch_pin_USBIO_ENTER_SLEEP;
            switch_pin_CR1_REG &= switch_pin_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(switch_pin__SIO)
        switch_pin_backup.sioState = switch_pin_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        switch_pin_SIO_REG &= (uint32)(~switch_pin_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: switch_pin_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to switch_pin_Sleep() for an example usage.
*******************************************************************************/
void switch_pin_Wakeup(void)
{
    #if defined(switch_pin__PC)
        switch_pin_PC = switch_pin_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            switch_pin_USB_POWER_REG &= switch_pin_USBIO_EXIT_SLEEP_PH1;
            switch_pin_CR1_REG = switch_pin_backup.usbState;
            switch_pin_USB_POWER_REG &= switch_pin_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(switch_pin__SIO)
        switch_pin_SIO_REG = switch_pin_backup.sioState;
    #endif
}


/* [] END OF FILE */
