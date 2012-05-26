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
#include "AbstractController.h"

#define MESSAGESIZE 40
#define Q_FLAGS     O_RDWR | O_CREAT
#define Q_PERM      S_IRUSR | S_IWUSR | S_IROTH

#define NAMESIZE 20

class Queue {

    private:
       mqd_t    qd;
       mqd_t    qr;                     // queue for receiving
       char     buf[MESSAGESIZE];       //message being passed
       char     name[NAMESIZE];
       AbstractController* controller;

       struct mq_attr attr;

    public:
        /* Constructor:
         * "name" is the name of the local mqueue (i1, i2, i3, central) */
        Queue(char* name);
        
        ~Queue();


        /* reads the queue qd, requires it's own thread. This should probably
         * just be called by the constructor, */
        void Read();

        /* this is the method used by intersection controllers, dest will always
         * be central */
        void SendMessage(char *msg);

        /* this is used by central controller, dest can be i1, i2, i3 */
        void SendMessage(char *dest, char *msg);
        
};

#endif
