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
    unsigned int priority;
    struct PCB *next;
};

struct PCB *readyQu;//��������
struct PCB *blockQu;//��������
struct PCB *runningPro;//��ǰ�������е�process
//struct PCB *tmp;

struct PCB* initPCB(unsigned int id, unsigned int timeNeed, unsigned int priority){
    struct PCB *pcb = NULL;
    pcb = (struct PCB*)malloc(sizeof(struct PCB));
    if(pcb == NULL){
        printf("Error!No enough memory to allocate!\n");
    }
    pcb->id = id;
    pcb->state = ready;
    pcb->priority = priority;
    pcb->timeNeed = timeNeed;
    pcb->next = NULL;
    return pcb;
}
void pushToReadyQu(struct PCB *pcb){//�������ȼ�����readyQu
    struct PCB *pr = readyQu;
    if(readyQu == NULL){//����Ϊ��ʱ
        readyQu = pcb;
        pcb->next = NULL;
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
                pcb->next = NULL;//important!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            }
        }
    }
}
void pushToBlockQu(struct PCB *pcb){
    if(blockQu == NULL){//����Ϊ��ʱ
        blockQu = pcb;
        pcb->next = NULL;
    }else{//���зǿ�ʱ
        //�嵽����
        pcb->next = blockQu;
        blockQu = pcb;
        /*�嵽��β
        while(pr->next != NULL){
            pr = pr->next;
        }
        pr->next =  pcb;
        pcb->next = NULL;
        */
    }
}
int isPidDuplicate(int idToCreate){
    int res = 0;
    if(runningPro != NULL){
        if(runningPro->id == idToCreate)
        {
            res = 1;
        }
    }
    if(readyQu != NULL){
        struct PCB *p = readyQu;
        while(p!= NULL){
            if(p->id == idToCreate){
                res = 1;
                break;
            }
            p = p->next;
        }
    }
    if(blockQu != NULL){
        struct PCB *p = blockQu;
        while(p!= NULL){
            if(p->id == idToCreate){
                res = 1;
                break;
            }
            p = p->next;
        }
    }
    if(res){
        printf("---------------Id duplicated. Please change to another nunmber.\n");
    }
    return res;
}
void create(){
    unsigned int id;
    unsigned int timeNeed;
    unsigned int priority;
    do{
        printf("Input the [ID priority timeNeed] for the process you want to create:\n");
        scanf("%d%d%d", &id, &priority, &timeNeed);
    }while(isPidDuplicate(id));
    //��ʼ��PCB
    struct PCB *pcb = initPCB(id, timeNeed, priority) ;
    //�����������
    pushToReadyQu(pcb);

    printf("\n---------------------------------Process (ID:[%d]) has been created.\n",pcb->id);

}

void kill(){
    if(runningPro != NULL){
        int id = runningPro->id;
        free(runningPro);
        printf("---------------------------------Process(id:[%d] has been killed.\n)", id);
        runningPro = NULL;
    }else{
        printf("---------------------------------There is no process running now!\n");
    }
}
void block(){
    if(runningPro != NULL){
        int id = runningPro->id;
        runningPro->state = blocked;
        pushToBlockQu(runningPro);
        printf("---------------------------------Process(id:[%d]) has been blocked.\n)", id);
        runningPro = NULL;
    }else{
        printf("---------------------------------There is no process running now!\n");
    }
}
void wakeup(){
    if(blockQu == NULL){
        printf("---------------------------------Block queue is empty!\n");
    }else{
        printf("Input the id of the process you want to wake up:> ");
        int idToWakeup;
        scanf("%d", &idToWakeup);

        int found = 0;
        struct PCB *p = blockQu;
        if(p->id == idToWakeup){//���ѵ��Ƕ��׽���
            if(p->next != NULL){//blockQu�в�ֻ��һ��PCB
                blockQu = p->next;
                pushToReadyQu(p);
            }else{
                pushToReadyQu(p);
                blockQu = NULL;
            }
            printf("---------------------------------Process(id:[%d]) has been waked up.\n)", idToWakeup);
            return;//��������
        }
        while(p->next != NULL){
            if(p->next->id == idToWakeup){
                struct PCB * pToWakeup = p->next;
                p->next = pToWakeup->next;
                pToWakeup->state = ready;
                pushToReadyQu(pToWakeup);
                printf("---------------------------------Process(id:[%d]) has been waked up.\n)", idToWakeup);
                found = 1;//�ҵ���
                break;
            }
            p = p->next;
        }
        if(!found){
            printf("---------------------------------Process(id:[%d]) not found.\n",idToWakeup);
        }
    }
}
//��������Ȩ����̬���ĵ���
void schedule(){
    //���������еĽ��̷ŵ��������л�����ý���
    if(runningPro != NULL){
        //��鵱ǰpro��timeNeed,��Ϊ0���������������ӵ�readyQu
        if(runningPro->timeNeed == 0){
            int id  = runningPro->id;
            free(runningPro);//�ͷ�ָ��
            printf("---------------------------------Process(id:[%d]) teminated.\n", id);
        }else{
            //priority��1������readyQu
            if(runningPro->priority > 0){
                runningPro->priority --;
            }
            runningPro->state = ready;
            pushToReadyQu(runningPro);
        }
    }
    //ѡreadyQu��һ������
    if(readyQu == NULL){
        printf("---------------------------------the ready queue is empty!\n");
        runningPro = NULL;
    }else{
        runningPro = readyQu;
        printf("____________________________________Process(id: [%d] ) is running.\n", runningPro->id);
        readyQu = readyQu->next;//��runningPro�Ƴ�readyQu
        runningPro->next = NULL;//important!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        runningPro->state = running;
        runningPro->timeNeed --;
    }
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
        printf("%d\t%s\t%d\t\t%d\n", p->id, stateToStr(p->state), p->priority, p->timeNeed);
        p = p->next;
    }
}
void display(){
    printf("PID\tState\tPriority\ttimeNeed\n");
    printf("----------------------------------------\n");
    if(runningPro != NULL){
        printf("%d\t%s\t%d\t\t%d\n", runningPro->id, stateToStr(runningPro->state), runningPro->priority, runningPro->timeNeed);
    }
    printf("----------------------------------------\n");
    print(readyQu);
    printf("----------------------------------------\n");
    print(blockQu);
}
int main()
{
    int run = 1;
    while(run){
        printf("######ProcessSimulation:  menu #####\n1. create process  2. kill process\n3. block process   4. wakeup process\n5. just run        0. exit\n");
        printf("##########-->");
        int choice = 5;
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
            case 5:{//just run 1 time_slice
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
            display();
        }
    }//while end

    return 0;
}
