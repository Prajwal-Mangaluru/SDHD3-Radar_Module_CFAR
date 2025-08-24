#include "project.h"
#include "functions.h"
#include <stdlib.h>
#include "fft_application.h"
extern int32 fftOutput[2 * NUM_OF_SAMPLES];

int main(void)
{
    CyGlobalIntEnable;
    initializeHardware();

    for (;;)
    {
        switch (state)
        {
            case IDLE:
                updateLEDs(0, 1, 1);  /**< Orange + Green ON */
                flushRxBuffer();
                if (buttonPressed == 1)
                {
                    CyDelay(300);
                    buttonPressed = 0;
                    sampleCount = 0;
                    state = SAMPLING;
                }
                break;

            case SAMPLING:
                updateLEDs(0, 1, 0);  /**< Orange ON */
                sampleADC();          /**< Capture ADC samples */
                fft_app(adcBuffer, fftOutput, NUM_OF_SAMPLES); /**< FFT and CFAR */
                if (uart_s_received)
                {
                    uart_s_received = 0;
                    state = UART_TRANSFER;
                }
                else
                {
                    state = IDLE;
                }
                break;

            case UART_TRANSFER:
                updateLEDs(0, 0, 1); /**< Green ON */
                uartSendFFTBuffer();
                sampleCount++;
                if (uart_o_received)
                {
                    uart_o_received = 0;
                    if (sampleCount >= 10)
                    {
                        state = IDLE;
                    }
                    else
                    {
                        state = SAMPLING;
                    }
                }
                else
                {
                    CyDelay(100);
                }
                break;
        }
    }
}
