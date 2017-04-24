#ifndef MYQUEUE_H_INCLUDED
#define MYQUEUE_H_INCLUDED

template <class T>
struct myqueue{
    struct T data;
    struct myqueue<T> *next;

};

bool isEmpty(struct myqueue<T> *head);
struct myqueue<T>* push(struct myqueue<T> *head);


#endif // MYQUEUE_H_INCLUDED
