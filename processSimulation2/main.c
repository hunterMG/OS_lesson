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
void create(){
    unsigned int id;
    unsigned int timeNeed;
    unsigned int priority;
    printf("Input the [ID priority timeNeed] for the process you want to create:\n");
    scanf("%d%d%d", &id, &priority, &timeNeed);
    //��ʼ��PCB
    struct PCB *pcb = initPCB(id, timeNeed, priority) ;
    //�����������
    pushToReadyQu(pcb);

    printf("\nProcess (ID:%d) has been created.\n",pcb->id);

}

void kill(){

}
void block(){

}
void wakeup(){

}
//��������Ȩ����̬���ĵ���
void schedule(){
    //���������еĽ��̷ŵ��������л�����ý���
    if(runningPro != NULL){
        //��鵱ǰpro��timeNeed,��Ϊ0���������������ӵ�readyQu
        if(runningPro->timeNeed == 0){
            free(runningPro);//�ͷ�ָ��
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
        printf("the ready queue is empty!\n");
        runningPro = NULL;
    }else{
        runningPro = readyQu;
        printf("______Process(id: %d ) is running.\n", runningPro->id);
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
    printf("running:\n");
    if(runningPro != NULL){
        printf("%d\t%s\t%d\t\t%d\n", runningPro->id, stateToStr(runningPro->state), runningPro->priority, runningPro->timeNeed);
    }
    printf("ready:\n");
    print(readyQu);
    printf("blocked:\n");
    print(blockQu);
}
int main()
{
    int run = 1;
    while(run){
        printf("######ProcessSimulation:  menu #####\n1. create process  2. kill process\n3. block process   4. wakeup process\n5. just run        0. exit\n");
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
