#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
	//OPERAND_W(op_dest, op_src->val);
	
	//int r_num=instr_fetch(cpu.eip,1)-0x50;
	if(DATA_BYTE==2)
	{
		cpu.esp=cpu.esp-2;
		swaddr_write(cpu.esp,2,op_src->val);
	}
	else{
		cpu.esp=cpu.esp-4;
		swaddr_write(cpu.esp,4,op_src->val);
	}
}

make_instr_helper(r);



#include "cpu/exec/template-end.h"