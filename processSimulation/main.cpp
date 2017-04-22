#include <iostream>
#include <queue>
#include <MyQueue.h>
//#include <ProcessSimulation.h>
using namespace std;

enum ProcessState{
    ready,
    blocked,
    running
};
class PCB{
public:
    unsigned int id;
    ProcessState state;
    unsigned int timeNeed;
    unsigned int timeUsed;
    unsigned int priority;

};

class Process{
public:
    //PCB pcb;
    void create();
    static void kill();
    void block();
    void wakeup();

    PCB initPCB(unsigned int id, unsigned int timeNeed, unsigned int timeUsed, unsigned int priority);
    bool apply4Source();
    bool freeSource();
};

queue<PCB> readyQu;
queue<PCB> blockQu;
queue<PCB> runningQu;
queue<PCB> tmp;

void Process::create(){
    unsigned int id;
    unsigned int timeNeed;
    //unsigned int timeUsed;
    unsigned int priority;
    cout<<"input the (ID timeNeed priority) for the process you want to create:"<<endl;
    cin>>id>>timeNeed>>priority;

    PCB pcb = initPCB(id,timeNeed,0,priority);
    apply4Source();
    readyQu.push(pcb);//插入就绪队列
    cout<<"Process (ID:"<<pcb.id<<") has been created."<<endl;
}
void Process::kill(){
    unsigned int idToKill;
    cout<<"Input the ID of the process you want to kill:"<<endl;
    cin>>idToKill;
    while(!tmp.empty()){
        tmp.pop();
    }
    bool found = false;//表示是否找到该进程
    while(!runningQu.empty()){
        if(runningQu.front().id == idToKill){
            found = true;
            runningQu.pop();
            continue;//该进程不再保存到tmp队列中
        }
        tmp.push(runningQu.front());
        runningQu.pop();
    }
    while(!tmp.empty()){//将其他进程重新添加到队列
        runningQu.push(tmp.front());
        tmp.pop();
    }
    if(found){
        cout<<"the process has been killed."<<endl;
    }else{
        cout<<"the process was not found."<<endl;
    }

//    if(pcb.state == running){
//        freeSource();
//        runningQu.pop();//????????????????????
//    }
}
void Process::block(){
    unsigned int idToBlock;
    PCB proToBlock;
    cout<<"input the ID of the process you want to block:"<<endl;
    cin>>idToBlock;
    while(!tmp.empty()){
        tmp.pop();
    }
    bool found = false;//表示是否找到该进程
    while(!runningQu.empty()){
        if(runningQu.front().id == idToBlock){
            found = true;
            proToBlock = runningQu.front();
            runningQu.pop();
            continue;//该进程不再保存到tmp队列中
        }
        tmp.push(runningQu.front());
        runningQu.pop();
    }
    while(!tmp.empty()){//将其他进程重新添加到队列
        runningQu.push(tmp.front());
        tmp.pop();
    }
    if(found){
        proToBlock.state = blocked;
        blockQu.push(proToBlock);
        cout<<"the process has been blocked."<<endl;
    }else{
        cout<<"the process was not found."<<endl;
    }
}
void Process::wakeup(){
    unsigned int idToWakeup;
    PCB proToWakeup;
    cout<<"input the ID of the process you want to wakeup:"<<endl;
    cin>>idToWakeup;
    while(!tmp.empty()){
        tmp.pop();
    }
    bool found = false;//表示是否找到该进程
    while(!blockQu.empty()){
        if(blockQu.front().id == idToWakeup){
            found = true;
            proToWakeup = blockQu.front();
            blockQu.pop();
            continue;//该进程不再保存到tmp队列中
        }
        tmp.push(blockQu.front());
        blockQu.pop();
    }
    while(!tmp.empty()){//将其他进程重新添加到队列
        blockQu.push(tmp.front());
        tmp.pop();
    }
    if(found){
        proToWakeup.state = ready;
        readyQu.push(proToWakeup);
        cout<<"the process has been waked up."<<endl;
    }else{
        cout<<"the process was not found."<<endl;
    }
}
bool Process::apply4Source(){
    return true;
}
bool Process::freeSource(){
    return true;
}
PCB Process::initPCB(unsigned int id, unsigned int timeNeed, unsigned int timeUsed, unsigned int priority){
    PCB pcb;
    pcb.id = id;
    pcb.state = ready;
    pcb.priority = priority;
    pcb.timeNeed = timeNeed;
    pcb.timeUsed = timeUsed;
    return pcb;
}

void schedule(){
    if(readyQu.empty()){
        cout<<"Ready queue is empty!"<<endl;
        return;
    }else{
        readyQu.front().state = running;
        runningQu.push(readyQu.front());
        readyQu.pop();
        PCB pcb = runningQu.front();
        if(pcb.timeNeed>pcb.timeUsed){
            pcb.timeUsed ++;

        }
    }

}
void display(){
    while(!tmp.empty()){ tmp.pop();}//make the tmp queue empty
    cout<<"PID"<<"\t"<<"State"<<"\t"<<"Priority"<<endl;
    while(!readyQu.empty()){
        PCB p = readyQu.front();
        cout<<p.id<<"\t"<<"ready"<<"\t"<<p.priority<<endl;
        readyQu.pop();
        tmp.push(p);
    }
    while(!tmp.empty()){
        readyQu.push(tmp.front());
        tmp.pop();
    }
    while(!runningQu.empty()){
        PCB p = runningQu.front();
        cout<<p.id<<"\t"<<"running"<<"\t"<<p.priority<<endl;
        runningQu.pop();
        tmp.push(p);
    }
    while(!tmp.empty()){
        runningQu.push(tmp.front());
        tmp.pop();
    }
    while(!blockQu.empty()){
        PCB p = blockQu.front();
        cout<<p.id<<"\t"<<"block"<<"\t"<<p.priority<<endl;
        blockQu.pop();
        tmp.push(p);
    }
    while(!tmp.empty()){
        blockQu.push(tmp.front());
        tmp.pop();
    }
}
int main()
{
    bool run = true;
    while(run){

        cout<<"######ProcessSimulation:  menu #####"<<endl
            <<" 1. create process"<<endl
            <<" 2. kill process"<<endl
            <<" 3. block process"<<endl
            <<" 4. wakeup process"<<endl
            <<" 5. exit"<<endl
            <<" 0. exit"<<endl;
        int choice = 9;
        Process pro;
        cin>>choice;
        switch(choice){
            case 1:{//create
                pro.create();
                break;
            }
            case 2:{//kill
                pro.kill();
                break;
            }
            case 3:{//block
                pro.block();
                break;
            }
            case 4:{//wakeup
                pro.wakeup();
                break;
            }
            case 5:{//just run(nothing to do)
                break;
            }
            case 0:{//exit
                run = false;
                break;
            }
            default:{
                cout<<"error! Don't test my robustness ,,,"<<endl;
                break;
            }
        }
        if(run){
            display();
            schedule();
        }
    }//while end

        return 0;
}
