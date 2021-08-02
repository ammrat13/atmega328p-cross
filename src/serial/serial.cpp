#include "serial.hpp"


namespace USART {


#ifdef __AVR_ATmega328P__
    USART PORT[NUM_PORTS] = {
        USART{0xc0},
    };
#endif


void USART::configure(Settings const &settings) {

    // Get all the registers
    volatile uint16_t * const baud_ptr = (volatile uint16_t *) (this->base_address + UBRRn_OFFSET);
    volatile uint8_t * const a_ptr = (volatile uint8_t *) (this->base_address + UCSRnA_OFFSET);
    volatile uint8_t * const b_ptr = (volatile uint8_t *) (this->base_address + UCSRnB_OFFSET);
    volatile uint8_t * const c_ptr = (volatile uint8_t *) (this->base_address + UCSRnC_OFFSET);

    // Disable USART (temporarily)
    // Disable interrupts
    // Set the high bits for 8-bit mode
    *b_ptr = (0 << 3) | (0 << 5) | (0 << 2);

    // Clear the transmit complete flag by writing one to it
    // Disable multi-processor communication
    // Set 2X if needed
    *a_ptr = (1 << 6) | (0 << 0) | ((settings.baudrate & (1 << 15)) >> 14);

    // Set the mode to asynchronous
    // Set the data size to 8-bit
    // Set the parity and stop bits
    *c_ptr = (0 << 6) | (3 << 1) | ((settings.flags & (7 << 0)) << 3);

    // Set the baud-rate register
    *baud_ptr = settings.baudrate & ~(1 << 15);

    // Finally, enable the reciever and transmitter as needed
    // Use the same settings from before
    *b_ptr = (0 << 5) | (0 << 2) | ((settings.flags & (3 << 3)) << 0);
}


Error USART::putc(uint8_t c){

    // Get all the registers
    volatile uint8_t * const data_ptr = (volatile uint8_t *) (this->base_address + UDRn_OFFSET);
    volatile uint8_t * const a_ptr = (volatile uint8_t *) (this->base_address + UCSRnA_OFFSET);

    // Wait for the transmit buffer to have space for new data
    do {} while((*a_ptr & (1 << 5)) == 0);

    // Write the data
    *data_ptr = c;

    // Wait for it to shift out
    do {} while((*a_ptr & (1 << 6)) == 0);

    // Check for errors and return
    return Error::NO_ERROR;
}

Error USART::getc(uint8_t &c) {
    (void)(c);
    return Error::NO_ERROR;
}


}; // namespace Serial
