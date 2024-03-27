/******************************************************************************
* File Name:   xmc_3p3z_filter_fixed.h
*
* Description: This file provides functions for initializing the filter
*              structure and performing the 3 poles 3 zeros filtering on the
*              input data using fixed values.
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

#ifndef XMC_3P3Z_FILTER_FIXED_H
#define XMC_3P3Z_FILTER_FIXED_H

/******************************************************************************
 * MACROS
 *****************************************************************************/
/**< Minimum value  calculation macro */
#define MIN(a,b) ((a) < (b) ? (a) : (b))
/**< Maximum value  calculation macro */
#define MAX(a,b) ((a) > (b) ? (a) : (b))
/**< Fix point to float calculation macro */
#define FIX_TO_FLOAT( i, q ) ((float)(i) / ((unsigned)1<<(q)) )
/**< Fix point from float calculation macro */
#define FIX_FROM_FLOAT( f, q ) (int)((f) * ((unsigned)1<<(q)) )

/******************************************************************************
 * DATA STRUCTURES
 *****************************************************************************/

/**
 * Structure defining the Filter calculation input parameters
 */
typedef struct XMC_3P3Z_DATA_FIXED
{
  /**< pointer to ADC register which is used for feedback */
  volatile uint32_t*  m_pFeedBack;
  uint32_t            m_pOut;
  int32_t             m_Acc;
  int32_t             m_KpwmMin;
  int32_t             m_KpwmMax;
  int32_t             m_KpwmMaxNeg;
  int32_t             m_Ref;        /**< ADC reference */
  int32_t             m_B[4];
  int32_t             m_A[4];
  int32_t             m_E[4];
  int32_t             m_U[3];
  int                 m_AShift;
  int                 m_BShift;
  int                 m_OShift;
} XMC_3P3Z_DATA_FIXED_t;

/******************************************************************************
 * API Prototypes
 *****************************************************************************/

/*******************************************************************************
* Function Name: XMC_3P3Z_InitFixed
********************************************************************************
* Summary:
* This API uses the raw coefficients for the filter and fills the filter structure.
*
* Parameters:
 * XMC_3P3Z_DATA_FIXED_t* [out] ptr Pointer to the filter structure
 * float                  [in]  cB0 B0 filter coefficient
 * float                  [in]  cB1 B1 filter coefficient
 * float                  [in]  cB2 B2 filter coefficient
 * float                  [in]  cB3 B3 filter coefficient
 * float                  [in]  cA1 A1 filter coefficient
 * float                  [in]  cA2 A2 filter coefficient
 * float                  [in]  cA3 A3 filter coefficient
 * float                  [in]  cK k factor of the filter
 * uint16_t               [in]  ref Reference value for the VADC
 * uint16_t               [in]  pwmMin 24 bit min PWM value.
 * uint16_t               [in]  pwmMax 24 bit max PWM value.
 * uint32_t*              [out] pFeedBack pointer to ADC register.
*
* Return:
*  void
*
*******************************************************************************/
__STATIC_INLINE void XMC_3P3Z_InitFixed(XMC_3P3Z_DATA_FIXED_t* ptr,
                                        float cB0,
                                        float cB1,
                                        float cB2,
                                        float cB3,
                                        float cA1,
                                        float cA2,
                                        float cA3,
                                        float cK,
                                        uint16_t ref,
                                        uint16_t pwmMin,
                                        uint16_t pwmMax,
                                        volatile uint32_t* pFeedBack)
{
  int A_iq, B_iq, U_iq;
  int AU_iq, BE_iq;

  /*Resetting the filter structure values */
  memset( ptr, 0, sizeof(*ptr));

  /* Initializing Feedback, reference, and OUT values Out  */
  ptr->m_pFeedBack  = pFeedBack;
  ptr->m_Ref    = ref;
  ptr->m_pOut   = 0;

  /*          IQ int      iQ fract    Bit size
     B        -1          19          19
     E         12          0          13
     ------------------------
     sum BnEn  12         19          32       */

  B_iq = 19;
  /*E_iq = 0;*/
  BE_iq = 19;

  /* Initializing coefficients */
  ptr->m_B[3] = FIX_FROM_FLOAT(cB3*cK,B_iq);
  ptr->m_B[2] = FIX_FROM_FLOAT(cB2*cK,B_iq);
  ptr->m_B[1] = FIX_FROM_FLOAT(cB1*cK,B_iq);
  ptr->m_B[0] = FIX_FROM_FLOAT(cB0*cK,B_iq);

  /*         IQ int      iQ fract    Bit size
     A         1          14          16
     U         8          8           17
     ------------------------
     sum AnUn  9          22          32       */
  A_iq = 14;
  U_iq = 8;
  AU_iq = 22;
  ptr->m_A[3] = FIX_FROM_FLOAT(cA3,A_iq);
  ptr->m_A[2] = FIX_FROM_FLOAT(cA2,A_iq);
  ptr->m_A[1] = FIX_FROM_FLOAT(cA1,A_iq);

  /* Initializing maximum and minimum PWM value */
  ptr->m_KpwmMin        = pwmMin;
  ptr->m_KpwmMax        = FIX_FROM_FLOAT((pwmMax-1),U_iq);
  ptr->m_KpwmMaxNeg     = -ptr->m_KpwmMax;

  /* Initializing shifting values */
  ptr->m_AShift = AU_iq - BE_iq;
  ptr->m_BShift = BE_iq - U_iq;
  ptr->m_OShift = U_iq;
}

/*******************************************************************************
* Function Name: XMC_3P3Z_FilterFixed
********************************************************************************
* Summary:
* This function performs the 3p3z filtering by using fix point coefficients
*
* Parameters:
* XMC_3P3Z_DATA_FIXED_t* [in/out] ptr Ptr Pointer to the filter structure
*
* Return:
*  void
*
*******************************************************************************/
__STATIC_INLINE void XMC_3P3Z_FilterFixed( XMC_3P3Z_DATA_FIXED_t* ptr )
{
    int32_t acc;

    /* Filter calculations */
    /* acc (iq9.22) = An (iq-1.19) * Un (iq(8.8)*/
    acc  = ptr->m_A[3]*ptr->m_U[2]; ptr->m_U[2] = ptr->m_U[1];
    acc += ptr->m_A[2]*ptr->m_U[1]; ptr->m_U[1] = ptr->m_U[0];
    acc += ptr->m_A[1]*ptr->m_U[0];
    acc = acc >> ptr->m_AShift;  /*iq is now iq9.19*/

    /* acc (iq12.19) = Bn (iq1.14) * En (iq(12.0)*/
    acc += ptr->m_B[3]*ptr->m_E[2]; ptr->m_E[2] = ptr->m_E[1];
    acc += ptr->m_B[2]*ptr->m_E[1]; ptr->m_E[1] = ptr->m_E[0];
    acc += ptr->m_B[1]*ptr->m_E[0]; ptr->m_E[0] =
                     ptr->m_Ref-((uint16_t)*ptr->m_pFeedBack);
    acc += ptr->m_B[0]*ptr->m_E[0];

    /*our number is now a iq12.19, but we need to store U as a iq8.8*/
    acc = acc >> ptr->m_BShift; /*now its a iq12.8*/

    /* Max/Min truncation */
    acc = MIN( acc , ptr->m_KpwmMax );
    acc = MAX( acc , ptr->m_KpwmMaxNeg ); /*now its a iq8.8*/
    ptr->m_U[0] = acc;

    acc = acc >> ptr->m_OShift; /*now its a iq8.0*/
    if ( acc < ptr->m_KpwmMin) acc = ptr->m_KpwmMin;

    /*Filter Output*/
    ptr->m_pOut = acc;
}

#endif /* #ifndef XMC_3P3Z_FILTER_FIXED_H */
