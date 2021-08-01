#pragma once

#include <stddef.h>
#include <stdint.h>


/**
 * Code for dealing with Serial IO
 *
 * It has classes to abstract away the MMIO needed for serial access, as well as
 * declarations for all the serial ports present on the platform.
 */
namespace Serial {


/**
 * USART interface on an AVR platform
 *
 * This class abstracts the MMIO interface for USARTs. It deals with hardware
 * registers internally, but provides a C++ abstraction.
 *
 * It should be instantiated statically, with one instance for each hardware
 * serial interface.
 *
 * Most Atmel chips have a "standard" interface for a USART, and this class
 * assumes it. The interface is, in order:
 *   - 1-byte Configuration Register A
 *   - 1-byte Configuration Register B
 *   - 1-byte Configuration Register C
 *   - 1-byte Reserved Space
 *   - 2-byte Baud-rate Registers (little-endian)
 *   - 1-byte Data Register
 *
 * The base address the constructor takes is that of Configuration Register A.
 * Use aggregate initialization to set it.
 */
class USART {

public:

    /**
     * Base address of the USART component
     *
     * The addresses of all the other registers are calculated by adding their
     * offset to this.
     */
    const uintptr_t base_address;

private:

    /** Offset to the A configuration register */
    static const size_t UCSRnA_OFFSET = 0x0;
    /** Offset to the B configuration register */
    static const size_t UCSRnB_OFFSET = 0x1;
    /** Offset to the C configuration register */
    static const size_t UCSRnC_OFFSET = 0x2;

    /** Offset to the baud-rate register */
    static const size_t UBRRn_OFFSET = 0x4;
    /** Offset to the data register */
    static const size_t UDRn_OFFSET = 0x6;

}; // class USART


/**
 * Number of hardware serial ports
 *
 * This number varies from platform to platform. The code uses the
 * `__AVR_Device__` macros to determine the correct value.
 */
#ifdef __DOXYGEN__
    constexpr size_t NUM_PORTS;
#elif __AVR_ATmega328P__
    constexpr size_t NUM_PORTS = 1;
#else
    #error LibSerial does not support this platform
#endif

/**
 * Array of hardware serial ports
 *
 * This array has `NUM_PORTS` entries, one for each hardware serial interface.
 * The contents change depending on the platform.
 *
 * @see NUM_SERIAL
 */
extern const USART PORT[NUM_PORTS];


}; // namespace Serial
