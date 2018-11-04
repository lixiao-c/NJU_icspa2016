#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute() {
	//OPERAND_W(op_dest, op_src->val);
	
	//int r_num=instr_fetch(cpu.eip,1)-0x50;
	if(DATA_BYTE==2)
	{
		swaddr_write(op_src->val,2,cpu.esp);
		cpu.esp=cpu.esp+2;
	}
	else{
		swaddr_write(op_src->val,4,cpu.esp);
		cpu.esp=cpu.esp+4;
	}
}

make_instr_helper(r);



#include "cpu/exec/template-end.h"