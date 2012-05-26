#include "Queue.h"

#include <stdio.h>
#include <stdlib.h>

Queue::Queue(char* name, AbstractController* controller) {
    strcpy(this->name, name);
    this->controller = controller;
    
    attr.mq_maxmsg = 100;
    attr.mq_msgsize = MESSAGESIZE;
    attr.mq_flags = 0;

    qr = mq_open(name, Q_FLAGS, Q_PERM, &attr);
    
}

Queue::~Queue() {
    mq_close(qd);
    mq_unlink(name);
}


void Queue::Read() {

	if ((qr = mq_open(name, O_RDONLY)) != -1)
	{
		mq_getattr(qr, &attr);
		printf ("max. %u msgs, %u bytes; waiting: %u\n",
				attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
		while (mq_receive(qr, buf, MESSAGESIZE, NULL) > 0)
		{
			//printf("dequeue: '%s'\n", buf);

			if (strcmp(buf, "CMD_MODE") == 0)
			    controller->setFlag(CMD_MODE);
			else if (strcmp(buf, "TIMER_MODE") == 0)
			    controller->setFlag(TIMER_MODE);
			else if (strcmp(buf, "SENSOR_MODE") == 0)
			    controller->setFlag(SENSOR_MODE);
			else
			    std::cout << "Error! Invalid message received\n";
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
