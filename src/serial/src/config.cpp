#include <serial.hpp>
#include <registers.hpp>


namespace USART {


#ifdef __AVR_ATmega328P__
    USART PORT[NUM_PORTS] = {
        USART{0xc0},
    };
#endif


void USART::setConfiguration(const Settings &settings) {

    // Disable USART (temporarily)
    // Disable interrupts
    // Set the high bits for 8-bit mode
    B_REG = (0 << 3) | (0 << 5) | (0 << 2);

    // Clear the transmit complete flag by writing one to it
    // Disable multi-processor communication
    // Set 2X if needed
    A_REG = (1 << 6) | (0 << 0) | (settings.use_2X << 1);

    // Set the mode to asynchronous
    // Set the data size to 8-bit
    // Set the parity and stop bits
    C_REG = (0 << 6) | (3 << 1) | (settings.parity << 4) | (settings.stop_bits << 3);

    // Set the baud-rate register
    BAUD_REG = settings.baudrate_register;

    // Finally, enable the reciever and transmitter as needed
    // Use the same settings from before
    B_REG = (0 << 5) | (0 << 2) | (settings.rx_en << 4) | (settings.tx_en << 3);
}

Settings USART::getConfiguration() {

    // Get the values in all the registers
    // This is so we don't repeatedly read from them
    uint16_t baud_val = BAUD_REG;
    uint8_t a_val = A_REG;
    uint8_t b_val = B_REG;
    uint8_t c_val = C_REG;

    // Fetch all the settings from the registers
    return Settings {
        // Baud-rate in the baud-rate register
        .baudrate_register = baud_val,

        // Settings from the A register
        // * U2X
        .use_2X = ((a_val >> 1) & 1) != 0,

        // Settings from the B register
        // * RXEN
        // * TXEN
        .rx_en = ((b_val >> 4) & 1) != 0,
        .tx_en = ((b_val >> 3) & 1) != 0,

        // Settings from the C register
        // * Stop bits
        // * Parity
        .stop_bits = static_cast<Settings::StopBits>((c_val >> 3) & 1),
        .parity    = static_cast<Settings::Parity>  ((c_val >> 4) & 3),
    };
}

Error USART::getError() const {
    // Errors are in the A register
    // We defined the ordering so it's just the value
    return static_cast<Error>((A_REG >> 2) & 7);
}


}; // namespace USART
