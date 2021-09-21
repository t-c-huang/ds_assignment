#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

//define the AVL tree structure
typedef struct treeNode *treePointer;

struct treeNode {
    treePointer leftChild;
    treePointer rightChild;
    int BF;
    char phoneNum[11];
    char name[21];
};

//define global variables
treePointer AVLroot = NULL;//the tree pointer point to the root of AVL tree.
bool checkBalance = true;//false for need to check balance.

//Do LL rotation when AVL tree unbalance.
void LLrotation(treePointer* root)
{
    treePointer child_L  = (*root)->leftChild;

    (*root)->leftChild = child_L->rightChild;
    child_L->rightChild = (*root);
    (*root)->BF = 0;
    (*root) = child_L;
    return;
}

//Do RR rotation when AVL tree unbalance.
void RRrotation(treePointer* root)
{
    treePointer child_R  = (*root)->rightChild;

    (*root)->rightChild = child_R->leftChild;
    child_R->leftChild = (*root);
    (*root)->BF = 0;
    (*root) = child_R;
    return;
}

//Do LR rotation when AVL tree unbalance.
void LRrotation(treePointer* root)
{
    treePointer child_L = (*root)->leftChild;
    treePointer grandChild_LR = child_L->rightChild;

    child_L->rightChild = grandChild_LR->leftChild;
    grandChild_LR->leftChild = child_L;
    (*root)->leftChild = grandChild_LR->rightChild;
    grandChild_LR->rightChild = (*root);

    switch(grandChild_LR->BF) {
        case 1:
            child_L->BF = 0;
            (*root)->BF = -1;
            break;
        case 0:
            child_L->BF = 0;
            (*root)->BF = 0;
            break;
        case -1:
            child_L->BF = 1;
            (*root)->BF = 0;
    }
    (*root) = grandChild_LR;
}

//Do RL rotation when AVL tree unbalance.
void RLrotation(treePointer* root)
{
    treePointer child_R = (*root)->rightChild;
    treePointer grandChild_RL = child_R->leftChild;

    child_R->leftChild = grandChild_RL->rightChild;
    grandChild_RL->rightChild = child_R;
    (*root)->rightChild = grandChild_RL->leftChild;
    grandChild_RL->leftChild = (*root);

    switch(grandChild_RL->BF) {
        case -1:

            child_R->BF = 0;
            (*root)->BF = 1;
            break;
        case 0:
            child_R->BF = 0;
            (*root)->BF = 0;
            break;
        case 1:
            child_R->BF = -1;
            (*root)->BF = 0;
    }
    (*root) = grandChild_RL;
}




//AVL tree insert function
void insert(treePointer *root, char* addName, char* addNum)
{

    //for add first node and creat a new node
    if (!*root) {
        //memory and data assign
        *root = (treePointer)malloc(sizeof(struct treeNode));
        strcpy((*root)->name, addName);
        strcpy((*root)->phoneNum, addNum);
        (*root)->leftChild = NULL;
        (*root)->rightChild = NULL;
        (*root)->BF = 0;//BF of new node is 0
        checkBalance = false;//because
        return;
    }
    else if (strcmp(addName, (*root)->name) < 0) {//addName<(*root)->name,go left.

        insert(&(*root)->leftChild, addName, addNum);//insert new node to the left.

        if (!checkBalance) {//still need th check for BF.

            switch ((*root)->BF) {

                case 1://BF of the root now is 1, change to 0 after rotation.
                    if ((*root)->leftChild->BF == 1)
                        LLrotation(root);

                    else if ((*root)->leftChild->BF == -1)
                        LRrotation(root);

                    (*root)->BF = 0;
                    checkBalance = true;//the tree after rotation is balance.
                    break;

                case -1://BF of the root now is -1, change to 0 because insert a new node to the left.
                    checkBalance = true;//the tree is balance after insertion to the left.
                    (*root)->BF = 0;
                    break;

                case 0://BF of the root now is 0, change to 1 because insert a new node to the left.
                    (*root)->BF = 1;
                    break;

            }
        }
    }
    else if (strcmp(addName, (*root)->name) > 0) {//addName>(*root)->name,go right.

        insert(&(*root)->rightChild, addName, addNum);//insert new node to the right.

        if (!checkBalance) {//still need th check for BF.

            switch ((*root)->BF) {
                case -1://BF of the root now is -1, change to 0 after rotation.
                    if ((*root)->rightChild->BF == -1)
                        RRrotation(root);

                    else if ((*root)->rightChild->BF == 1)
                        RLrotation(root);

                    (*root)->BF = 0;
                    checkBalance = true;//the tree after rotation is balance.
                    break;

                case 1://BF of the root now is 1, change to 0 because insert a new node to the right.
                    checkBalance = true;//the tree is balance after insertion to the right.
                    (*root)->BF = 0;
                    break;

                case 0://BF of the root now is 0, change to -1 because insert a new node to the right.
                    (*root)->BF = -1;
                    break;
            }
        }
    }
    return;
}


//pre-order VLR traversal function
bool firstPrint = true;
void preorder(treePointer root)
{
    if(root)//not empty tree or leaf
    {
        if(firstPrint == false)//print space (if not first time print)
            printf(" ");
        printf("%s", root->name);
        firstPrint = false;
        preorder(root->leftChild);
        preorder(root->rightChild);
    }
}


//search a name in tree, return 1 and print data when the name was found(return 0 otherwise)
bool  search(treePointer root,char* find_name)
{
    if(!root)
        return false;
    if(strcmp(find_name,root->name)==0)
    {
        printf("%s",root->phoneNum);
        return true;
    }
    if(strcmp(find_name,root->name)<0)
        return search(root->leftChild, find_name);
    return search(root->rightChild, find_name);
}



int main() {

    //read input data
    char inputName[21] = {};
    char inputPhoneNum[21] = {};

    //input:Data(D)
    scanf("%s", inputName); //to skip D
    while(1) {
        scanf("%s", inputName);
        if (strcmp(inputName,"S")==0)
            break;
        scanf("%s",inputPhoneNum);
        insert(&AVLroot, inputName, inputPhoneNum);//insert a new node.
    }

    //output:Name in preorder
    preorder(AVLroot);
    printf("\n");
    firstPrint = true;

    //step:Search(S)
    while(scanf("%s", inputName)&&strcmp(inputName,"E")) {
        //output:Name and phone number pair
        if(firstPrint == false)//change line (if not first time print)
            printf("\n");
        printf("%s ", inputName);
        firstPrint = false;
        if(!search(AVLroot, inputName))
            printf("null");
    }
    return 0;
}