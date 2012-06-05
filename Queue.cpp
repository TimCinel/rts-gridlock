#include "Queue.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

/*constructor*/
Queue::Queue(char* name, AbstractController* controller)
{
    this->name = name;
    this->controller = controller;

    /*set queue attributes: 100 messages, 28 bytes each*/
    attr.mq_maxmsg = 100;
    attr.mq_msgsize = MESSAGESIZE;
    attr.mq_flags = 0;

    /*create the thread*/
    pthread_create (&t_thread, NULL, read_queue, this);
    std::cerr << "Opened queue " << name << "\n";
}

/*deconstuctor, clean up once the queue is removed*/
Queue::~Queue()
{
    mq_unlink(name);
}

/*accessor*/
char* Queue::get_name()
{
    return name;
}

/*accessor*/
struct mq_attr* Queue::getAttr()
{
    return &attr;
}

/*accessor*/
AbstractController* Queue::getController()
{
    return controller;
}

/*thread function to read items from the queue and send the message request
to the controller*/
void* read_queue(void* args)
{
    Queue *queue = (Queue *)args;

    mqd_t qr;
    mq_message buf;
    char* hamburgler;

    /*pointer to allow the message to be read easily*/
    hamburgler = (char*)((void*)&buf);
            
    while (1)
    {

        /*open the queue, if it cannot be opened then it probably does not
        exist or may just be unused so far*/
        if ((qr = mq_open(queue->get_name(), O_CREAT | O_RDONLY, S_IRUSR,
            queue->getAttr())) < 0)
        {
            std::cerr << "Could not open Queue "<< queue->get_name() <<
                "(read)\n";
            sleep(1);
            continue;
        }

        /*output queue attributes for debugging*/
        std::cerr << queue->getAttr()->mq_maxmsg << " max, " << queue->getAttr()->mq_msgsize << "size, " << queue->getAttr()->mq_curmsgs << " messages";

        /*read message, if there is no message then close file and sleep for
        1 second*/
        if (mq_receive(qr, hamburgler, MESSAGESIZE, NULL) < 0)
        {
            perror("Error");
            mq_close(qr);
            sleep(1);
            continue;
        }

        /*output message details for debugging*/
        std::cerr << "dequeue: " << buf.header << ", " << buf.sender << ", " << buf.msg << "\n";

        /*pass message request to controller*/
        queue->getController()->receiveMessage(buf.sender, buf.header, buf.msg);

        /*close queue*/
        mq_close(qr);
    }

    return NULL;
}

/*thread for writing to the thread, this can be called asynchronously and does
not require a queue object*/
void write_queue(char *dest, int header, char* sender, int msg)
{
    mqd_t qs;
    char* cast_msg;
    mq_message message;
    struct mq_attr attr;

    /*store message in mq_message struct*/
    message.header = header;
    sprintf(message.sender, sender);
    message.msg = msg;

    /*specify queue attributes*/
    attr.mq_maxmsg = 100;
    attr.mq_msgsize = MESSAGESIZE;
    attr.mq_flags = 0;

    /*pointer allows message to be writen easily*/
    cast_msg = (char*)&message;
    
    /*open queue*/
    if ((qs = mq_open(dest, Q_FLAGS, Q_PERM, &attr)) >= 0)
    {
        /*send the message, check for error*/
        if (mq_send(qs, cast_msg, sizeof(message), 0) < 0)
            std::cerr << "Error: Unable to send message\n";
        else
            std::cerr << "Sent '" << cast_msg << "'i\n";

        /*close queue*/
        mq_close(qs);
    }

    /*unable to open queue*/
    else
        std::cerr << "Unable to open queue " << dest << "(write)\n";
}

