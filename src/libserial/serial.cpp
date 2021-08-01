#include "serial.hpp"


namespace Serial {


#ifdef __AVR_ATmega328P__
    const USART PORT[NUM_PORTS] = {
        USART{0xc0},
    };
#endif


}; // namespace Serial
