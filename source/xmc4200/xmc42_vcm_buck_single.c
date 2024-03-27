/******************************************************************************
* File Name:   xmc42_vcm_buck_single.c
*
* Description: This file provides functions for initializing the converter
*              structure, and performing the conversion. The converter runs in
*              an ISR enabled during the initialization.
*
*              This is the voltage control buck implementation which measures
*              the output voltage with an ADC channel, calculates the error to
*              the target output voltage, process it by the 3 pole 3 zero filter
*              and applies the results to the PWM generated by the HRPWM.
*
* Related Document: See README.md
*
*
*******************************************************************************
* Copyright 2022-2023, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/
#include "cybsp.h"
#include "cy_utils.h"
#include "xmc_3p3z_filter_float.h"
#include "xmc42_vcm_buck_single.h"

#if (UC_FAMILY == XMC4)
/*******************************************************************************
* Macros
********************************************************************************/
/* Here 3p3z filter is used in the compensator as per the below equation.
* y[n] = B0*x[n] + B1*x[n-1] + B2*x[n-2] + B3*x[n-3] + A1*y[n-1] + A2*y[n-2] + A3*y[n-3]
* Coefficients for the filter is shown below.
* These coefficients are calculated for the following configuration.
*
* Vout             = 3.3 V
* Switching freq   = 200 kHz
* Crossover freq   = 10 kHz
* Phase margin     = 50 degrees
* PWM master clock = 20,480 GHz = 80 MHz *256 (8 bits for HRPWM)
* PWM resolution   = 150 ps
* ADC resolution   = 12 bits
* Max duty         = 90%
*/
#define B0  (+1.072329384164)
#define B1  (-1.009391619615)
#define B2  (-1.071806296352)
#define B3  (+1.009914707427)
#define A1  (+1.611302392630)
#define A2  (-0.426276608711)
#define A3  (-0.185025783919)
#define K   (+105.121205758148)
#define REF (3215) /* For a reference output voltage of 3.3 V */
#define DUTY_TICKS_MIN (0)
#define DUTY_TICKS_MAX (92160)/* Max allowed duty cycle set to 90% of switching period */

/* Maximum value of high resolution positioning based on operating clock */
#define MAX_HRPWM_POS 82

/* Number of bits available for HRPWM */
#define NUM_BITS_HRPWM 8

/* ADC channel reading output voltage */
#define ADC_CH_VOUT               6U

/* Priority for ADC interrupt.*/
#define ADC_ISR_PRIORITY_HIGH     63U
/*******************************************************************************
* Function Prototypes
********************************************************************************/

/*******************************************************************************
* Global Variable
*******************************************************************************/
volatile XMC_VADC_RESULT_SIZE_t adc_result =0;
XMC_3P3Z_DATA_FLOAT_t ctrlFloat;

/*******************************************************************************
* Function Name: VADC0_G0_0_IRQHandler
********************************************************************************
* Summary:
* Interrupt service routine triggered by the ADC which is used for reading the
* output voltage. The compensator algorithm is running inside this ISR.
* The compensator calculates the PWM compare values for the next cycle and
* writes it to the compare register of the PWM.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void VADC0_G0_0_IRQHandler(void)
{
    /* Read result from ADC result register. */
    adc_result = XMC_VADC_GROUP_GetResult(VADC_G0, ADC_CH_VOUT);

    /* 3P3Z filter */
    XMC_3P3Z_FilterFloat(&ctrlFloat);

    /* Updating the PWM compare register with the calculated values. */
    /* Writing Higher 16 bits to the low resolution PWM. */
    XMC_CCU8_SLICE_SetTimerCompareMatchChannel1(((XMC_CCU8_SLICE_t *)CCU80_CC80),
                                                ((ctrlFloat.m_Out)>>NUM_BITS_HRPWM));

    /* Writing lower 8 bits to the high resolution PWM. */
    XMC_HRPWM_HRC_SetCompare2(HRPWM0_HRC0,
                              ((ctrlFloat.m_Out & 0xFF)*(MAX_HRPWM_POS))/256);

    /* Initiating the shadow transfer. */
    XMC_CCU8_EnableShadowTransfer(((XMC_CCU8_MODULE_t*)CCU80_BASE),
                                  (uint32_t)XMC_CCU8_SHADOW_TRANSFER_SLICE_0);
}

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* Main function performing the initialization of the peripherals, compensator,
* and interrupt.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void xmc42_vcm_buck_single_init(void)
{
    /* Initializing the compensator with the values for the required regulator
    configuration. */
    XMC_3P3Z_InitFloat(&ctrlFloat,
                       B0,
                       B1,
                       B2,
                       B3,
                       A1,
                       A2,
                       A3,
                       K,
                       REF,
                       DUTY_TICKS_MIN,
                       DUTY_TICKS_MAX,
                       (uint32_t*)&adc_result);

    /* Starting the timer. */
    XMC_CCU8_SLICE_StartTimer((XMC_CCU8_SLICE_t*) CCU80_CC80);

    /* Initializing the interrupt. */
    NVIC_SetPriority(VADC0_G0_0_IRQn,
                     NVIC_EncodePriority(NVIC_GetPriorityGrouping(),
                     ADC_ISR_PRIORITY_HIGH,
                     0));

    NVIC_EnableIRQ(VADC0_G0_0_IRQn);
}

#endif /*(UC_FAMILY == XMC4)*/
/* [] END OF FILE */