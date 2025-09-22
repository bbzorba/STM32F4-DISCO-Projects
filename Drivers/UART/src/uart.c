#include "uart.h"

#define RCC_APB1ENR_USART2EN ((uint32_t)0x00020000)          // Bit 17
#define RCC_AHB1ENR_GPIOAEN  ((uint32_t)0x00000001)          // Bit 0

#define GPIOA_MODER_ALTERNATE ((uint32_t)0x000000F0)         // mask to clear PA2/PA3 mode bits
#define GPIOA_MODER_PIN2     ((uint32_t)0x00000020)          // set AF (10) for PA2
#define GPIOA_MODER_PIN3     ((uint32_t)0x00000080)          // set AF (10) for PA3

#define GPIOA_AFR_TYPE       ((uint32_t)0x0000FF00)          // AFRL for PA2 and PA3
#define GPIOA_AFR_VALUE      ((uint32_t)0x00007700)          // AF7 for USART2

#define BAUD_RATE 115200

void USART2_Init(void)
{
    //1. enable clock access to UART
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    //2. enable clock access to GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    //3. enable pins for alternate function, PA2 and PA3
    GPIOA->MODER   &= ~(GPIOA_MODER_ALTERNATE);              // Clear mode bits for PA2 and PA3
    GPIOA->MODER   |= (GPIOA_MODER_PIN2 | GPIOA_MODER_PIN3); // Set mode to Alternate Function

    //4. configure type of alternate function
    GPIOA->AFR[0]  &= ~(GPIOA_AFR_TYPE);                     // Clear AFRL for PA2 and PA3
    GPIOA->AFR[0]  |= (GPIOA_AFR_VALUE);                     // Set AF7 for USART2

    //5. Configure UART mode
    USART2->BRR = 42000000 / BAUD_RATE;                      // Set baud rate to 115200
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;              // Enable transmitter and receiver
    // No parity / flow control adjustments needed
    USART2->CR2 |= 0;                                        // No-op
    USART2->CR3 |= 0;                                        // No-op
    USART2->CR1 |= USART_CR1_UE;                             // Enable USART2

}

void USART2_Write(int ch)
{
    //1. wait until TXE is set in the SR register
    while(!(USART2->SR & USART_SR_TXE));

    //2. write to DR register
    USART2->DR = (ch & 0xFF);

    return ch;
}

int USART2_Read(void)
{
    //1. wait until RXNE is set in the SR register
    while(!(USART2->SR & USART_SR_RXNE));

    //2. read data from DR register
    return (USART2->DR & 0xFF);
}

int fgetc(FILE *f) {
    int c;
    c = USART2_Read();                          //read a character from USART2
    if (c == '\r') {                            // if received character is carriage return
        USART2_Write(c);                        // send carriage return
        c = '\n';                               // change carriage return to newline
    }
    USART2_Write(c);                            // send the character
    return c;
}

int fputc(int c, FILE *f) {
    USART2_Write(c);                            // write a character to USART2
    return c;
}

int n;
char str[80];

void test_setup(void)
{
    printf("Please enter a number: ");
    scanf("%d", &n);
    printf("You entered: %d\r\n", n);

    printf("Please enter a string: ");
    scanf("%s", str);
    printf("You entered: %s\r\n", str);

    puts(str);
    printf("\r\n");
}
