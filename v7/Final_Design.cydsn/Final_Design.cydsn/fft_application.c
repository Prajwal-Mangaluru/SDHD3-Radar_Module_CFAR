#include "fft_application.h"
#include <stdlib.h>
#include "arm_math.h"
#include "cfar.h"
#include "project.h"

void fft_app(uint16 *adc_samples, int32 *fft_output, uint16 no_of_samples)
{
    int32 *fft_input = NULL; /**< Buffer for FFT input */
    
    arm_status status;
    arm_rfft_instance_q31 fft_instance;
    
    status = arm_rfft_init_q31(&fft_instance, no_of_samples, 0, 1); /**< Initialize RFFT */
    if (status != ARM_MATH_SUCCESS)
    {
        return;
    }

    fft_input = (int32*)malloc(no_of_samples * sizeof(int32));
    if (fft_input == NULL)
    {
        return;
    }

    /**< Convert ADC values to signed Q31 format */
    for (int i = 0; i < no_of_samples; i++)
    {
        fft_input[i] = ((int32)adc_samples[i] - 2048) << 19;
    }

    arm_rfft_q31(&fft_instance, (q31_t*)fft_input, (q31_t*)fft_output); /**< Compute FFT */
    free(fft_input);
    
    run_cfar((int32_t*)fft_output, no_of_samples); /**< Run CFAR detection */

    uint8_t detectionFound = 0; /**< Flag to indicate detection */
    for (int i = 0; i < no_of_samples; i++)
    {
        if (cfar_output[i])
        {
            detectionFound = 1;
            led_red_Write(1);
            break;
        }
    }

    if (detectionFound)
    {
        led_red_Write(1);  // Turn ON red LED
    }
    else
    {
        led_red_Write(0);  // Turn OFF red LED
    }
}
