#include "cpu/exec/template-start.h"

#define instr je

static void do_execute() {
	DATA_TYPE tail=op_src->val;
	uint32_t temp=cpu.eip+tail;
	if(DATA_BYTE==2)
		temp&=0xffff;
	if(cpu.eflags.zf==1)
		cpu.eip=temp;
}
make_instr_helper(i)

#undef instr

#include "cpu/exec/template-end.h"