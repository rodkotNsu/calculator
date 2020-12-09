#define  _CRT_SECURE_NO_WARNINGS 
#include <crtdbg.h>
#include <stdio.h>
#include<malloc.h>
#include<string.h>
#include<math.h>
#define MAX_LEN_EXP 100 
#define ERROR_OUTPUT -9999

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
};
typedef struct SYMBOL_DATA SYMBOL_DATA;
SYMBOL_DATA** free_array_symbol(SYMBOL_DATA** arr);
SYMBOL_DATA** parsing_str(char* str) {
	unsigned int i = -1;
	unsigned int count = 0;
	unsigned int check_num = 0;
	int flag_error = 0;

	char* buffer = NULL;

	SYMBOL_DATA** arr_exp = malloc(sizeof(SYMBOL_DATA*) * MAX_LEN_EXP);
	SYMBOL_DATA* new_object = NULL;

	char alf_operation[] = "\n *-/+()";
	char alf_operand[] = ".0123456789";

	do {
		i++;
		if (strchr(alf_operation,str[i])!=NULL){
			if (check_num > 0) {
				//Exception: Float haven`t float part 
				if (buffer[check_num-1] == '.') {
					flag_error = 1;
					break;
				}
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

				buffer = NULL;
			}
		}
		else
		if (strchr(alf_operand, str[i]) != NULL) {
			if (check_num == 0) {
				//Exception: Float haven`t integer part 
				if (str[i] == '.') {
					flag_error = 1;
					break;
				}
				buffer = malloc(sizeof(char) * 10);

				buffer[0] = str[i];
				buffer[1] = '\0';

				check_num = 1;
			}
			else {
				//Exception: Float have more then 1 points 
				if (str[i]=='.' && buffer!=NULL && strchr(buffer, alf_operand[0]) != NULL) {
					flag_error = 1;
					break;
				}
				buffer[check_num] = str[i];
				buffer[check_num + 1] = '\0';

				check_num++;
			}
		}
			   else
		{
			//Exception: String have foreign symbol
			flag_error = 1;
			break; 
		}
	} while (str[i] != '\n' && flag_error==0);
	// exception handling 
	if (flag_error == 1) {
		printf("ERROR");
		new_object = malloc(sizeof(SYMBOL_DATA));
		new_object->type = END;
		new_object->data = NULL;
		if (buffer != NULL)
			free(buffer);
		arr_exp[count] = new_object;
	    free_array_symbol(arr_exp);
		return NULL;
	}
	 

	return arr_exp;
}
SYMBOL_DATA** free_array_symbol(SYMBOL_DATA** arr) {
	if (arr != NULL) {
		int i = 0;
		int flag = 0;
		while (flag == 0) {
			if ((*arr[i]).type == END)
				flag = 1;
			if (arr[i]->data != NULL)
				free(arr[i]->data);
			free(arr[i]);
			i++;

		}
		free(arr);
		arr = NULL;
		return arr;
	}
}
SYMBOL_DATA** rpn(SYMBOL_DATA** arr) {
	int i = 0;
	int situation_result = 0;
	int situation_stack = -1;
	SYMBOL_DATA** result = malloc(sizeof(SYMBOL_DATA*) * MAX_LEN_EXP);
	SYMBOL_DATA** stack = malloc(sizeof(SYMBOL_DATA*) * MAX_LEN_EXP);

	while (1)
	{
		switch (arr[i]->type)
		{
		case  OPERAND:
			{
				result[situation_result++] = arr[i];
			}
			break;
		case  OPERATION:
		{
			switch ((arr[i]->data)[0])
			{
			
			case '(': {
				stack[++situation_stack] = arr[i];
			}break;
			
			case ')':
			{
				int flag = 0;
				while (situation_stack >= 0)
				{
					if (stack[situation_stack]->data[0] == '(')
					{
						flag = 1;
						break;
					}
					result[situation_result++] = stack[situation_stack];
					situation_stack--;
				}
				if (flag == 1) {
					situation_stack--;
				}
				else {
					free(stack);
					free(result);
					return NULL;
				}
			}
			break;

			case '*':
			case '/': {
				while (situation_stack >= 0 && stack[situation_stack]->data[0] != '(') {

					if (stack[situation_stack]->data[0] == '*' || stack[situation_stack]->data[0] == '/') {
						result[situation_result++] = stack[situation_stack];
						situation_stack--;
					}
					else {
						break;
					}
				}
				stack[++situation_stack] = arr[i];
			}break;

			case '+':
			case '-': {
				while (situation_stack >= 0 && stack[situation_stack]->data[0] != '(') {
					result[situation_result++] = stack[situation_stack];
					situation_stack--;	
					}
				stack[++situation_stack] = arr[i];
			}break;
			}
		}break;
		case END: {
			while (situation_stack >= 0)
			{
				if (stack[situation_stack]->data[0] == '(')
				{
					free(result);
					free(stack);
					return NULL;
				}
				result[situation_result++] = stack[situation_stack];
				situation_stack--;
			}
			result[situation_result++] = arr[i];
			free(stack);
			return result;
		}break;
		}
		i++;
	} while (1);
}
void print_arr(SYMBOL_DATA** arr) {
	
	if (arr != NULL) {
		int i = 0;
		while ((*arr[i]).type!=END)
		{
			printf("%s ", arr[i]->data);
			i++;
		}
	}
}
double parsing_str_to_float(char* str) {
	double result = 0;
	double pow_10 = 1;
	char* point = strchr(str, '.');
	if (point != NULL)
		pow_10 = pow(10, (double)(point - &str[0]-1));
	else 
		pow_10 = pow(10, (double)(strlen(str) - 1));
	int i = 0;
	while (str[i] != '\0') {
		if (str[i] != '.')
		{
			result += (str[i] - '0') * pow_10;
			pow_10 /=10.0;
		}
		i++;
	}
	return result;
}
double postfix_to_result(SYMBOL_DATA** arr) {
	double* stack = malloc(sizeof(double) * 100);
	int situation_stack = -1;
	int flag_error = 0;
	int i = 0;
	while(arr[i]->type != END){
		if (arr[i]->type == OPERAND)
		{
			situation_stack++;
			stack[situation_stack] = parsing_str_to_float(arr[i]->data);
		}
		else if (arr[i]->type == OPERATION) {
			if (situation_stack < 1)
			{
				flag_error = 1;
				break;
			}
			else {
				switch (arr[i]->data[0])
				{
				case '*':
					stack[situation_stack - 1] *= stack[situation_stack];
					break;
				case '/':
					if (stack[situation_stack - 1] == 0) {
						flag_error = 1;
						break;
					}
					else {
						stack[situation_stack - 1] /= stack[situation_stack];
					}
					break;
				case '-':
					stack[situation_stack - 1] -= stack[situation_stack];
					break;
				case '+':
					stack[situation_stack - 1] += stack[situation_stack];
					break;
				}
				situation_stack--;
			}
	    }
		i++;
	}
	if (flag_error == 0 && situation_stack == 0) {
	 
			double result = stack[situation_stack];
			free(stack);
			return  result;
		}
	free(stack);
	return ERROR_OUTPUT;


}
int main() {
	char* str_expression = (char*)malloc(sizeof(char) * MAX_LEN_EXP);
	if (fgets(str_expression, MAX_LEN_EXP, stdin)) {
	}
	SYMBOL_DATA** symbol_arr = parsing_str(str_expression);
	/* TODO:
	Релизовать input error 
	*/
	if (symbol_arr != NULL) {
		SYMBOL_DATA** symbol_arr_rpn = rpn(symbol_arr);
		if (symbol_arr_rpn != NULL) {
			 
			double result = postfix_to_result(symbol_arr_rpn);
		}
	}
	 
	free_array_symbol(symbol_arr);
 
	free(str_expression);

	_CrtDumpMemoryLeaks();


	return 0;
}