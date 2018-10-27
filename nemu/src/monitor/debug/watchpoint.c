#include "monitor/watchpoint.h"
#include "monitor/expr.h"

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

void free_wp(WP *wp)
{
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

