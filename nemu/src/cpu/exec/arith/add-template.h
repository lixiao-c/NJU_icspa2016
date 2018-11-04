#include "cpu/exec/template-start.h"

#define instr add

static void do_execute () {
	DATA_TYPE result = op_src->val+op_dest->val;
	OPERAND_W(op_dest, result);

	print_asm_template2();
}

make_instr_helper(i2rm)

#include "cpu/exec/template-end.h"
