/*Data Structure Assignment [2]
Queueing in campus cafeteria
by NCKU CSIE E14084117*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>



//stack struct
typedef struct
{
    int ram_num;
}plate;


//queue structure
typedef struct
{
    int plate_num;
}person;
//test func


//stack functions
plate * push_s( plate *, int , int *,int *);
int pop_s(plate *plates, int *top);

//queue functions
person * enqueue_q(person *, int , int *, int *, int *);
int dequeue_q(person *, int *, int *);





int main(void){

    //parameters for read input operations.
    char *operation, line;
    int index = 0, ans = 0;



    //stack parameters.
    plate *plates;
    int capacity = 10;
    int top = -1;    // the capacity of stack start by 10.
    plates = (plate *)malloc(sizeof(plate)*capacity);

    //queue parameters.
    person *line_A, *line_B;

    int size_A = 10, front_A = -1, rear_A = -1,
            size_B = 10, front_B = -1, rear_B = -1;
    //assign memory to line (queue)
    line_A = (person *)malloc(sizeof(person)*size_A);
    line_B = (person *)malloc(sizeof(person)*size_B);


    while (scanf("%s",operation)!=EOF) {

        //Check operations from input.

        //PUSH N
        if(strcmp(operation, "PUSH") == 0)
        {   //indicates the staff refills a plate index N into the plate stack.
            //Read index of pushed plate.
            scanf("%d",&index);
            plates = push_s( plates, index , &capacity, &top);
        }

        //POP
        else if(strcmp(operation,"POP") == 0)
        {   //indicates the customer takes a plate from the top of the plate stake.
            index = pop_s(plates, &top);
        }

        //ENQUEUE X
        else if(strcmp(operation,"ENQUEUE") == 0)
        {   //indicates a customer joins the end of the line X.

            //Check which line to enqueue.
            scanf("%s",&line);

            switch(line)
            {
                case 'A':
                    line_A = enqueue_q(line_A, index, &front_A, &rear_A, &size_A);
                    break;

                case 'B':
                    line_B = enqueue_q(line_B, index, &front_B, &rear_B, &size_B);
                    break;
            }

        }

        //DEQUEUE X
        else if(strcmp(operation,"DEQUEUE") == 0)
        {   //indicates a customer at the front of the line X leaves the line to checkout.

            //Check which line to dequeue
            scanf("%s",&line);

            switch(line)
            {
                case 'A':
                    ans = dequeue_q(line_A, &front_A, &rear_A);
                    break;

                case 'B':
                    ans = dequeue_q(line_B, &front_B, &rear_B);
                    break;
            }
            printf("%d\n\n",ans);
        }

        //Other unexpected cases
        else
        {
            printf("INPUT UNEXPECTED OPERATION: %s\n",operation);
            break;
        }
    }

    //free memory
    free(plates);
    free(line_A);
    free(line_B);

    return 0;
}






//stack functions
plate * push_s( plate *plates, int index, int *capacity,int *top){// indicates the staff refills a plate index N into the plate stack.

    //If stack is full, realloc() more memory.
    if(*top == *capacity-1)
    {
        *capacity += 10;
        plates = (plate *)realloc(plates,sizeof(plate)*(*capacity));
    }

    *top+=1;
    plates[*top].ram_num = index;

    return plates;
}


int pop_s(plate *plates, int *top){//indicates the customer takes a plate from the top of the plate stake.

    //Check stack is empty or not.
    if(*top == -1)
    {
        printf("There isn't any plates to take\n");
        return -1;
    }

    int out = plates[*top].ram_num;
    *top-=1;

    return out;
}


//queue functions
person * enqueue_q(person *line_choose, int index, int *front, int *rear, int *line_size){  //indicates a customer joins the end of the line X.

    //for enqueue first element to queue, add 1 for -1 -> 0.
    if(*front==-1)
    {
        *front+=1;
    }

    //If queue is full, realloc() more memory.
    if(*rear == *line_size-1)
    {
        *line_size += 10;
        line_choose = (person *)realloc(line_choose,sizeof(person)*(*line_size));
    }

    *rear += 1;
    line_choose[*rear].plate_num = index;

    return line_choose;
}




int dequeue_q(person *line_choose, int *front, int *rear){   //indicates a customer at the front of the line X leaves the line to checkout.

    //Check the queue is empty or not.
    if(*rear == -1 && *front == *rear)
    {
        printf("This line is empty!\n");
        return -1;
    }

    int ans = line_choose[*front].plate_num;

    line_choose[*front].plate_num = 0;
    *front+=1;

    return ans;
}
