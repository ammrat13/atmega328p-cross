#include <serial.hpp>


namespace USART {


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


}; // namespace USART
