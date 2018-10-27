#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, NUM

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */
	{"[0-9]+",NUM},	
	{"\\)",')'},
	{"\\(",'('},	
	{"/",'/'},	
	{"\\*",'*'},	
	{"-", '-'},
	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"==", EQ}						// equal
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */

				switch(rules[i].token_type) {
					case '+':
					case '-':
					case '*':
					case '/':
					case '(':
					case ')':
					case EQ :
						{
						tokens[nr_token].type=rules[i].token_type;
						nr_token++;
						if(nr_token>=32)
							assert(0);
						break;
						}
					case NOTYPE:
						{
						break;
						}
					case NUM:
						{
						tokens[nr_token].type=rules[i].token_type;
						int ini;
						if(substr_len>31)
							assert(0);					
						for(ini=0;ini<substr_len;ini++)						
						{
							tokens[nr_token].str[ini]=e[position-substr_len+ini];
						}
						tokens[nr_token].str[substr_len]='\0';
						nr_token++;
						if(nr_token>=32)
							assert(0);
						break;
						}				
					default: 
						break;
				}

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

int get_token_num_value(char* str){
	int i=0;	
	int ret=0;
	while(str[i]!='\0')
	{	
		ret=ret*10;		
		ret+=str[i]-'0';
		i++;
	}
	return ret;
}

bool check_parentheses(int start,int end)
{
	if(tokens[start].type=='(' && tokens[end].type==')')
		return true;
	return false;
}

int eval(int start,int end)
{
	if(start>end){
		assert(0);
	}
	else if(start==end){
		return 	get_token_num_value(tokens[start].str);
	}
	else if(check_parentheses(start,end)){
		return eval(start+1,end-1);
	}
	else{
		int op=-1;
		int list_num=0;//'(' & ')' num
		int i;
		for(i=start;i<=end;i++)
		{
			if(tokens[i].type=='(')
				list_num++;
			else if(tokens[i].type==')')
				list_num--;
			else if(tokens[i].type=='*' || tokens[i].type=='/'){
				if(op<0 && list_num==0)
					op=i;			
			}
			else if(tokens[i].type=='+' || tokens[i].type=='-'){
				if(list_num==0){
					if(op<0)
						op=i;
					else if(tokens[op].type=='*' || tokens[op].type=='/')
						op=i;				
				}			
			}
		}
		int val1=eval(start,op-1);
		int val2=eval(op+1,end);
		switch(tokens[op].type){
			case '+':
				return val1+val2;
			case '-':
				return val1-val2;
			case '*':
				return val1*val2;
			case '/':
				return val1/val2;	
			default:
				assert(0);	
		}	
	}
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
	printf("the expr value is %d\n",eval(0,nr_token-1));
	//printf("num value%d \n",get_token_num_value("123"));
	return 0;
}

