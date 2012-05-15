#include "Queue.h"

#include <stdio.h>
#include <stdlib.h>

Queue::Queue(char* name) {

    attr.mq_maxmsg = 100;
    attr.mq_msgsize = MESSAGESIZE;
    attr.mq_flags = 0;

    qr = mq_open(name, Q_FLAGS, Q_PERM, &attr);
    
}

Queue::~Queue() {
    mq_close(qd);
    mq_unlink(name);
}


void Queue::Read(char *name) {

	if ((qr = mq_open(name, O_RDONLY)) != -1)
	{
		mq_getattr(qr, &attr);
		printf ("max. %u msgs, %u bytes; waiting: %u\n",
				attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
		while (mq_receive(qr, buf, MESSAGESIZE, NULL) > 0)
		{
			printf("dequeue: '%s'\n", buf);
		}
	}
    
}

void Queue::SendMessage(char *msg) {
    
}

void Queue::SendMessage(char *dest, char *msg) {

  /*  if (qd != -1) {
        mq_send(qd, msg, strlen(msg), 0);
    } else {
        printf("error sending\n");
    }*/
        
}
