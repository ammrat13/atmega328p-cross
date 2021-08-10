#include <serial.hpp>


USART::Configuration USART0_CONFIGURATION {
    .baudrate_register = 16,
    .use_2X = true,
    .rx_en = true,
    .tx_en = true,
    .stop_bits = USART::Configuration::StopBits::ONE,
    .parity = USART::Configuration::Parity::NONE,
};

int main(void) {

    size_t num_read;
    char buf[256];

    USART::PORT[0].setConfiguration(USART0_CONFIGURATION);

    while(1) {
        USART::PORT[0].gets(buf, 256, num_read, '\n');
        USART::PORT[0].puts(buf);
    }
}
