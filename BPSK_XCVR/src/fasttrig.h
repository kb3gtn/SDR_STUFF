#pragma once

#include "std_types.h"

// compute cos(x)/sin(x) and return as complex IQ pair.
// cos - I,  sin - Q
struct fc32_t fast_cos_sin(float rads);

