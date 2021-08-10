#include <serial.hpp>
#include <registers.hpp>


namespace USART {


void USART::flushRX() {
    // Only flush the receive buffer if we can receive
    if(this->getConfiguration().rx_en) {
        // Turn it off and on again
        B_REG &= ~(1 << 4);
        B_REG |=  (1 << 4);
    }
}


}; // namespace USART
