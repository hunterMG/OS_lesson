#include "MyQueue.h"
#include <cstdio>
#include <cstdlib>

bool isEmpty(struct myqueue<T> *head){
    if(head==nullptr)   return true;
    else    return false;
}

struct myqueue<T>* push(struct myqueue<T> *head){
    struct myqueue<T> *p = nullptr;
    p = (struct myqueue<T>*)malloc(sizeof(struct myqueue<T>))
    if(p==nullptr){
        printf("Error!No enough memory to allocate!\n");
    }

    return head;
}

