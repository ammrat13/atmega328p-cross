// Macros to help with manipulating serial hardware registers
// These should only be used inside class methods for `USART::USART` since they
//  reference `this`
// They are also `#defines`, which are slightly dangerous. But, it makes the
//  code nicer.

#pragma once

#include <stdint.h>


#define BAUD_REG \
    (*reinterpret_cast<volatile uint16_t *>(this->base_address + UBRRn_OFFSET))

#define DATA_REG \
    (*reinterpret_cast<volatile uint8_t *>(this->base_address + UDRn_OFFSET))

#define A_REG \
    (*reinterpret_cast<volatile uint8_t *>(this->base_address + UCSRnA_OFFSET))
#define B_REG \
    (*reinterpret_cast<volatile uint8_t *>(this->base_address + UCSRnB_OFFSET))
#define C_REG \
    (*reinterpret_cast<volatile uint8_t *>(this->base_address + UCSRnC_OFFSET))
