/* Queue header. This object contains all the mqueues for a controller.
 *
 * For i2:
 *          /dev/mqueue/i2
 * this is the LISTENING mqueue. It will run on it's own thread once created,
 * and simply run the Read() function.
 *
 *          /net/CONTROLLER.sece-lab.rmit.edu.au/dev/mqueue/centrol
 * this is the SENDING mqueue. This will be the same for all controllers (except
 * central).
 * 
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <mqueue.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/queue.h>
#include "AbstractController.h"

#define MESSAGESIZE 28
#define SENDERSIZE 20
#define Q_FLAGS     O_RDWR | O_CREAT
#define Q_PERM      S_IRUSR | S_IWUSR | S_IROTH

#define MODE_CHANGE    1
#define MODE_REQUEST   2
#define STATUS_NOTIFY  3

#define READ 0
#define WRITE 1

/*used to store messages when passing via the queue*/
typedef struct
{
    int header;
    char sender[SENDERSIZE];
    int msg;
} mq_message;

class Queue
{
    /*instance variables*/
    private:
        pthread_t t_thread;
        char* name;
        struct mq_attr attr;
        AbstractController* controller;

    /*constructors*/
    public:
        Queue(char*, AbstractController*);
        ~Queue();

    /*member functions*/
    public:
        char* get_name();
        struct mq_attr* getAttr();
        void set_name(char* name);
        AbstractController* getController();
};

/*thread functions, cannot be member functions*/
void* read_queue(void* args);
void write_queue(char*, int, char*, int);

#endif

