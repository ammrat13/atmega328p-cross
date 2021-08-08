#include <serial.hpp>


USART::Settings USART0_SETTINGS {
    .baudrate_register = 16,
    .use_2X = true,
    .tx_en = true,
    .rx_en = true,
    .stop_bits = USART::Settings::StopBits::ONE,
    .parity = USART::Settings::Parity::NONE,
};

int main(void) {

    size_t num_read;
    char buf[256];

    USART::PORT[0].setConfiguration(USART0_SETTINGS);

    while(1) {
        USART::PORT[0].gets(buf, 256, num_read, '\n');

        USART::PORT[0].puts(buf);
        USART::PORT[0].putc('\n');
    }
}
