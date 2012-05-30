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

#define MESSAGESIZE 40
#define Q_FLAGS     O_RDWR | O_CREAT
#define Q_PERM      S_IRUSR | S_IWUSR | S_IROTH

#define MODE_CHANGE    1
#define MODE_REQUEST   2
#define STATUS_NOTIFY  3

struct mq_Message
{
    int header;
    char *sender;
    int msg;
};

class Queue
{

    private:
        //mqd_t    qr;                     // queue for receiving
        //queue<int> mq_q;               // empty queue of messages used for 
        pthread_t  t_thread;
        //mq_Message* buf;
        char*     name;
       // Controller* controller;

        struct mq_attr attr;

    public:
        /* Constructor:
         * "name" is the name of the local mqueue (i1, i2, i3, central) */
        Queue(char* name/*, Controller* controller*/);
        
        ~Queue();

        char* get_name() {return name;}
        /*mqd_t get_qr() {return qr;}
        mq_Message* get_buf() {return buf;}
        mq_attr get_attr() {return attr;}*/

        void set_name(char* name) {this->name = name;}
};

void* Read(void* args);

void SendMessage(char *dest, mq_Message* msg);


#endif
