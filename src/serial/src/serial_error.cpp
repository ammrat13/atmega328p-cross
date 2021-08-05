#include <serial.hpp>


namespace USART {


Error operator&(const Error lhs, const Error rhs) {
    return static_cast<Error>(
        static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs)
    );
}

Error& operator&=(Error &lhs, const Error rhs) {
    return lhs = lhs & rhs;
}

Error operator|(const Error lhs, const Error rhs) {
    return static_cast<Error>(
        static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs)
    );
}

Error& operator|=(Error &lhs, const Error rhs) {
    return lhs = lhs | rhs;
}


}; // namespace USART
