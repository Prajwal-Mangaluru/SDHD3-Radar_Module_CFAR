#include "cfar.h"
#include <math.h>

extern uint8_t cfar_output[NUM_OF_SAMPLES]; /**< CFAR detection output array */

void run_cfar(const int32_t *fft_buffer, uint16_t num_samples)
{
    const int NG = 2;              /**< Number of guard cells */
    const int NR = 12;             /**< Number of reference cells */
    const float PFA = 1e-2f;       /**< Probability of false alarm */
    const float MIN_MAG = 1e-4f;   /**< Minimum magnitude threshold */
    const float alpha = NR * (powf(PFA, -1.0f / NR) - 1.0f); /**< Threshold scaling factor */

    for (int i = 0; i < num_samples; i++)
        cfar_output[i] = 0; /**< Clear detection output */

    for (int i = NR + NG; i < num_samples - (NR + NG); i++)
    {
        float noise_sum = 0.0f;

        /**< Sum power in leading reference cells */
        for (int j = i - NR - NG; j < i - NG; j++)
        {
            float re = fft_buffer[2 * j]     / 2147483648.0f;
            float im = fft_buffer[2 * j + 1] / 2147483648.0f;
            noise_sum += re * re + im * im;
        }

        /**< Sum power in lagging reference cells */
        for (int j = i + NG + 1; j <= i + NG + NR; j++)
        {
            float re = fft_buffer[2 * j]     / 2147483648.0f;
            float im = fft_buffer[2 * j + 1] / 2147483648.0f;
            noise_sum += re * re + im * im;
        }

        float noise_avg = noise_sum / (2.0f * NR);        /**< Average noise power */
        float threshold = alpha * noise_avg;              /**< Detection threshold */
        float re = fft_buffer[2 * i]     / 2147483648.0f; /**< Real part of CUT */
        float im = fft_buffer[2 * i + 1] / 2147483648.0f; /**< Imaginary part of CUT */
        float magnitude = re * re + im * im;              /**< Power at CUT */

        if (magnitude > threshold && magnitude > MIN_MAG)
        {
            cfar_output[i] = 1;      /**< Mark detection */
            led_red_Write(1);       /**< Turn on red LED */
        }
    }
}
