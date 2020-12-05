#define  _CRT_SECURE_NO_WARNINGS 
#include <crtdbg.h>
#include <stdio.h>
#include<malloc.h>
#define MAX_LEN_EXP 100 
 
enum SYNTAX_TYPE
{
	OPERATION,
	OPERAND,
	END
};
struct SYMBOL_DATA
{
	char* data;
	enum SYNTAX_TYPE type;
} ;
typedef struct SYMBOL_DATA SYMBOL_DATA;
SYMBOL_DATA** free_array_symbol(SYMBOL_DATA** arr);
 SYMBOL_DATA ** parsing_str(char * str ) {
	 unsigned int i = 0;
	 unsigned int count = 0;
	 unsigned int check_num = 0;
	 
	 char* buffer=NULL;

	 SYMBOL_DATA** arr_exp = malloc(sizeof(SYMBOL_DATA*) * 100);
	 SYMBOL_DATA* new_object = NULL;
	do {
		switch (str[i]) {
		case '\n':case' ':case'd':case'*':case'-':
		case'/':case'+':case')':case'(': {
			if (check_num > 0) {
				new_object = malloc(sizeof(SYMBOL_DATA));

				(*new_object).type = OPERAND;
				new_object->data = buffer;

				arr_exp[count++] = new_object;
				check_num = 0;
			}
			if (str[i] != ' ') {
				new_object = malloc(sizeof(SYMBOL_DATA));

				buffer = (char*)malloc(sizeof(str) * 10);
				buffer[0] = str[i];
				buffer[1] = '\0';
				if (str[i] == '\n')
					new_object->type = END;
				else
					new_object->type = OPERATION;
				new_object->data = buffer;

				arr_exp[count++] = new_object;
			}
		}
			   break;
		case'0':case'1':case'2':case'3':
		case'4':case'5':case'6':case'7':
		case'8':case'9':{
			if (check_num == 0) {
				buffer = malloc(sizeof(char) * 10);

				buffer[0]= str[i];
				buffer[1] = '\0';

				check_num = 1;
			}
			else {
				buffer[check_num] = str[i];
				buffer[check_num + 1] = str[i];

				check_num++;
			}
		}
			 break;	 
		 default:
			 printf("ERROR");
			 new_object = malloc(sizeof(SYMBOL_DATA));

			 (*new_object).type = END;
			 new_object->data = NULL;
			 if (buffer != NULL)
				 free(buffer);
			 arr_exp[count] = new_object;
			 return free_array_symbol(arr_exp);
			 break;
		 }

		 i++;
	} while (str[i-1] != '\n');
	 return arr_exp;
}
 SYMBOL_DATA** free_array_symbol(SYMBOL_DATA** arr) {
	 if (arr != NULL) {
		 int i = 0;
		 int flag = 0;
		 while (flag==0) {
			 if ((*arr[i]).type == END)
				 flag = 1;
			 if (arr[i]->data!=NULL)
			 free(arr[i]->data);
			 free(arr[i]);
			 i++;		 
			 
		 } 
		 free(arr);
		 arr = NULL;
		 return arr;
	 }
 }
 SYMBOL_DATA** polskaya(SYMBOL_DATA** arr) {
	 int i = 0;
	 int uk = 0;
	 int num_brack = 0;
	 int situation_stack = 0;
	 SYMBOL_DATA** result = malloc(sizeof(SYMBOL_DATA*) * 100);
	 SYMBOL_DATA** stack  = malloc(sizeof(SYMBOL_DATA*) * 100);
	 while (arr[i]->type!=END)
	 {
		 if (arr[i]->type == OPERAND)
		 {
			 result[uk] = arr[i];
			 uk++;
		 
		 }
		 else
		 {
			 switch ((arr[i]->data)[0])
			 {
			 case '(': {
				 stack[situation_stack++] = arr[i];
				 num_brack++;
			 }
				 break;
			 case ')':
			 {
				 num_brack--;
				 free(arr[situation_stack]->data);
				 free(arr[situation_stack]);

				 situation_stack--;
				 while (stack[situation_stack]->data[0]!='(' && situation_stack >=0)
				 {
					 result[uk++] = arr[situation_stack];
					 situation_stack--;
				 }
				 if (situation_stack >= 0) {
					 free(arr[situation_stack]->data);
					 free(arr[situation_stack]);
				 }
				 else {
				     /*TODO ERROR INPUT*/
					 printf("ERROR");
				 }
			 }
				 break;

			 case '*':
			 case '/':{
				 stack[situation_stack++] = arr[i];
			 
			 }break;
			 case '+':
			 case '-': {
				 if (situation_stack == 0) {
					 situation_stack--;
				 }else
				 while (situation_stack >= 0 && stack[situation_stack-1]->data[0]!='(') {
					 if (stack[situation_stack]->data[0] == '*' || stack[situation_stack]->data[0] == '/') {
						 result[uk++] = arr[situation_stack];
						 situation_stack--;
					 }
					
				 }
				 situation_stack++;
				 stack[situation_stack++] = arr[i];

			 }break;
			 default:
				 break;
			 }
		 }
		 i++;

	 }


 }
int main() {
	char* str_expression = (char*)malloc(sizeof(char) * MAX_LEN_EXP);
	if (fgets(str_expression, MAX_LEN_EXP, stdin)) {
	}
    SYMBOL_DATA** str_exp = parsing_str(str_expression);
	polskaya(str_exp);
 free_array_symbol(str_exp);
   
 free(str_expression);
   _CrtDumpMemoryLeaks();


   return 0;
}