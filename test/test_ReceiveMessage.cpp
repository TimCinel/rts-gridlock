#include "../Queue.h"
#include "../IntersectionController.h"
#include "../RemoteController.h"

#include <cstring>

using namespace ControllerInfo;
using namespace RemoteInfo;

/*test operations performed using messages. this is used while an intersection
is running, from the command line, to send manual messages*/
int main(int argsc, char **argsv)
{
    /*test the message header, possible values are stored in the *Info
    namespace*/
    int header;
    if (strcmp("SET_CONTROLLER_FLAG", argsv[2]) == 0)
        header = SET_CONTROLLER_FLAG;
    else if (strcmp("CLEAR_CONTROLLER_FLAG", argsv[2]) == 0)
        header = CLEAR_CONTROLLER_FLAG;
    else if (strcmp("NOTIFY_STATE", argsv[2]) == 0)
        header = NOTIFY_STATE;
    else if (strcmp("REQUEST_MODE", argsv[2]) == 0)
        header = REQUEST_MODE;

    /*find the message value from the Mode namespace*/
    int message;
    for (int i = 0; i < CONTROLLER_MODE_SENTINEL; i++)
    {
        if (strcmp(argsv[3], controllerFlagNames[i]) == 0)
        {
            message = i;
            break;
        }
    }

    /*send the message*/
    write_queue(argsv[1], header, "", message);
}

