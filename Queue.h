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

typedef struct
{
    int header;
    char sender[SENDERSIZE];
    int msg;
} mq_message;

class Queue
{

    private:
        pthread_t  t_thread;
        pthread_mutex_t mutex;
        char*     name;
        unsigned int simonsConstant;
        struct mq_attr attr;

    public:
        AbstractController* controller;
        /* Constructor:
         * "name" is the name of the local mqueue (i1, i2, i3, central) */
        Queue(char*, AbstractController*);
        
        ~Queue();

        char* get_name() {return name;}
        struct mq_attr* getAttr() {return &attr;}

        void set_name(char* name) {this->name = name;}
        AbstractController* getController() {return controller;}

        void downMutex();
        void upMutex();
        int getSimonsConstant();
        void setSimonsConstant();
        void clearSimonsConstant();
};

void* read_queue(void* args);
void write_queue(char*, int, char*, int);

#endif
