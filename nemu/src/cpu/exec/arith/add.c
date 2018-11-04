#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "add-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "add-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(add_i2rm)