#include "cpu/exec/template-start.h"

#define instr test

static void do_execute () {
	DATA_TYPE temp = op_dest->val & op_src->val;
	cpu.eflags.sf=MSB(temp);	
	if(!temp)
		cpu.eflags.zf=1;
	else
		cpu.eflags.zf=0;
	temp&=0xff;
	int i;	
	DATA_TYPE re_temp=0x0;
	for(i=0;i<8;i++)
	{	
		re_temp^=(temp>>i);
	}
	cpu.eflags.pf=(re_temp&0x1);
	cpu.eflags.of=0;
	cpu.eflags.cf=0;

	print_asm_template2();
}

make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"