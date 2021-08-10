#include <serial.hpp>
#include <registers.hpp>


namespace USART {


void USART::flushRX() {
    // Only flush the receive buffer if we can receive
    if(this->configuration.rx_en) {
        // Turn it off and on again
        // Don't use bitwise in-place
        uint8_t original = B_REG;
        B_REG = original & ~(1 << 4);
        B_REG = original;
    }
}


}; // namespace USART
