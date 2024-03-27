/******************************************************************************
* File Name:   xmc_3p3z_filter_float.h
*
* Description: This file provides functions for initializing the filter structure
*              and performing the 3 poles 3 zeros filtering on the input data
*              using float values.
*
* Related Document: See README.md
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

#ifndef XMC_3P3Z_FILTER_H
#define XMC_3P3Z_FILTER_H

/*******************************************************************************
* MACROS
*******************************************************************************/

/**< Minimum value  calculation macro */
#define MIN(a,b) ((a) < (b) ? (a) : (b))
/**< Maximum value  calculation macro */
#define MAX(a,b) ((a) > (b) ? (a) : (b))

/******************************************************************************
* DATA STRUCTURES
******************************************************************************/
/* Structure defining the Filter calculation input parameters */
typedef struct XMC_3P3Z_DATA_FLOAT
{
  volatile uint32_t*  m_pFeedBack;    /* pointer to ADC register which is used for feedback */
  uint32_t            m_Out;          /* 24 bit integer (16 bit low resolution + 8 bit high resolution) */
  uint16_t            m_Ref;          /* ADC reference */
  float               m_A1;
  float               m_A2;
  float               m_A3;
  float               m_B0;
  float               m_B1;
  float               m_B2;
  float               m_B3;
  float               m_E[4];
  float               m_U[4];
  float               m_K;
  float               m_Min;
  float               m_Max;
} XMC_3P3Z_DATA_FLOAT_t;

/*******************************************************************************
* Function Name: XMC_3P3Z_InitFloat
********************************************************************************
* Summary:
* This API uses the raw coefficients for the filter and fills the filter structure.
*
* Parameters:
* XMC_3P3Z_DATA_FLOAT_t* [out] ptr Pointer to the filter structure
* float                  [in]  cB0 B0 filter coefficient
* float                  [in]  cB1 B1 filter coefficient
* float                  [in]  cB2 B2 filter coefficient
* float                  [in]  cB3 B3 filter coefficient
* float                  [in]  cA1 A1 filter coefficient
* float                  [in]  cA2 A2 filter coefficient
* float                  [in]  cA3 A3 filter coefficient
* float                  [in]  cK k factor of the filter
* uint16_t               [in]  ref Reference value for the VADC
* float                  [in]  pwmMin 24 bit min PWM value.
* float                  [in]  pwmMax 24 bit max PWM value.
* volatile uint32_t*     [out] pFeedBack pointer to ADC register.
*
* Return:
*  void
*
*******************************************************************************/
__STATIC_INLINE void XMC_3P3Z_InitFloat(XMC_3P3Z_DATA_FLOAT_t* ptr,
                                        float cB0,
                                        float cB1,
                                        float cB2,
                                        float cB3,
                                        float cA1,
                                        float cA2,
                                        float cA3,
                                        float cK,
                                        uint16_t ref,
                                        float pwmMin,
                                        float pwmMax,
                                        volatile uint32_t* pFeedBack )
{
  /*Resetting the filter structure values */
  memset( ptr, 0, sizeof(*ptr));

  /* Initializing Feedback, reference, Out and K values */
  ptr->m_pFeedBack  = pFeedBack;
  ptr->m_Ref        = ref;
  ptr->m_Out        = 0;
  ptr->m_K          = cK;

  /* Initializing coefficients */
  ptr->m_A1         = cA1;
  ptr->m_A2         = cA2;
  ptr->m_A3         = cA3;
  ptr->m_B0         = cB0*cK;
  ptr->m_B1         = cB1*cK;
  ptr->m_B2         = cB2*cK;
  ptr->m_B3         = cB3*cK;

  /* Initializing maximum and minimum PWM value*/
  ptr->m_Min        = pwmMin;
  ptr->m_Max        = pwmMax;
}

/*******************************************************************************
* Function Name: XMC_3P3Z_InitFloat
********************************************************************************
* Summary:
* This function performs the 3p3z filtering by using floating point coefficients
*
* Parameters:
* XMC_3P3Z_DATA_FLOAT_t* [in/out] ptr Pointer to the filter structure
*
* Return:
*  void
*
*******************************************************************************/
__STATIC_INLINE void XMC_3P3Z_FilterFloat(XMC_3P3Z_DATA_FLOAT_t* ptr )
{
  float acc;

  /* Filter calculations */
  acc = ptr->m_B3*ptr->m_E[2]; ptr->m_E[2] = ptr->m_E[1];
  acc += ptr->m_B2*ptr->m_E[1]; ptr->m_E[1] = ptr->m_E[0];
  acc += ptr->m_B1*ptr->m_E[0]; ptr->m_E[0] = (float)(ptr->m_Ref-((uint16_t)*ptr->m_pFeedBack));
  acc += ptr->m_B0*ptr->m_E[0];

  acc += ptr->m_A3*ptr->m_U[2]; ptr->m_U[2] = ptr->m_U[1];
  acc += ptr->m_A2*ptr->m_U[1]; ptr->m_U[1] = ptr->m_U[0];
  acc += ptr->m_A1*ptr->m_U[0];

  /* Max/Min truncation */
  acc = MIN( acc , ptr->m_Max );
  acc = MAX( acc , -ptr->m_Max );
  ptr->m_U[0] = acc;
  if ( acc < ptr->m_Min) acc = ptr->m_Min;

  /*Filter Output*/
  ptr->m_Out = (uint32_t)acc;
}

#endif /* #ifndef XMC_3P3Z_FILTER_H */
