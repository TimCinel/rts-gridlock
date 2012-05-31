#include "Queue.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

Queue::Queue(char* name, AbstractController* controller)
{
    this->name = name;
    this->controller = controller;

    attr.mq_maxmsg = 100;
    attr.mq_msgsize = MESSAGESIZE;
    attr.mq_flags = 0;

    //create the thread
    pthread_create (&t_thread, NULL, read_queue, this);
    std::cerr << "Opened queue " << name << "\n";
}

Queue::~Queue()
{
    mq_unlink(name);
}

char* Queue::get_name()
{
    return name;
}

struct mq_attr* Queue::getAttr()
{
    return &attr;
}

void Queue::set_name(char* name)
{
    this->name = name;
}

AbstractController* Queue::getController()
{
    return controller;
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
        if ((qr = mq_open(queue->get_name(), O_CREAT | O_RDONLY, S_IRUSR, queue->getAttr())) < 0)
        {
            std::cerr << "Could not open Queue "<< queue->get_name() << "(read)\n";
            continue;
        }

        std::cerr << queue->getAttr()->mq_maxmsg << " max, " << queue->getAttr()->mq_msgsize << "size, " << queue->getAttr()->mq_curmsgs << " messages";

        if (mq_receive(qr, hamburgler, MESSAGESIZE, NULL) < 0)
        {
            perror("Error");
            mq_close(qr);
            continue;
        }

        std::cerr << "dequeue: " << buf.header << ", " << buf.sender << ", " << buf.msg << "\n";

        queue->controller->receiveMessage(buf.sender, buf.header, buf.msg);

        mq_close(qr);
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
    
    if ((qs = mq_open(dest, Q_FLAGS, Q_PERM, &attr)) >= 0)
    {
        if (mq_send(qs, cast_msg, sizeof(message), 0) < 0)
            std::cerr << "Error: Unable to send message\n";
        else
            std::cerr << "Sent '" << cast_msg << "'i\n";
        mq_close(qs);
    } else {
        std::cerr << "Unable to open queue " << dest << "(write)\n";
    }
}

