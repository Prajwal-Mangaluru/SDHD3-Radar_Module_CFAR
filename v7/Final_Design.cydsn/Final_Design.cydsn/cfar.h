#ifndef CFAR_H
#define CFAR_H

#include <stdint.h>
#include "project.h"
#define NUM_OF_SAMPLES 1024  /**< Total number of samples processed in FFT and CFAR */

uint8_t cfar_output[NUM_OF_SAMPLES]; /**< Array holding CFAR detection flags (1 = detection, 0 = no detection) */

/**
 * @brief Run Cell Averaging CFAR (CA-CFAR) algorithm on FFT data.
 *
 * This function applies the CA-CFAR technique to detect targets in noisy FFT output.
 * The algorithm uses a sliding window with guard and reference cells to estimate noise
 * and compares each cell's magnitude with a threshold derived from the noise level.
 *
 * @param fft_buffer Pointer to interleaved Q31 FFT output (real and imag pairs).
 * @param num_samples Number of FFT bins (e.g., 1024).
 */
void run_cfar(const int32_t *fft_buffer, uint16_t num_samples);

#endif // CFAR_H
