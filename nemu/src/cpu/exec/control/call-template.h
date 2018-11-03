#include "cpu/exec/template-start.h"

#define instr call

static void do_execute() {
	if(instr_fetch(cpu.eip,1)==0xe8){
		cpu.eip+=op_src->val;
		print_asm("call $0x%x",cpu.eip);
	}
	else{
		cpu.eip=op_src->val;
		print_asm("call $0x%x",cpu.eip);
	}
}
make_instr_helper(i)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"