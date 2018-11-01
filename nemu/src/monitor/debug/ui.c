#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);
WP* new_wp();
void free_wp(int wpid);
void info_watchpoint();

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_p(char *args){
	if(args==NULL){
		printf("please input expr again \n");
		return 0;	
	}	
	else{
		bool* success = malloc(4);	
		uint32_t ret=expr(args,success);
		printf("the expr value is %d (decimal)\n",ret);
		printf("the expr value is 0x%x (hex)\n",ret);	
		return 0;
	}
}

static int cmd_si(char *args) {
	if(args==NULL)
		printf("please input args\n");
	bool* success=malloc(1);	
	int num=expr(args,success);
	cpu_exec(num);
	return 0;
}

static int cmd_w(char *args){
	if(args==NULL){
		printf("please input expr again \n");
		return 0;	
	}	
	else{
		WP* wp=new_wp();
		strcpy(wp->expr,args);
		bool* success = malloc(4);
		wp->expr_record_val=expr(args,success);
		printf("watchpoint %d is set\n",wp->NO);
		return 0;
	}
}

static int cmd_d(char *args){
	if(args==NULL){
		printf("please input expr again \n");
		return 0;	
	}	
	else{
		int wpid=args[0]-'0';
		free_wp(wpid);
		printf("watchpoint %d is deleted\n",wpid);
		return 0;
	}
}

static int cmd_x(char *args){
	int i=0;
	int len=strlen(args);
	while(i<len){	
		if(args[i]==' ')
			break;
		i++;
	}
	if(i==len){
		printf("please input again\n");
		return 0;
	}
	char str1[10],str2[32];
	int j;	
	for(j=0;j<i;j++)
		str1[j]=args[j];
	str1[i]='\0';
	for(j=0;j<len-i-1;j++)
		str2[j]=args[i+1+j];
	str2[len]='\0';
	bool* success=malloc(1);	
	int num=expr(str1,success);
	int addr=expr(str2,success);
	printf("addr		value\n");
	for(i=0;i<num;i++)
		printf("0x%x		0x%x\n",addr+i*4,swaddr_read(addr+i*4,4));
	return 0;
}

static int cmd_info(char *args){
	if(args[0]=='w')
		info_watchpoint();
	else if(args[0]=='r'){
		printf("REG	VALUE\n");
		printf("eax	0x%x\n",reg_l(R_EAX));	
		printf("ecx	0x%x\n",reg_l(R_ECX));	
		printf("ebx	0x%x\n",reg_l(R_EDX));	
		printf("edx	0x%x\n",reg_l(R_EBX));	
		printf("esp	0x%x\n",reg_l(R_ESP));
		printf("ebp	0x%x\n",reg_l(R_EBP));	
		printf("esi	0x%x\n",reg_l(R_ESI));	
		printf("edi	0x%x\n",reg_l(R_EDI));	
		printf("eip	0x%x\n",cpu.eip);		
	}
	return 0;
}

static int cmd_help(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "exec by step n", cmd_si },
	{ "p", "expr calculate", cmd_p },
	{ "w", "set watchpoint", cmd_w },
	{ "d", "delete watchpoint", cmd_d },
	{ "info", "look up watchpoint/reg info", cmd_info },
	{ "x", "print memory", cmd_x },
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
