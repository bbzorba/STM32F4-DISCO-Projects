
//enable clock access to UART
RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

//enable clock access to GPIOA
RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

//set the alternate function type to UART for PA2 and PA3
RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

//Configure type of alternate function

//Configure UART mode