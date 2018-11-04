#include "cpu/exec/helper.h"

make_helper(ret)
{
	cpu.eip=swaddr_read(cpu.esp,4);
	cpu.eip+=4;
	cpu.esp+=4;
	return 1;
}