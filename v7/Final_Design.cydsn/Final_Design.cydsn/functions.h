#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <project.h>
#include <stdint.h>

#define NUM_OF_SAMPLES 1024  /**< Total number of ADC samples to process */

typedef enum {IDLE, SAMPLING, UART_TRANSFER} State;

extern State state;                         /**< Current state of the system */
extern uint16_t adcBuffer[NUM_OF_SAMPLES];  /**< Buffer to hold ADC samples */
extern volatile uint8_t buttonPressed;      /**< Flag set in Button ISR */
extern uint8_t sampleCount;                 /**< Count of sampling iterations */
extern volatile uint8_t uart_s_received;    /**< 's' command received flag */
extern volatile uint8_t uart_o_received;    /**< 'o' command received flag */
extern int32 fftOutput[2 * NUM_OF_SAMPLES]; /**< Output of FFT (interleaved real/imaginary pairs) */

/** @brief Button interrupt handler. */
CY_ISR_PROTO(Button_ISR_Handler);

/** @brief UART interrupt handler. */
CY_ISR_PROTO(UART_ISR_Handler);

/**
 * @brief Sample data from ADC and store in adcBuffer.
 */
void sampleADC(void);

/**
 * @brief Send raw ADC buffer data over UART.
 */
void uartSendBuffer(void);

/**
 * @brief Send FFT output and CFAR flags over UART.
 */
void uartSendFFTBuffer(void);

/**
 * @brief Update LED states.
 *
 * @param red Red LED state (0 or 1)
 * @param orange Orange LED state (0 or 1)
 * @param green Green LED state (0 or 1)
 */
void updateLEDs(uint8_t red, uint8_t orange, uint8_t green);

/**
 * @brief Initialize peripherals and interrupts.
 */
void initializeHardware(void);

/**
 * @brief Flush any leftover characters in UART RX buffer.
 */

void flushRxBuffer(void);

#endif