#include "serial.hpp"


namespace USART {


#ifdef __AVR_ATmega328P__
    USART PORT[NUM_PORTS] = {
        USART{0xc0},
    };
#endif


void USART::configure(Settings const &settings) {

    // Get all the registers
    volatile uint16_t * const baud_ptr = reinterpret_cast<volatile uint16_t *>(this->base_address + UBRRn_OFFSET);
    volatile uint8_t * const a_ptr = reinterpret_cast<volatile uint8_t *>(this->base_address + UCSRnA_OFFSET);
    volatile uint8_t * const b_ptr = reinterpret_cast<volatile uint8_t *>(this->base_address + UCSRnB_OFFSET);
    volatile uint8_t * const c_ptr = reinterpret_cast<volatile uint8_t *>(this->base_address + UCSRnC_OFFSET);

    // Disable USART (temporarily)
    // Disable interrupts
    // Set the high bits for 8-bit mode
    *b_ptr = (0 << 3) | (0 << 5) | (0 << 2);

    // Clear the transmit complete flag by writing one to it
    // Disable multi-processor communication
    // Set 2X if needed
    *a_ptr = (1 << 6) | (0 << 0) | (settings.use_2X << 1);

    // Set the mode to asynchronous
    // Set the data size to 8-bit
    // Set the parity and stop bits
    *c_ptr = (0 << 6) | (3 << 1) | (settings.parity << 4) | (settings.stop_bits << 3);

    // Set the baud-rate register
    *baud_ptr = settings.baudrate_register;

    // Finally, enable the reciever and transmitter as needed
    // Use the same settings from before
    *b_ptr = (0 << 5) | (0 << 2) | (settings.rx_en << 4) | (settings.tx_en << 3);
}


Error USART::putc(uint8_t c){

    // Get all the registers
    volatile uint8_t * const data_ptr = reinterpret_cast<volatile uint8_t *>(this->base_address + UDRn_OFFSET);
    volatile uint8_t * const a_ptr = reinterpret_cast<volatile uint8_t *>(this->base_address + UCSRnA_OFFSET);

    // Wait for the transmit buffer to have space for new data
    do {} while((*a_ptr & (1 << 5)) == 0);

    // Write the data and return
    *data_ptr = c;
    return Error::NONE;
}

Error USART::getc(uint8_t &c) {

    // Get all the registers
    volatile uint8_t * const data_ptr = reinterpret_cast<volatile uint8_t *>(this->base_address + UDRn_OFFSET);
    volatile uint8_t * const a_ptr = reinterpret_cast<volatile uint8_t *>(this->base_address + UCSRnA_OFFSET);

    // Wait for there to be data
    do {} while((*a_ptr & (1 << 7)) == 0);

    // Read the data and return
    // We have to read the return value first since it goes invalid when we read
    //  the data
    Error ret = static_cast<Error>((*a_ptr >> 2) & 7);
    c = *data_ptr;
    return ret;
}


}; // namespace Serial
