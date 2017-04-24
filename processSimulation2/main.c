#include <stdio.h>
#include <stdlib.h>

enum ProcessState{
    ready,
    blocked,
    running
};
struct PCB{
    unsigned int id;
    int state;
    unsigned int timeNeed;
    unsigned int timeUsed;
    unsigned int priority;
    struct PCB *next;
};

struct PCB *readyQu;
struct PCB *blockQu;
//struct PCB *runningQu;
//struct PCB *tmp;

struct PCB* initPCB(unsigned int id, unsigned int timeNeed, unsigned int timeUsed, unsigned int priority){
    struct PCB *pcb = NULL;
    pcb = (struct PCB*)malloc(sizeof(struct PCB));
    if(pcb == NULL){
        printf("Error!No enough memory to allocate!\n");
    }
    pcb->id = id;
    pcb->state = ready;
    pcb->priority = priority;
    pcb->timeNeed = timeNeed;
    pcb->timeUsed = timeUsed;
    pcb->next = NULL;
    return pcb;
}

void create(){
    unsigned int id;
    unsigned int timeNeed;
    unsigned int priority;
    printf("input the (ID timeNeed priority) for the process you want to create:\n");
    scanf("%d%d%d",&id,&timeNeed,&priority);
    struct PCB *pcb = initPCB(id,timeNeed,0,priority) , *pr = readyQu;
    //readyQu.push(pcb);//�����������
    if(readyQu == NULL){//����Ϊ��ʱ
        readyQu = pcb;
    }else{//���зǿ�ʱ
        if(readyQu->priority < pcb->priority){//�������ȼ����ڵ�ǰpcb�����ȼ�ʱ
            pcb->next = readyQu;
            readyQu = pcb;
        }else{
            while(pr->next != NULL){
                if(pr->next->priority < pcb->priority){
                    pcb->next = pr->next;
                    pr->next = pcb;
                    break;
                }
                pr = pr->next;
            }
            if(pr->next == NULL){//��ǰpcb���ȼ���ͣ����˶�β
                pr->next = pcb;
            }
        }
    }
    printf("Process (ID:%d) has been created.\n",pcb->id);

}

void kill(){

}
void block(){

}
void wakeup(){

}
void schedule(){

}
char* stateToStr(int state){
    switch (state){
    case ready:
        return "ready";
        break;
    case running:
        return "running";
        break;
    case blocked:
        return "blocked";
        break;
    default:
        return "error";
    }
}
void print(struct PCB *p){
    while(p != NULL){
        printf("%d\t%s\t%d\n", p->id, stateToStr(p->state), p->priority);
        p = p->next;
    }
}
void display(){
    printf("PID\tState\tPriority\n");
    print(readyQu);
    print(blockQu);
}
int main()
{
    int run = 1;
    while(run){
        printf("######ProcessSimulation:  menu #####\n1. create process\n2. kill process\n3. block process\n4. wakeup process\n5. just run\n0. exit\n");
        int choice = 9;
        scanf("%d",&choice);
        switch(choice){
            case 1:{//create
                create();
                break;
            }
            case 2:{//kill
                kill();
                break;
            }
            case 3:{//block
                block();
                break;
            }
            case 4:{//wakeup
                wakeup();
                break;
            }
            case 5:{//just run(nothing to do)
                break;
            }
            case 0:{//exit
                run = 0;
                break;
            }
            default:{
                printf("input error! Don't test my robustness ,,,\n");
                continue;
                break;
            }
        }
        if(run){
            schedule();
            display(readyQu);
        }
    }//while end

    return 0;
}
