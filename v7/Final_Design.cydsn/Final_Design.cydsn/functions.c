#include "functions.h"

State state = IDLE;                               /**< Initial state */
uint16_t adcBuffer[NUM_OF_SAMPLES];               /**< ADC sample buffer */
volatile uint8_t buttonPressed = 0;               /**< Button press flag */
uint8_t sampleCount = 0;                          /**< Sampling loop counter */
volatile uint8_t uart_s_received = 0;             /**< UART 's' received flag */
volatile uint8_t uart_o_received = 0;             /**< UART 'o' received flag */
int32 fftOutput[2 * NUM_OF_SAMPLES];              /**< FFT output buffer */
extern uint8_t cfar_output[NUM_OF_SAMPLES];       /**< CFAR result */

CY_ISR(Button_ISR_Handler)
{
    buttonPressed = 1;
    isr_Button_ClearPending();
    Button_ClearInterrupt();
}

CY_ISR(UART_ISR_Handler)
{
    uint8_t rxChar = UART_LOG_GetChar();
    if (rxChar == 's')
    {
        uart_s_received = 1;
    }
    else if (rxChar == 'o')
    {
        uart_o_received = 1;
    }
    isr_UART_ClearPending();
}

void sampleADC()
{
    for (int i = 0; i < NUM_OF_SAMPLES; i++)
    {
        ADC_StartConvert();
        while (!ADC_IsEndConversion(ADC_RETURN_STATUS));
        adcBuffer[i] = ADC_GetResult16();
    }
}

void uartSendBuffer()
{
    for (int i = 0; i < NUM_OF_SAMPLES; i++)
    {
        UART_LOG_PutChar(adcBuffer[i] & 0xFF);
        UART_LOG_PutChar((adcBuffer[i] >> 8) & 0xFF);
    }
}

void uartSendFFTBuffer()
{
    for (int i = 0; i < 2 * NUM_OF_SAMPLES; i++)
    {
        UART_LOG_PutChar(fftOutput[i] & 0xFF);
        UART_LOG_PutChar((fftOutput[i] >> 8) & 0xFF);
        UART_LOG_PutChar((fftOutput[i] >> 16) & 0xFF);
        UART_LOG_PutChar((fftOutput[i] >> 24) & 0xFF);
    }
    
    for (int i = 0; i < NUM_OF_SAMPLES; i++)
    {
        UART_LOG_PutChar(cfar_output[i]);
    }
}

void updateLEDs(uint8_t red, uint8_t orange, uint8_t green)
{
    led_red_Write(red);
    led_orange_Write(orange);
    led_green_Write(green);
}

void initializeHardware()
{
    WaveDAC8_1_Start();
    UART_LOG_Start();
    ADC_Start();
    isr_Button_StartEx(Button_ISR_Handler);
    isr_UART_StartEx(UART_ISR_Handler);
    updateLEDs(0, 0, 0);
}

void flushRxBuffer()
{
    while (UART_LOG_GetRxBufferSize())
    {
        UART_LOG_GetChar();
    }
}