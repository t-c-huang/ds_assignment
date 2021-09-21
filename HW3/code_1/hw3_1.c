#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//define stack and queue data type ( with linked list characters)

typedef struct stack_struct
{
    int num_in_stack;//for store plate number
    struct stack_struct *stack_link;
}stack_node;

stack_node *top = NULL;

typedef struct queue_struct
{
    int num_in_queue;//for store plate number
    struct queue_struct *queue_link;

}queue_node;

queue_node *frontA = NULL, *rearA = NULL,
           *frontB = NULL, *rearB = NULL;


//stack functions
void push_stack(int);//PUSH N (indicates the staff refills a plate index N into the plate stack.)
int pop_stack(void);//POP (indicates the customer takes a plate from the top of the plate stake.)


//queue functions
void enqueue(int, char);//ENQUEUE X (indicates a customer joins the end of the line X.)
int dequeue(char);//DEQUEUE X (indicates a customer at the front of the line X leaves the line to checkout.)


int print_times = 0;//counting print out times (for not change line at the end)

int main(void)
{
    char *operation, line;
    int push_num, pop_num, dequeue_num;

    while(scanf("%s",operation)!=EOF){

        //PUSH N (indicates the staff refills a plate index N into the plate stack.)
        if(strcmp(operation, "PUSH") == 0)
        {
            scanf("%d",&push_num); //Read index fallow by operation.
            push_stack(push_num);
        }

        //POP (indicates the customer takes a plate from the top of the plate stake.)
        else if(strcmp(operation,"POP") == 0)
            pop_num = pop_stack();

        //ENQUEUE X (indicates a customer joins the end of the line X.)
        else if(strcmp(operation,"ENQUEUE") == 0)
        {
            scanf("%s",&line);  //Read line fallow by operation.
            enqueue(pop_num, line);
        }

        //DEQUEUE X (indicates a customer at the front of the line X leaves the line to checkout.)
        else if(strcmp(operation,"DEQUEUE") == 0)
        {
            scanf("%s",&line);  //Read line fallow by operation.
            dequeue_num = dequeue(line);
            if(print_times>0)//if isn't first time print answer change line before print answer.
                printf("\n");
            printf("%d",dequeue_num);
            print_times=1;
        }
        else
            break;
    }
    return 0;
}


//stack functions
void push_stack(int push_num)
{
    stack_node *new_stack_node = (stack_node*)malloc(sizeof(stack_node));

    new_stack_node -> num_in_stack = push_num;

    if (top != NULL)//not empty stack
        new_stack_node->stack_link = top;

    top = new_stack_node;
    return;
}


int pop_stack(void)
{
    if (top == NULL){//stack is empty
        //printf("The stack is empty\n");
        return -1;
    }

    stack_node *pop_stack_node = top;

    int pop_num = pop_stack_node->num_in_stack;
    top = pop_stack_node ->stack_link;

    free(pop_stack_node);

    return pop_num;
}

//queue functions

void enqueue(int enqueue_num, char line)
{
    queue_node *new_queue_node = (queue_node *) malloc(sizeof(queue_node));
    new_queue_node->num_in_queue = enqueue_num;
    new_queue_node->queue_link= NULL;

    switch (line){
        case 'A':
            if(frontA)
                rearA->queue_link = new_queue_node;
            else
                frontA = new_queue_node;
            rearA = new_queue_node;
            break;

        case 'B':
            if(frontB)
                rearB->queue_link = new_queue_node;
            else
                frontB = new_queue_node;
            rearB = new_queue_node;
            break;
    }
    return;
}

int dequeue(char line){
    queue_node *out_queue_node;
    int out_value;
    switch (line) {
        case 'A':
            out_queue_node = frontA;
            if(!out_queue_node)
            {
                printf("queue is empty\n");
                return -1;
            }
            out_value = out_queue_node->num_in_queue;
            frontA = out_queue_node ->queue_link;
            break;
        case 'B':
            out_queue_node = frontB;
            if(!out_queue_node)
            {
                printf("queue is empty\n");
                return -1;
            }
            out_value = out_queue_node->num_in_queue;
            frontB = out_queue_node ->queue_link;
            break;
    }
    free(out_queue_node);
    return out_value;
}

