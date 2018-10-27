#include "monitor/watchpoint.h"
#include "monitor/expr.h"
#include <stdlib.h>

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp()
{
	if(free_==NULL)
		assert(0);
	WP *temp=free_;
	free_=free_->next;
	temp->next=NULL;
	return temp;
}

void free_wp(int wpid)
{
	WP* wp=&wp_pool[wpid];
	if(head==wp)
		head=NULL;
	else{	
		WP *temp=head;
		while(temp->next!=wp)
		{
			temp=temp->next;
		}
		temp->next=wp->next;
		wp->next=NULL;
	}
	WP *free_temp=free_;
	while(free_temp->next!=NULL)
	{
		free_temp=free_temp->next;
	}
	free_temp->next=wp;
}

bool check_watchpoint()
{
	WP *wp=head;
	bool* success_flag=malloc(1);
	bool ret=true;	
	while(wp!=NULL)
	{
		uint32_t new_value=expr(wp->expr,success_flag);
		if(new_value!=wp->expr_record_val){
			printf("\nHit watchpoint %d  old value = 0x%x, new value = 0x%x .\n"
				,wp->NO,wp->expr_record_val,new_value);
			ret=false;
			goto end;			
		}
		wp=wp->next;
	}
end:	
	free(success_flag);
	return ret;
}

