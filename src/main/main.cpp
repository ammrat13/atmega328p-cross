#include <serial.hpp>


USART::Settings USART0_SETTINGS {
    .baudrate_register = 16,
    .use_2X = true,
    .tx_en = true,
    .rx_en = false,
    .stop_bits = USART::Settings::StopBits::ONE,
    .parity = USART::Settings::Parity::NONE,
};

int main(void) {
    USART::PORT[0].configure(USART0_SETTINGS);
    while(1){
        USART::PORT[0].putc('A');
    }
}
