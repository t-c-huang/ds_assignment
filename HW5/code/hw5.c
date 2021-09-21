#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TABLE_SIZE 13
#define PRIME 7

typedef struct hash_node hash_node;
struct hash_node
{
    int index;
    int hash_num;
    hash_node* next;
};
hash_node* front = NULL;

//hash functions are define by assignment.
int hash1(int hash_num)
{
    hash_num = hash_num % TABLE_SIZE;
    return hash_num;
}
int hash2(int hash_num)
{
    hash_num = PRIME - (hash_num%PRIME);
    return hash_num;
}
int hash_func(int hash_num,int i)
{
    hash_num = (hash1(hash_num) + i*hash2(hash_num)) % TABLE_SIZE;
    return hash_num;
}

//add a new number to the hash table.
void hash_value(int hash_num , int index)
{
    //set up new hash_node.
    hash_node *temp = malloc(sizeof(hash_node));
    temp->hash_num = hash_num;
    temp->index = index;
    temp->next = NULL;


    //find right place to add new hash_node

    //case1: if it's the first hash_node to add
    if(!front)
    {
        front = temp;
        return;
    }

    //case2: the index less then current front
    if(index<front->index)
    {
        temp->next = front;
        front = temp;
        return;
    }

    //case3: other general cases
    hash_node *ptr_1 = front ,*ptr_2 = front->next;

    while(ptr_2!=NULL)
    {
        //if find the right place.
        if(index<ptr_2->index)
        {
            ptr_1->next = temp;
            temp->next = ptr_2;
            return;
        }
        //shift ptr_1 ans ptr_2 to right 1 node.
        ptr_1 = ptr_2;
        ptr_2 = ptr_2->next;
    }

    ptr_1->next = temp;
    return;
}

//print out the hash table by index order.
int first_print = true;
void print_hash(void)
{
    hash_node *ptr = front;
    while(ptr)
    {
        //did not print out the '\n' before output value first time.
        if(first_print == false)
            printf("\n");
        first_print = false;
        printf("%d->%d",ptr->index,ptr->hash_num);

        ptr = ptr->next;
    }

    return;
}



int main() {
    int input_num = 0;
    int list_i[13] = {0};
    while(scanf("%d ",&input_num)!=EOF)
    {
        //add input_num to the hash table
        int i = 0;
        while(list_i[hash_func(input_num,i)]!=false)
            i++;
        list_i[hash_func(input_num,i)] = true;
        hash_value(input_num,hash_func(input_num,i));
    }

    //print out the hash table.
    print_hash();
    return 0;
}



