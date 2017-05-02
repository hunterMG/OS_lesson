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

struct PCB *readyQu;//就绪队列
struct PCB *blockQu;//阻塞队列
struct PCB *runningPro;//当前正在运行的process
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
void pushToReadyQu(struct PCB *pcb){//根据优先级插入readyQu
    struct PCB *pr = readyQu;
    if(readyQu == NULL){//队列为空时
        readyQu = pcb;
        pcb->next = NULL;
    }else{//队列非空时
        if(readyQu->priority < pcb->priority){//队首优先级低于当前pcb的优先级时
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
            if(pr->next == NULL){//当前pcb优先级最低，到了队尾
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
    //初始化PCB
    struct PCB *pcb = initPCB(id, timeNeed, priority) ;
    //插入就绪队列
    pushToReadyQu(pcb);

    printf("\nProcess (ID:%d) has been created.\n",pcb->id);

}

void kill(){

}
void block(){

}
void wakeup(){

}
//基于优先权（动态）的调度
void schedule(){
    //将正在运行的进程放到就绪队列或结束该进程
    if(runningPro != NULL){
        //检查当前pro的timeNeed,若为0则结束它；否则添加到readyQu
        if(runningPro->timeNeed == 0){
            free(runningPro);//释放指针
        }else{
            //priority减1，插入readyQu
            if(runningPro->priority > 0){
                runningPro->priority --;
            }
            runningPro->state = ready;
            pushToReadyQu(runningPro);
        }
    }
    //选readyQu第一个运行
    if(readyQu == NULL){
        printf("the ready queue is empty!\n");
        runningPro = NULL;
    }else{
        runningPro = readyQu;
        printf("______Process(id: %d ) is running.\n", runningPro->id);
        readyQu = readyQu->next;//将runningPro移出readyQu
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
