#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//create card structure
typedef struct card_struct {
    int card_value;
    struct card_struct *next_card;
}card_node;

card_node *front = NULL , *rear = NULL;

//queue functions

void enqueue(int card_in)//put the card into the card queue
{
    card_node *new_card_node = (card_node*)malloc(sizeof(card_node));
    new_card_node -> card_value = card_in;
    new_card_node->next_card = NULL;

    if (rear == NULL)
    {
        front  = new_card_node;
        rear = new_card_node;
    }

    else {
        rear->next_card = new_card_node;
        rear = new_card_node;
    }
    return;
}

int dequeue(void)//draw out a card from the card queue
{
    card_node *out_card_node = front;

    if (out_card_node != NULL) {
        front = front->next_card;
        int out_value = out_card_node->card_value;
        free(out_card_node);
        return out_value;
    }

    return -1;

}


int print_times = 0;//counting print out times (for not change line at the end)

int main(void) {

    char *input;
    int card_in;

    //create the card queue from input data.
    for (int i = 0; i<13; i++)
    {
        scanf("%s", input);

        if(!strcmp(input,"K"))
            card_in = 13;

        else if (!strcmp(input,"Q"))
            card_in = 12;

        else if (!strcmp(input,"J"))
            card_in = 11;

        else if (!strcmp(input,"A"))
            card_in = 1;

        else
            card_in = atoi(input);

        enqueue(card_in);
    }



    int index = 13, card_out;

    while (index>0)//while not finish draw out.(end at index=1(A))
    {
        if (print_times > 0)
            printf("\n");
        for (card_node *ptr = front; ptr!= NULL; ptr = ptr->next_card)//print all cards in the queue
        {

            if(ptr->card_value == 13)
                printf("K ");

            else if (ptr->card_value == 12)
                printf("Q ");

            else if (ptr->card_value ==11)
                printf("J ");

            else if (ptr->card_value ==1)
                printf("A ");

            else
                printf("%d ", ptr->card_value);
        }
        print_times = 1;



        card_out = dequeue();//draw out a card.

        if (card_out != index)//check it's target value(in order) or not
            enqueue(card_out);//put the card back to queue.
        else
            index--;//change to next target value.
    }
    return 0;
}


