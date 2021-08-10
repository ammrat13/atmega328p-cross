#include <serial.hpp>
#include <registers.hpp>


namespace USART {


const Configuration Configuration::STARTUP = Configuration {
    .baudrate_register = 0,
    .use_2X = false,
    .rx_en = false,
    .tx_en = false,
    .stop_bits = Configuration::StopBits::ONE,
    .parity    = Configuration::Parity::NONE,
};


USART::USART(uintptr_t base) :
    base_address(base),
    configuration(Configuration::STARTUP)
{}

void USART::setConfiguration(const Configuration &config) {

    // Set the cached field to the configuration provided
    // This way, we don't have to fetch it from hardware every time
    // We still have to update the hardware though
    this->configuration = config;

    // Disable the USART temporarily
    // Nuke all the other configuration in there
    B_REG = 0;

    // Set up the A register
    // * U2Xn
    // * TXCn  = Reset
    // * MPCMn = No
    A_REG = (config.use_2X << 1) | (1 << 6) | (0 << 0);
    // Set up the C register
    // * UPMn
    // * USBSn
    // * UMSELn = Asynchronous
    // * UCSZn  = 8-bit
    C_REG = (config.parity << 4) | (config.stop_bits << 3) | (0 << 6) | (3 << 1);
    // Set up the baud rate
    BAUD_REG = config.baudrate_register;

    // Enable the USART
    // Set up the B register at the same time
    // * IEn   = Off
    // * UCSZn = 8-bit
    B_REG = (config.rx_en << 4) | (config.tx_en << 3) | (0 << 5) | (0 << 2);
}

Configuration USART::getConfiguration() const {
    return this->configuration;
}


#ifdef __AVR_ATmega328P__
    USART PORT[NUM_PORTS] = {
        USART{0xc0},
    };
#endif


}; // namespace USART
