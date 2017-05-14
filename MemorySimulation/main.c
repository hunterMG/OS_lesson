#include <stdio.h>
#include <stdlib.h>

enum Algorithm{
    FirstFit0,FirstFit, NextFit, BestFit
};
struct FreeMem_block{
    int startAddr;
    int length;
    struct FreeMem_block *next;
};

struct FreeMem_block *freeMemBlockList;
struct FreeMem_block *tailBlock;//for NextFit to loop lookup
int nodeNum = 3;//for Next Fit
void init(int algorithm){//Init the freeMemBlockList, 3 free blocks in total
    if(algorithm == BestFit){
        struct FreeMem_block *p;
        p = (struct FreeMem_block*)malloc(sizeof(struct FreeMem_block));
        if(p == NULL){
            printf("Error!No enough memory to allocate!\n");
        }
        p->startAddr = 400;
        p->length = 60;
        freeMemBlockList = p;

        struct FreeMem_block *p1 = (struct FreeMem_block*)malloc(sizeof(struct FreeMem_block));
        if(p1 == NULL){
            printf("Error!No enough memory to allocate!\n");
        }
        p1->startAddr = 200;
        p1->length = 100;
        p->next = p1;

        struct FreeMem_block *p2 = (struct FreeMem_block*)malloc(sizeof(struct FreeMem_block));
        if(p2 == NULL){
            printf("Error!No enough memory to allocate!\n");
        }
        p2->startAddr = 20;
        p2->length = 120;
        p1->next = p2;
        p2->next = NULL;

        return;
    }
    struct FreeMem_block *p;
    p = (struct FreeMem_block*)malloc(sizeof(struct FreeMem_block));
    if(p == NULL){
        printf("Error!No enough memory to allocate!\n");
    }
    p->startAddr = 20;
    p->length = 120;
    freeMemBlockList = p;

    struct FreeMem_block *p1 = (struct FreeMem_block*)malloc(sizeof(struct FreeMem_block));
    if(p1 == NULL){
        printf("Error!No enough memory to allocate!\n");
    }
    p1->startAddr = 200;
    p1->length = 100;
    p->next = p1;

    struct FreeMem_block *p2 = (struct FreeMem_block*)malloc(sizeof(struct FreeMem_block));
    if(p2 == NULL){
        printf("Error!No enough memory to allocate!\n");
    }
    p2->startAddr = 400;
    p2->length = 60;
    //tail node
    if(algorithm == FirstFit){
        p2->next = NULL;
    }else if(algorithm == NextFit){
        p2->next = freeMemBlockList;//loop
        nodeNum = 3;
    }

    p1->next = p2;

    tailBlock = p2;
}
// push --for Best Fit algorithm , order by block size(asc)
void pushToFreeBlockList(struct FreeMem_block *blockToPush){
    struct FreeMem_block *p = freeMemBlockList;
    if(freeMemBlockList == NULL){
        freeMemBlockList = blockToPush;
        blockToPush->next = NULL;
    }else{
        if(freeMemBlockList->length > blockToPush->length){
            blockToPush->next = freeMemBlockList;
            freeMemBlockList = blockToPush;
        }else{
            while(p->next != NULL){
                if(p->next->length > blockToPush->length){
                    blockToPush->next = p->next;
                    p->next = blockToPush;
                    break;
                }
                p = p->next;
            }
            if(p->next == NULL){
                p->next = blockToPush;
                blockToPush->next = NULL;
            }
        }
    }
}
int isNoSpace(){
    if(freeMemBlockList == NULL){
        return 1;
    }else{
        return 0;
    }
}
/* First Fit algorithm
    begin the search from the head of freeMemBlockList, allocate the first block which will satisfy the need
*/
void firstFit(int sizeNeed){
    if( isNoSpace() ){//No space free
        printf("OS has NO FREE SPACE NOW !\n");
        return;
    }
    struct FreeMem_block *p = freeMemBlockList;
    struct FreeMem_block *pr = p;
    int foundFlag = 0;
    while(p != NULL){
        if(p->length > sizeNeed){//freeBlock length > sizeNeed : allocate the space = sizeNeed
            p->startAddr = p->startAddr + sizeNeed;
            p->length = p->length - sizeNeed;

            foundFlag = 1;
            break;
        }else if(p->length == sizeNeed){//freeBlock length = sizeNeed : allocate the whole freeBlock,free the node of the list
            if(p == freeMemBlockList){//p is the head node
                freeMemBlockList = p->next;
                free(p);
            }else{
                pr->next = p->next;
                free(p);
            }

            foundFlag = 1;
            break;
        }
        pr = p;
        p = p->next;
    }
    if(foundFlag){
        printf("#---The space you want has been allocated.---#\n");
    }else{
        printf("#------------NO ENOUGH MEMOTY !--------------#\n");
    }
}
/* Next Fit algorithm
    begin the search from the next block of th one which has been found at the previous time.
*/
struct FreeMem_block *nextToFound;
void nextFit(int sizeNeed){
    if( isNoSpace() ){//No space free
        printf("OS has NO FREE SPACE NOW !\n");
        return;
    }
    struct FreeMem_block *p = freeMemBlockList;
    struct FreeMem_block *pr = NULL;
    if(p->next != NULL){//while the list has more than 1 node
        while(p!= NULL){//to find the previous node of 'nextToFound'
            if(p->next == nextToFound){
                pr = p;
                break;
            }
            p = p->next;
        }
    }
    p = nextToFound;
    int foundFlag = 0;
    int foundTime = 0;//to avoid endless loop in cycle list
    while(p != NULL){
        foundTime ++;
        if(p->length > sizeNeed){//freeBlock length > sizeNeed : allocate the space = sizeNeed
            p->startAddr = p->startAddr + sizeNeed;
            p->length = p->length - sizeNeed;
            if(p == tailBlock){
                nextToFound = freeMemBlockList;//loop lookup
            }else{
                nextToFound = p->next;/////////
            }
            foundFlag = 1;
            break;
        }else if(p->length == sizeNeed){//freeBlock length = sizeNeed : allocate the whole freeBlock,free the node of the list
            if(p == freeMemBlockList){//p is the head node
                if(p == tailBlock){
                    nextToFound = NULL;
                    freeMemBlockList = NULL;
                    tailBlock = NULL;
                }else{
                    nextToFound = p->next;///////////
                    freeMemBlockList = p->next;
                }
                free(p);
            }else{
                pr->next = p->next;
                if(p == tailBlock){
                    nextToFound = freeMemBlockList;//loop lookup
                    tailBlock = pr;
                    tailBlock->next = NULL;
                }else{
                    printf("-----------\n");
                    nextToFound = p->next;/////////
                }
                free(p);
            }

            foundFlag = 1;
            nodeNum --;
            break;
        }
        if(foundTime == nodeNum){//Not found the space when it has searched for 1 loop
            break;
        }
        pr = p;
        p = p->next;
    }
    if(foundFlag){
        printf("#---The space you want has been allocated.---#\n");
    }else{
        printf("#------------NO ENOUGH MEMOTY !--------------#\n");
    }
}
void bestFit(int sizeNeed){
    if( isNoSpace() ){//No space free
        printf("OS has NO FREE SPACE NOW !\n");
        return;
    }
    struct FreeMem_block *p = freeMemBlockList;
    struct FreeMem_block *pr = p;
    int foundFlag = 0;
    while(p != NULL){
        if(p->length > sizeNeed){//freeBlock length > sizeNeed : allocate the space = sizeNeed
            p->startAddr = p->startAddr + sizeNeed;
            p->length = p->length - sizeNeed;
            //length chaned , reorder:pop the node then push it in
            struct FreeMem_block *tmp = p;
            if(p == freeMemBlockList){// head node
                freeMemBlockList = p->next;//pop the node
                pushToFreeBlockList(tmp);  //push into
                printf("---------Reordered01----------\n");
            }else{
                pr->next = p->next;
                pushToFreeBlockList(tmp);
                printf("---------Reordered02----------\n");
            }

            foundFlag = 1;
            break;
        }else if(p->length == sizeNeed){//freeBlock length = sizeNeed : allocate the whole freeBlock,free the node of the list
            if(p == freeMemBlockList){//p is the head node
                freeMemBlockList = p->next;
                free(p);
            }else{
                pr->next = p->next;
                free(p);
            }

            foundFlag = 1;
            break;
        }
        pr = p;
        p = p->next;
    }
    if(foundFlag){
        printf("#---The space you want has been allocated.---#\n");
    }else{
        printf("#------------NO ENOUGH MEMOTY !--------------#\n");
    }

}
void displayMemBlockList(){
    if( isNoSpace() ){
        printf("OS has NO FREE SPACE NOW !\n");
        return;
    }else{
        printf("\nFree Memory Block List: \nstartAddr\tlength\n");
    }
    struct FreeMem_block *p = freeMemBlockList;
    while(p != NULL){
        printf("%6d\t\t%6d\n", p->startAddr, p->length);
        if(p == tailBlock){
            break;
        }
        p = p->next;
    }
}
void freeAllSpace(){
    struct FreeMem_block *p = freeMemBlockList;
    while(p!=NULL){
        struct FreeMem_block *tmp = p;
        p = p->next;
        free(tmp);
    }
}
void showMenu(){
    printf("#------Memory Simulation-----#\n");
    printf("#---Address space: 0 ~ 1023 -#\n");
    printf("?? which Algorithm do u prefer ??\n");
    printf("          1 --- First Fit\n");
    printf("          2 --- Next Fit\n");
    printf("          3 --- Best Fit\n>>");

}
int main()
{
    showMenu();
    int algorithm = FirstFit;
    scanf("%d", &algorithm);
    init(algorithm);
    nextToFound = freeMemBlockList;//init for the Next Fit algorithm
    int sizeNeed = 0;
    while(1){
        displayMemBlockList();
        printf("input the size of the space you want:-->>");
        scanf("%d", &sizeNeed);
        if(sizeNeed < 1){
            if(sizeNeed == 0)
            {
                printf("\nCongratulations!!! U have found the backdoor of this program. Simulation is refreshing...\n");
                showMenu();
                scanf("%d", &algorithm);
                freeAllSpace();
                init(algorithm);
            }else{
                printf("THe size CAN'T be smaller than 1 !\n");
            }
        }else{
            switch (algorithm){
            case FirstFit:
                firstFit(sizeNeed);
                break;
            case NextFit:
                nextFit(sizeNeed);
                break;
            case BestFit:
                bestFit(sizeNeed);
                break;
            default:
                firstFit(sizeNeed);
            }
        }
    }//end of while
    displayMemBlockList();
    return 0;
}
