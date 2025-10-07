#include "uart.h"


int USART_2_Write(int ch)
{
    //1. wait until TXE is set in the SR register
    while(!(USART_2->SR & USART_SR_TXE));

    //2. write to DR register
    USART_2->DR = (ch & 0xFF);

    return ch;
}

int USART_2_Read(void)
{
    //1. wait until RXNE is set in the SR register
    while(!(USART_2->SR & USART_SR_RXNE));

    //2. read data from DR register
    return (USART_2->DR & 0xFF);
}

int fgetc(FILE *f) {
    int c;
    c = USART_2_Read();                          //read a character from USART_2
    if (c == '\r') {                            // if received character is carriage return
        USART_2_Write(c);                        // send carriage return
        c = '\n';                               // change carriage return to newline
    }
    USART_2_Write(c);                            // send the character
    return c;
}

int fputc(int c, FILE *f) {
    USART_2_Write(c);                            // write a character to USART_2
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
