#include <serial.hpp>


USART::Settings USART0_SETTINGS {
    .baudrate = USART::Settings::BAUDRATE_FLAG_USE_2X | 16,
    .flags = USART::Settings::FLAG_TX_EN,
};

int main(void) {
    USART::PORT[0].configure(USART0_SETTINGS);
    while(1){
        USART::PORT[0].putc('A');
    }
}
