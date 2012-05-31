#include "Queue.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

Queue::Queue(char* name, AbstractController* controller)
{
    this->name = name;
    this->controller = controller;

    //optional placeholders in case attr is brought back
    attr.mq_maxmsg = 100;
    attr.mq_msgsize = MESSAGESIZE;
    attr.mq_flags = 0;

    // Create the thread
    mutex = PTHREAD_MUTEX_INITIALIZER;
    simonsConstant = 0;
    pthread_create (&t_thread, NULL, read_queue, this);
    std::cout << "Opened queue " << name << "\n";
}

Queue::~Queue()
{
    mq_unlink(name);
}

void* read_queue(void* args)
{
    Queue *queue = (Queue *)args;

    mqd_t qr;
    mq_message buf;      // struct to store the message
    char* hamburgler;    // char* for the above struct to pass to mq

    hamburgler = (char*)((void*)&buf);
            
    while (1)
    {
        //while(!queue->getSimonsConstant());
        //std::cout << "queueconsumerdown...";
        //queue->downMutex();

        if ((qr = mq_open(queue->get_name(), O_RDONLY, S_IRUSR, queue->getAttr())) < 0)
        {
            //std::cout << "Could not open Queue "<< queue->get_name() << "\n";
            //queue->upMutex();
            continue;
        }

        //std::cout << queue->getAttr()->mq_maxmsg << " max, " << queue->getAttr()->mq_msgsize << "size, " << queue->getAttr()->mq_curmsgs << " messages";

        if (mq_receive(qr, hamburgler, MESSAGESIZE, NULL) < 0)
        {
            //perror("error");
            mq_close(qr);
            //std::cout << "queueconsumerup!\n";
            //queue->upMutex();
            continue;
        }

        std::cout << "dequeue: " << buf.header << ", " << buf.sender << ", " << buf.msg << "\n";

/*        if (buf.header == MODE_CHANGE) {
            //queue->getController()->setFlag(buf.msg);
        }
        else if (buf.header == MODE_REQUEST)
        {
            // reply with message containing expected mode
        }
        else if (buf.header == STATUS_NOTIFY)
        {
            // message is an int containing state
            //mq_q.push()
        }*/

        queue->controller->receiveMessage(buf.sender, buf.header, buf.msg);

        mq_close(qr);

        //std::cout << "queueconsumerup!\n";
        //queue->upMutex();
        //queue->clearSimonsConstant();
    }
    return NULL;
}

void write_queue(char *dest, int header, char* sender, int msg)
{

    mqd_t    qs;                     // queue for sending
    char* cast_msg;
    mq_message message;
    struct mq_attr attr;

    message.header = header;
    sprintf(message.sender, sender);
    message.msg = msg;

    attr.mq_maxmsg = 100;
    attr.mq_msgsize = MESSAGESIZE;
    attr.mq_flags = 0;

    cast_msg = (char*)&message;
    
    //while(getSimonsConstant());
    //std::cout << "queueproducerdown...";
    //downMutex();

    if ((qs = mq_open(dest, Q_FLAGS, Q_PERM, &attr)) >= 0)
    {
        if (mq_send(qs, cast_msg, sizeof(message), 0) < 0)
            std::cout << "Error: Unable to send message\n";
        else
            std::cout << "Sent '" << cast_msg << "'i\n";
        //std::cout << "queueproducerup!\n";
        mq_close(qs);
    } else {
        std::cout << "Unable to open queue " << dest << "\n";
    }

    //upMutex();
    //setSimonsConstant();
}

void Queue::downMutex()
{
    pthread_mutex_lock(&mutex);
}

void Queue::upMutex()
{
    pthread_mutex_unlock(&mutex);
}

int Queue::getSimonsConstant()
{
    return simonsConstant;
}

void Queue::setSimonsConstant()
{
    simonsConstant = 1;
}

void Queue::clearSimonsConstant()
{
    simonsConstant = 0;
}

