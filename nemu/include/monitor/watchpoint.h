#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
	
	/* TODO: Add more members if necessary */
	char expr[32];
	uint32_t expr_record_val;

} WP;

#endif
