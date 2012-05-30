#include <string>
#include <unistd.h>
#include "../Queue.h"

int main(void) {
    //AbstractController *mock = new AbstractController();
    char* name = "/i1";
    mq_message mes;

    mes.header = MODE_CHANGE;
    mes.sender = "Wayne";
    mes.msg = 0;
    
    SendMessage(name, &mes);
    sleep(1);
    Queue q(name);
    sleep(1);
    SendMessage(name, &mes);
    sleep(1);
}
