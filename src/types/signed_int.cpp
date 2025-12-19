#include "signed_int.hpp"

namespace holycpp {

// Explicit template instantiations
template class SInt<8>;
template class SInt<16>;
template class SInt<32>;
template class SInt<64>;

// Global constants
const I8  I8_MIN = I8(INT8_MIN);
const I8  I8_MAX = I8(INT8_MAX);
const I16 I16_MIN = I16(INT16_MIN);
const I16 I16_MAX = I16(INT16_MAX);
const I32 I32_MIN = I32(INT32_MIN);
const I32 I32_MAX = I32(INT32_MAX);
const I64 I64_MIN = I64(INT64_MIN);
const I64 I64_MAX = I64(INT64_MAX);

} // namespace holycpp