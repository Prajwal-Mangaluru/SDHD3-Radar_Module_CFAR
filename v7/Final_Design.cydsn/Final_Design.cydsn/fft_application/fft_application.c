/* =================================================================
 * Copyright Hochschule Darmstadt, 2020
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY 
 * OF H_DA.
 *
 * Author: Guillermo Urizar
 * Tutor: Prof. Dr. Stephen Bannwarth
 * Spring Project
 *
 * FFT Module for 24GHz-Radar for Movement Detection
 * v1.0
 *
 * Content of file: Source file for the FFT application.
 * =================================================================
*/

#include "fft_application.h"
#include "arm_math.h"
#include "arm_const_structs.h"

extern void fft_app(uint16 *adc_samples, int32 *fft_output, uint16 no_of_samples)
{
    int32 *fft_input = NULL;
    
    arm_status status;
    arm_rfft_instance_q31 fft_instance;
    
    //Initiating an instance of a Q31 real FFT calculation
    status = arm_rfft_init_q31(&fft_instance,no_of_samples,0,1);

    //Allocating the necessary memory for the input buffer of the calculation.
    //In order to maintain the original time domain data, a separate input
    //buffer is mandatory, due to the destructive nature of the CMSIS FFT
    //algorithm towards the input. This is because the input buffer is also
    //used to store partial results of the calculation.
    fft_input = (int32*)malloc(no_of_samples*sizeof(int32));
    for(int i=0 ; i<no_of_samples ; i++)
    {
        fft_input[i] = (int32)adc_samples[i];
    }
    
    //Calling the CMSIS real FFT function.
    arm_rfft_q31(&fft_instance,(q31_t*)fft_input,(q31_t*)fft_output);
    
    //Freeing the previously allocated memory.
    free(fft_input);
}

/* [] END OF FILE */