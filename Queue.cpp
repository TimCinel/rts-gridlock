#include "Queue.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

Queue::Queue(char* name)
{

    this->name = name;
    //this->controller = controller;

    //optional placeholders in case attr is brought back
    attr.mq_maxmsg = 100;
    attr.mq_msgsize = MESSAGESIZE;
    attr.mq_flags = 0;

    // Create the thread
    pthread_create (&t_thread, NULL, Read, this);
    
}

Queue::~Queue()
{
    //mq_close(qr);
    //mq_unlink(name);
}

void* Read(void* args)
{
    Queue *queue = (Queue *)args;
    
        struct mq_attr attr;
        mqd_t qr = 0;     // 
        mq_message buf;      // struct to store the message
        char* hamburgler;    // char* for the above struct to pass to mq
        char* name;

        //if ((qr = mq_open(name, O_RDONLY)) != -1)
        //buf = queue->get_buf();
        //qr = queue->get_qr();
        //attr = queue->get_attr();
        name = queue->get_name();

        mq_getattr(qr, &attr);
        hamburgler = (char*)((void*)&buf);
        
            
    if ((qr = mq_open(name, Q_FLAGS, Q_PERM, NULL)) != -1)
    {
        printf("queue created on %d\n", (int)qr);
        while (1)
        {
            //printf ("max. %u msgs, %u bytes; waiting: %u\n",
                    //attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
            while (mq_receive(qr, hamburgler, strlen(hamburgler), NULL) > 0)
            {
                //printf("dequeue: '%s'\n", buf);
                if (buf.header == MODE_CHANGE) {
                    //controller->setFlag(buf.msg);
                }
                else if (buf.header == MODE_REQUEST)
                {
                    // reply with message containing expected mode
                    
                }
                else if (buf.header == STATUS_NOTIFY)
                {
                    // message is an int containing state
                    //mq_q.push()
                    
                }
            }
        } 
    } else {
        perror("err");
    }
    return NULL;
}

void SendMessage(char *dest, mq_message* msg)
{

    mqd_t    qs;                     // queue for sending
    char* cast_msg;

    cast_msg = (char*)((void*)msg);
    
    if ((qs = mq_open(dest, O_RDWR)) != -1)
    {
        if (mq_send(qs, cast_msg, strlen(cast_msg), 0) != -1)
        {
            printf("sent something on %d\n", (int)qs);
        }
        else
        {
            printf("Error: Unable to send message\n");
        }
    } else {
        perror("Unable to open queue - ");
    }
}
