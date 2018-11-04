#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute () {
	DATA_TYPE temp = op_src->val-op_dest->val;
	long long result1= ((long long)op_src->val-(long long)op_dest->val)>>(8*DATA_BYTE);
	cpu.eflags.cf=result1&1;
	
	if((MSB(op_dest->val)!=MSB(op_src->val))&&(MSB(op_dest->val)!=MSB(temp)))
		cpu.eflags.of=1;
	else
		cpu.eflags.of=0;
	
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
}

#if DATA_BYTE==2 || DATA_BYTE==4
make_instr_helper(si2rm)
#endif

#include "cpu/exec/template-end.h"
