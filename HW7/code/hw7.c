#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define max(x,y)    (x > y?x:y)
#define min(x,y)    (x < y?x:y)

typedef struct node ELEMENT;
struct node
{
    int nodeNum;
    union 
    {
        ELEMENT *earlyNext;
        ELEMENT *lateNext;
        ELEMENT *stackNext;
    }next;
    union 
    {
        int earlyDuration;
        int lateDuration;
    }duration;
};


typedef struct active ACTIVE;
struct active 
{
    int activeNum;
    int startVertex;
    int endVertex;
    int weight;
    int earlyTime;
    int lateTime;
    bool criticalActive;
};


typedef struct vertex VERTEX;
struct vertex
{
    int vertexNum;
    int earlyCount;
    int lateCount;
    int ee;
    int le;
    ELEMENT *earlyFirst;
    ELEMENT *lateFirst;
};


//  The function to doing topologic sort.
void topSort(ACTIVE *, VERTEX *, int, int);

//  Function for adding an ELEMENT to the stack.
void stackAdd(ELEMENT **, int);

//  Function for delete an ELEMENT from the stack.
int stackPop(ELEMENT **);

//  The functions to find ee[] and le[] of all vertex.
void find_ee(VERTEX * , ACTIVE *);

void find_le(VERTEX *, ACTIVE *, int);

//  Function for label all critical actives.
void findCriticalActive(ACTIVE *, VERTEX *, int);

//  Functions to find the early time and late time for all actives.
void findEarly(ACTIVE *, VERTEX *, int);

void findLate(ACTIVE *, VERTEX *, int);





int main()
{

    //  Read integer N (0 < N < 100), which indicates the number of activities (i.e., edges) of the network.
    int N = 0;
    scanf("%d", &N);
    
    //  Creat an ACTIVE array to store the data of actives.
    ACTIVE *activeArray = (ACTIVE *)malloc(sizeof(ACTIVE)*N);

    //  Read data of actives.
    for (int i = 0; i < N; i++)
    {
        scanf("%d", &activeArray[i].activeNum);
        scanf("%d", &activeArray[i].startVertex);
        scanf("%d", &activeArray[i].endVertex);
        scanf("%d", &activeArray[i].weight);
    }
    
    //  Find number of vertexes.
    int vertexNum = activeArray[N-1].endVertex;
    
    //  Create an VERTEX array to store the data of vertexes.
    VERTEX * vertexArray = (VERTEX*)malloc(sizeof(VERTEX)*(vertexNum+1));
    
    //  Initialize all vertex.
    for (int i = 0; i <= vertexNum; i++)
    {
        vertexArray[i].vertexNum = i;
        vertexArray[i].earlyCount = 0;
        vertexArray[i].lateCount = 0;
        vertexArray[i].earlyFirst = NULL;
        vertexArray[i].lateFirst = NULL;
        vertexArray[i].ee = 0;
        vertexArray[i].le = 0;
    }
        
    //  Doing topologic sort.
    topSort(activeArray, vertexArray, N, vertexNum);

    //  find ee[] and le[]
    find_ee(vertexArray, activeArray);
    find_le(vertexArray, activeArray, vertexNum);

    //  Find early time and late time.
    findEarly(activeArray, vertexArray, N);
    findLate(activeArray, vertexArray, N);

    //Marked the critical actives.
    findCriticalActive(activeArray, vertexArray, N);
    
    //  toggle first print.
    bool firstPrint = true;


    //  print out active number, it's early time and late time.
    for (int i = 0; i < N; i++)
    {
        if(firstPrint == false)
        printf("\n");
        printf("%d %d %d", activeArray[i].activeNum, activeArray[i].earlyTime, activeArray[i].lateTime);
        firstPrint = false;
    }


    printf("\n");

    //  Reset first print.
    firstPrint = true;

    //  Print out all critical actives.
    for (int i = 0; i < N; i++)
    {
        if (activeArray[i].criticalActive == true)
        {
            if(firstPrint == false)
                printf(" ");
            printf("%d",activeArray[i].activeNum);
            firstPrint = false;
        }
    }
    return 0;
}


void topSort(ACTIVE* activeArray, VERTEX* vertexArray, int N, int vertexNum)
{
    for (int i = 0; i < N; i++)
    {
        //*topSort for ee
        ELEMENT *newNode = (ELEMENT *)malloc(sizeof(ELEMENT));
        newNode->nodeNum = activeArray[i].endVertex;
        newNode->duration.earlyDuration = activeArray[i].weight;
        newNode->next.earlyNext= vertexArray[activeArray[i].startVertex].earlyFirst;
        vertexArray[activeArray[i].startVertex].earlyFirst = newNode;
        vertexArray[activeArray[i].endVertex].earlyCount++;

        //*topSort for le
        newNode = (ELEMENT *)malloc(sizeof(ELEMENT));
        newNode->nodeNum = activeArray[i].startVertex;
        newNode->duration.lateDuration = activeArray[i].weight;
        newNode->next.lateNext= vertexArray[activeArray[i].endVertex].lateFirst;
        vertexArray[activeArray[i].endVertex].lateFirst = newNode;
        vertexArray[activeArray[i].startVertex].lateCount++;
    }
}
void stackAdd(ELEMENT **topPtr, int addNum)
{
    ELEMENT *temp = (ELEMENT *)malloc(sizeof(ELEMENT));
    temp->nodeNum = addNum;
    temp->next.stackNext = *topPtr;
    *topPtr = temp;
}
int stackPop(ELEMENT **topPtr)
{
    int popNum = (*topPtr)->nodeNum;
    ELEMENT *ptr = *topPtr;
    (*topPtr) = (*topPtr)->next.stackNext;
    free(ptr);
    return popNum;
}
void find_ee(VERTEX* vertexArray, ACTIVE *activeArray)
{
    ELEMENT *stackTop = (ELEMENT *)malloc(sizeof(ELEMENT));
    stackTop->nodeNum = 0;
    stackTop->next.stackNext = NULL;
    int popNum = -1;
    
    do
    {
        popNum = stackPop(&stackTop);
        if(vertexArray[popNum].earlyCount <= 1)
            for (ELEMENT *ptr = vertexArray[popNum].earlyFirst; ptr!=NULL; ptr = ptr->next.earlyNext)
            {
                stackAdd(&stackTop,ptr->nodeNum);
                vertexArray[ptr->nodeNum].ee = max(vertexArray[ptr->nodeNum].ee,vertexArray[popNum].ee+ptr->duration.earlyDuration);
            }
        else
            vertexArray[popNum].earlyCount--;
    } 
    while (stackTop!=NULL);
}
void find_le(VERTEX* vertexArray, ACTIVE *activeArray, int vertexNum)
{
    for (int i = 0; i <= vertexNum; i++)
         vertexArray[i].le = vertexArray[vertexNum].ee;
    
    ELEMENT *stackTop = (ELEMENT *)malloc(sizeof(ELEMENT));
    stackTop->nodeNum = vertexNum;
    stackTop->next.stackNext = NULL;
    int popNum = -1;
    
    do
    {
        popNum = stackPop(&stackTop);
        if(vertexArray[popNum].lateCount <= 1)
            for (ELEMENT *ptr = vertexArray[popNum].lateFirst; ptr!=NULL; ptr = ptr->next.lateNext)
            {
                stackAdd(&stackTop,ptr->nodeNum);
                vertexArray[ptr->nodeNum].le = min(vertexArray[ptr->nodeNum].le,vertexArray[popNum].le-ptr->duration.lateDuration);
            }
        else
            vertexArray[popNum].lateCount--;
    } 
    while (stackTop!=NULL);
}
void findCriticalActive(ACTIVE *activeArray, VERTEX *vertexArray, int N)
{
    for (int i = 0; i < N; i++)
    {
        if(vertexArray[activeArray[i].startVertex].ee+activeArray[i].weight == vertexArray[activeArray[i].endVertex].le)
            activeArray[i].criticalActive = true;
        else
            activeArray[i].criticalActive = false;
    }
}
void findEarly(ACTIVE *activeArray, VERTEX *vertexArray, int N)
{
    for (int i = 0; i < N; i++)
    {
        activeArray[i].earlyTime = vertexArray[activeArray[i].startVertex].ee;
    }
}
void findLate(ACTIVE *activeArray, VERTEX *vertexArray, int N)
{
    for (int i = 0; i < N; i++)
    {
        activeArray[i].lateTime = vertexArray[activeArray[i].endVertex].le - activeArray[i].weight;
    }
}