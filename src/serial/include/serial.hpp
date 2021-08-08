#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


/**
 * Code for dealing with USART Serial IO
 *
 * It has classes to abstract away the MMIO needed for serial access, as well as
 * declarations for all the serial ports present on the platform.
 */
namespace USART {


/**
 * Encapsulates a USART's configuration
 *
 * Before use, a USART must be configured. The user must specify things like
 * whether to send or recieve, whether and which parity to use, and the
 * baud-rate with 2X if needed. This library does not allow the user to
 * configure the number of bits to work with - it's always 8-bit.
 */
struct Settings {

    /** Baud-rate register value */
    // This must be specified first since C++ struct fields can't be reordered
    unsigned baudrate_register : 12;

    /** Whether to use 2X mode for the baud-rate */
    bool use_2X : 1;

    /** Whether to enable the reciever */
    bool rx_en : 1;
    /** Whether to enable the transmitter */
    bool tx_en : 1;

    /** Possible settings for the number of stop bits to use */
    enum StopBits : bool {
        ONE = 0,
        TWO = 1,
    };
    /** Whether to use one (`false`) or two (`true`) stop bits */
    StopBits stop_bits : 1;

    /** Possible settings for the parity bits to use */
    enum Parity : uint8_t {
        NONE = 0, /** No parity */
        EVEN = 2, /** Even parity */
        ODD = 3,  /** Odd parity */
    };
    /** Setting for parity */
    Parity parity : 2;
}; // struct Settings

/**
 * Structure to report errors USART operations
 *
 * Serial transmission can fail. For instance, there could be a parity error or
 * a data overrun. This structure serves to report on those errors.
 *
 * If no error is present, there's a special value for that: `NONE`. It's set
 * equal to zero. Other than that, there are other flags for an error - they can
 * be bitwise-ORed together.
 */
enum Error : uint8_t {
    NONE = 0,    /** Indicates success */
    PARITY = 1,  /** Parity error from `UPEn` */
    OVERRUN = 2, /** Data overrun from `DORn` */
    FRAME = 4,   /** Frame error from `FEn` */
}; // enum Error
/** Bitwise AND for the error type */
Error operator&(const Error lhs, const Error rhs);
/** Bitwise AND assignment for the error type */
Error& operator&=(Error &lhs, const Error rhs);
/** Bitwise OR for the error type */
Error operator|(const Error lhs, const Error rhs);
/** Bitwise OR assignment for the error type */
Error& operator|=(Error &lhs, const Error rhs);

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

    /**
     * Configure the USART based on the settings given
     *
     * This will modify hardware registers to bring them in line with the
     * configuration given. It will also enable the USART if that option was
     * given. This must be done before using the USART, otherwise the default
     * state of the registers will be used.
     *
     * @param [in] settings How to configure the serial port
     * @see Settings
     * @see getConfiguration
     */
    void setConfiguration(const Settings &settings);
    /**
     * Retrieve the current configuration of the USART
     *
     * This will read the hardware registers set by `setConfiguration` and place
     * them into a `Settings` object. The state of the registers will not be
     * changed.
     *
     * @return How the port is configured
     * @see Settings
     * @see setConfiguration
     */
    Settings getConfiguration();
    /**
     * Get the error flags currently set in the USART
     *
     * Read from the hardware registers to get this information. It will not
     * modify the registers, just read them. Note however that reading from the
     * USART will destroy these error flags.
     *
     * @return The current error flags set
     */
    Error getError() const;

    /**
     * Put a character onto the USART
     *
     * This function cannot report an error. The error flags the hardware sets
     * are specifically for recieving. Thus, this function returns `void`
     * instead of `Error`.
     *
     * *Note:* Using this function with the reciever disabled leads to undefined
     * behavior, usually a hang.
     *
     * @param [in] c The character to put
     */
    void putc(uint8_t c);
    /**
     * Put `len` bytes from `buf` onto the USART
     * @param [in] buf The bytes to put
     * @param [in] len How many bytes to put
     * @see putc
     */
    void putn(const uint8_t *buf, size_t len);
    /**
     * Put a null-terminated string onto the USART
     * @param [in] buf The string to put
     * @see putc
     */
    void puts(const char *buf);

    /**
     * Get a character from the USART
     *
     * Unlike `putc`, this function can report an error on the data read. It
     * also destroys the error flags in the process, so be sure to save them.
     *
     * *Note:* Using this function with the transmitter disabled leads to
     * undefined behavior, usually a hang.
     *
     * @param [out] c The character received
     * @return Any errors, or `NO_ERROR` if there weren't any
     * @see Error
     */
    Error getc(uint8_t &c);
    /**
     * Get up to `len` characters from the USART
     *
     * It'll read until the first error, at which point it returns. The number
     * of bytes read is returned as well.
     *
     * @param [in] buf The buffer to read into
     * @param [in] len How many bytes to try to read
     * @param [out] read How many bytes were actually read
     */
    Error getn(uint8_t *buf, size_t len, size_t &read);
    /**
     * Read up to `len` characters from the USART until the character `until`
     *
     * It'll read until it either encounters an error or until it hits the
     * character specified by `until`. The number of bytes read is put in
     * `read`. Note that the terminating `until` character is included in this
     * count and in the final string.
     *
     * The parameter `len` indicates the length of the buffer, including the
     * null terminator. A null terminator is automatically placed after `read`
     * regardless of the reason of termination. Thus, `read` will be at most
     * `len-1`.
     *
     * @param [in] buf The buffer to read into
     * @param [in] len How many bytes to try to read
     * @param [out] read How many bytes were actually read
     * @param [in] until The character to read until
     */
    Error gets(char *buf, size_t len, size_t &read, char until);


private:

    /** Offset to the A configuration register */
    static constexpr size_t UCSRnA_OFFSET = 0x0;
    /** Offset to the B configuration register */
    static constexpr size_t UCSRnB_OFFSET = 0x1;
    /** Offset to the C configuration register */
    static constexpr size_t UCSRnC_OFFSET = 0x2;

    /** Offset to the baud-rate register */
    static constexpr size_t UBRRn_OFFSET = 0x4;
    /** Offset to the data register */
    static constexpr size_t UDRn_OFFSET = 0x6;

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
extern USART PORT[NUM_PORTS];


}; // namespace USART
