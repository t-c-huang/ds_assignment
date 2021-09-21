#include<stdio.h>
#include<stdlib.h>

typedef struct node Node;
struct node
{
    int data;
    Node* leftChild;
    Node* rightChild;
    Node* next;
};

Node *rootPointer = NULL, *front = NULL, *rear = NULL;
int first_print = 1;//first time print or not

//add node with num to the tree
void add_node(int num)
{
    //Assign memory to the new node.
    Node *temp = (Node*)malloc(sizeof(Node));
    temp->data = num;
    temp->leftChild = NULL;
    temp->rightChild = NULL;
    temp->next = NULL;

    //Only for creat the first node in the tree.
    if(!rootPointer)
        rootPointer = temp;

        //General cases.
    else
    {
        //find right place to add the new node.
        Node *ptr = rootPointer;

        while (1){
            //add num < current node
            if(num < ptr->data)
            {
                //find empty leftChild place to add the new node.
                if(ptr->leftChild == NULL)
                {
                    ptr-> leftChild = temp;
                    break;
                }
                    //move ptr to the leftChild
                else
                    ptr = ptr->leftChild;
            }

                //add num > current node
            else
            {
                //find empty rightChild place to add the new node.
                if(ptr -> rightChild == NULL)
                {
                    ptr -> rightChild = temp;
                    break;
                }
                    //move ptr to the rightChild
                else
                    ptr = ptr -> rightChild;
            }
        }
    }
    return;
}

//Function to find minimum node in right subtree.
Node* find_mini_node(Node* root)
{
    while(root->leftChild != NULL)
        root = root->leftChild;
    return root;
}

//function for delete node with value same as delete_num.
Node* delete_node(int delete_num,Node *root) {

    //the tree is empty
    if (root == NULL)
        return root;

        //delete number less than current root.
    else if (delete_num < root->data)
        root->leftChild = delete_node(delete_num, root->leftChild);

        //delete number bigger than current root.
    else if (delete_num > root->data)
        root->rightChild = delete_node(delete_num,root->rightChild);


        //the root now is the node we want to delete, doing different arrange for different child state.
    else {
        //case1: delete node has no child
        if (root->leftChild == NULL && root->rightChild == NULL) {
            free(root);
            root = NULL;
        }

            //case2: delete node has only left child
        else if (root->leftChild == NULL) {
            Node *temp = root;
            root = root->rightChild;
            free(temp);
        }

            //case3: delete node has only right child
        else if (root->rightChild == NULL) {
            Node *temp = root;
            root = root->leftChild;
            free(temp);
        }
            //case4: delete node has two child
        else {
            Node *temp = find_mini_node(root->rightChild);
            root->data = temp->data;
            root->rightChild = delete_node(temp->data,root->rightChild);
        }
    }
    return root;
}


//function of enqueue
void enqueue(Node* ptr)
{
    rear->next = ptr;
    rear = ptr;
    return;
}

//function of enqueue children of a node.
void enqueue_children(Node* ptr)
{
    if(ptr->leftChild)
        enqueue(ptr->leftChild);

    if(ptr->rightChild)
        enqueue(ptr->rightChild);
}

//function for print front node in queue and enqueue it's child.
void dequeue(void)
{
    Node *out_queue_node = front;
    int out_value = out_queue_node->data;

    if(first_print == 0)//from second time, print the space before print the value.
        printf("\n");
    printf("%d", out_value);

    enqueue_children(out_queue_node);//enqueue child of print node.

    front = front->next;

    free(out_queue_node);
    return;
}

//function for print out the remain nodes.
void print_tree()
{
    if(!rootPointer)//print nothing if tree is empty.
        return;

    front = rootPointer;
    rear  = rootPointer;

    do
    {
        dequeue();
        first_print = 0;
    }while (front!=NULL);

    return;
}

int main() {
    int input_value;
    char ch;

    while(1){
        scanf("%d",&input_value);
        add_node(input_value);
        ch=getchar();
        if(ch=='\n')
            break;
    }

    //read second line input.
    while(1){
        scanf("%d",&input_value);
        delete_node(input_value, rootPointer);
        ch=getchar();
        if(ch==EOF)
            break;
    }

    print_tree();
    return 0;

}