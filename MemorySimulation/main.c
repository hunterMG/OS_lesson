#include <stdio.h>
#include <stdlib.h>

struct FreeMem_block{
    int startAddr;
    int length;
    struct FreeMem_block *next;
};

struct FreeMem_block *freeMemBlockList;

void init(){//Init the freeMemBlockList
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
    p = p->next;

    struct FreeMem_block *p2 = (struct FreeMem_block*)malloc(sizeof(struct FreeMem_block));
    if(p2 == NULL){
        printf("Error!No enough memory to allocate!\n");
    }
    p2->startAddr = 400;
    p2->length = 60;
    p2->next = NULL;
    p->next = p2;
}
int isNoSpace(){
    if(freeMemBlockList == NULL){
        return 1;
    }else{
        return 0;
    }
}
/*First Fit algorithm
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
            pr->next = p->next;
            free(p);

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
void nextFit(int sizeNeed){
}
void bestFit(int sizeNeed){
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
        p = p->next;
    }
}
int main()
{
    printf("#------Memory Simulation-----#\n");
    printf("#---Address space: 0 ~ 1023 -#\n");
    init();

    int sizeNeed = 0;
    while(1){
        displayMemBlockList();
        printf("input the size of the space you want:-->>");
        scanf("%d", &sizeNeed);
        if(sizeNeed < 0){
            printf("THe size CAN'T be smaller than 0 !\n");
        }else{
            firstFit(sizeNeed);
        }
    }
    displayMemBlockList();
    return 0;
}
