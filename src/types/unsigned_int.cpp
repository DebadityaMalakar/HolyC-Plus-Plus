#include "unsigned_int.hpp"

namespace holycpp {

// Explicit template instantiations
template class UInt<8>;
template class UInt<16>;
template class UInt<32>;
template class UInt<64>;

// Global constants
const U8  U8_MIN = 0;
const U8  U8_MAX = UINT8_MAX;
const U16 U16_MIN = 0;
const U16 U16_MAX = UINT16_MAX;
const U32 U32_MIN = 0;
const U32 U32_MAX = UINT32_MAX;
const U64 U64_MIN = 0;
const U64 U64_MAX = UINT64_MAX;

} // namespace holycpp