
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>

union qvalue{
	int output;
	char* input;
};
/*tho i don't think we need an union to form a queue, but for the sake of practising i created
 * an union for the queue.
 */

struct queue{
	int counter, front;
	union qvalue queuearray[100];
/*i create 100 unions, which could contain either 100 int or 100 string.
i have make it homogeneous*/
};

struct stack{
	int top;
	char operator[100];
};

void enqueue (union qvalue in, struct queue* current);
union qvalue dequeue(struct queue* current);
int push(char elem, struct stack* s);
char pop(struct stack* s);
void inputq(struct queue* in_q, char** in, int argc);
int operator(char* str);
void shuntingyard(struct queue* in_q, struct queue* leftq, struct stack* opera, int argc);
int get_precedence(char op);
int has_higher_precedence(char op, char* string);
char looktop(struct stack* s);
void print(struct queue* everything, int argc);

int main(int argc, char **argv) {//argv[0][0]
	struct queue holdin;
		holdin.counter=0;
		holdin.front=0;
	struct queue holdout;
		holdout.counter=0;
		holdout.front=0;
	struct stack opp;
		opp.top=0;
	inputq(&holdin, argv, argc);
	shuntingyard(&holdin, &holdout, &opp, argc);
	printf("In2post: ");
	for(int i = 1; i < argc; i++){
	printf("%s ", argv[i]);
	}
	printf("\n");
	printf("postfix: ");
	print(&holdout, argc);
/*union qvalue x1;
x1.input = "than k u";
enqueue(x1, &holdin);
x1.input = "hello world";
enqueue(x1,&holdin);
enqueue(5, &holdin);
enqueue(3, &holdin);
union qvalue a = dequeue(&holdin);
printf("%s\n", a.input);
a = dequeue(&holdin);
printf("%s\n", a.input);*/
/*
int b = dequeue(&holdin);
printf("%d\n", b);
int c = dequeue(&holdin);
printf("%d\n", c);
int d = dequeue(&holdin);
printf("%d\n", d);
int e = dequeue(&holdin);
printf("%d\n", e);
*/
/*test for queue*/
/*struct stack teststack;
teststack.top=0;
push('*',&teststack);
push('-',&teststack);
char a = pop(&teststack);
printf("%c\n", a);
char b = pop(&teststack);

printf("%c\n", b);
test for stack*/

	return EXIT_SUCCESS;
}

void enqueue (union qvalue in, struct queue* current){
	if(current->counter < 100){
		//printf("The counter is %d. The front is %d\n",current->counter,current->front);
		current->queuearray[current->front+current->counter] = in;
		//printf("Inserted %d.\n",in);

		current->counter++;
		//printf("ENQ counter %d, front %d [%s]\n\n",current->counter,current->front,in.input);
	}
}
/* inspired from slides lecture 11.12 page 23*/

union qvalue dequeue(struct queue* current){
	if (current->counter > 0){
		//printf("DE: The counter is %d. The front is %d\n",current->counter,current->front);
		current->counter--;
		//printf("DEQ counter %d, front %d [%s]\n\n",current->counter,current->front,current->queuearray [current->front].input);
		return current->queuearray [current->front++];
		/* we wanna increase front for 1 after return but the datatype union has to end with return,
		 * so i put the ++ inside the square bracket which is a smart way to deal the problem
		 */
	}
	else
	{
		union qvalue error;
		error.output = 0;
		return error;
	}

}
/* inspired from slides lecture 11.12 page 23*/

int push(char elem, struct stack* s){
	if (s->top == 99)
		return 0;
	/*99 is the limitation of my stack, when it stores up to 99, no more int can be stored so
	 * just return 0 as a reminder that the stack is full
	 */
	else{
		s->operator[s->top] = elem;
		s->top++;
		//printf("PSH top %d [%c]\n\n",s->top,elem);
		return 1;
	}
}
/*moreover, i return 0 or 1 is like boolean return. Thus later then i can use shortcut like
 * has_higher_precedence(looktop(opera)) which is directly equally saying of "has_higher_precedence(looktop(opera) = 1
 * then it's true.
 */
char pop(struct stack* s){
	if (s->top == -1){
		return '\0';
	}
	else {
		//printf("POP top %d [%c]\n\n",s->top - 1, s->operator[s->top - 1]);
		return s->operator[--s->top];
	}
}
/* "if" is not a loop*/

void inputq(struct queue* in_q, char** in, int argc){
	/*rn i m actually transferring argv to input queue, which queue is a union and i told
	 it to store as char**    */
	//seems like pointer to pointer can pass value outside
	union qvalue x;
	for(int i = 1; i < argc; i++){
		x.input = in[i];
		//strcpy(x.input,in[i]);
		enqueue(x,in_q);
	}
}

int operator(char* str){
	// how to distinguish operator and operand
	/*switch(str[0]){
	case '+':
	case '-':
	case '*':
	case '/':
		return 1;
	default:
		return 0;
	}*/
	if (str[0] == '+'||str[0] == '-'||str[0] == 'x'||str[0] == '/')
		return 1;
	else
		return 0;

}

int get_precedence(char op){
	if (op == 'x' || op == '/' )
		return 2;
	else
		return 1;
}

int has_higher_precedence(char op, char* string){
	int topprecedence = get_precedence(op);
	int rightprecedence = get_precedence(string[0]);
	if (topprecedence >= rightprecedence)
		return 1;
	else
		return 0;
}

char looktop(struct stack* s){
	if (s->top == -1){
		return '\0';
	}
	else {
		//printf("TOP top %d [%c]\n\n",s->top - 1, s->operator[s->top - 1]);
		return s->operator[s->top - 1];
	}
}



void shuntingyard(struct queue* in_q, struct queue* leftq, struct stack* opera, int argc){
	union qvalue y;
	union qvalue z;
	char* ptr;
	for(int i = 1; i < argc; i++){
		y = dequeue(in_q);
		if(operator(y.input))//by default: operator(y.input) == 1//
		{
			while(looktop(opera) != '\0' && has_higher_precedence(looktop(opera), y.input)){
				//printf("* Not empty and has higher precedence.\n");
					ptr = (char*) malloc(2);
					ptr[0] = looktop(opera);
					ptr[1] = '\0';
					z.input = ptr;
					/*z.input[0] = looktop(opera);
					z.input[1] = '\0';
					this is for i define the size if char* in union at header*/
				enqueue(z, leftq);
		//printf("Not dead yet.\n");
				pop(opera);
		//printf("Not dead yet.\n");
			}
			push(y.input[0], opera);
		}
		else
		{
			//printf("* Not an operator\n");
			enqueue(y, leftq);
		}
	}
	while(looktop(opera) != '\0'){
			ptr = (char*) malloc(2);
			ptr[0] = looktop(opera);
			ptr[1] = '\0';
			z.input = ptr;
			/*z.input[0] = looktop(opera);
			z.input[1] = '\0';*/
		enqueue(z,leftq);
		pop(opera);
	}
}

void print(struct queue* everything, int argc){
	union qvalue w;
	for(int i = 1; i < argc; i++){
		w = dequeue(everything);
		printf("%s ",w.input);
	}
	printf("\n");
}
