#include "GridLock.h"

using namespace std;
using namespace ControllerInfo;


using namespace GridLock;

int main(int argc, char **argv) {

    if (argc != 4)
    {
        std::cout << "Invalid arguments\n";
        exit(-1);
    }

    char *type = argv[1];
    centralName = argv[2];
    intersectionName = argv[3];


    //used when creating RemoteController and IntersectionController
    pthread_t thread;

    if (strcmp(type, "central") == 0)
    {

        //create monitors to monitor intersection instances
        string centralPrefix = string(centralName);
        string intersectionPrefix = string(intersectionName);

        remotes.push_back(new RemoteController(centralPrefix + "2", intersectionPrefix + "2"));
        remotes.push_back(new RemoteController(centralPrefix + "3", intersectionPrefix + "3"));
        remotes.push_back(new RemoteController(centralPrefix + "1", intersectionPrefix + "1"));


        std::cout << "#1: " << remotes[0]->getIntersectionName() << "\n";
        std::cout << "#1: " << remotes[1]->getIntersectionName() << "\n";
        std::cout << "#3: " << remotes[2]->getIntersectionName() << "\n";

        for (unsigned int i = 0; i < remotes.size(); i++)
        {
            //create a thread for each remote
            std::cout << "Creating thread for " << remotes[i]->getIntersectionName() << "\n";

            if (pthread_create(&thread, NULL, remoteRunner, remotes[i]) != 0)
            {
                //thread's resources will be released as soon as it closes
                pthread_detach(thread);
                std::cout << "Detaching " << remotes[i]->getIntersectionName() << "\n";
            }
        }

        //show the menu to the user (will block)
        centralMenu();

        quit = true;

        while (!remotes.empty())
        {
            delete remotes.back();
            remotes.pop_back();
        }

    }
    else if (strncmp(type, "intersection", strlen("intersection")) == 0)
    {
        //if the user specified "intersectiont" then it's a tram intersection 
        int tram = ('t' == type[strlen("intersection")] ? TRAM : NOTRAM);

        //create the IntersectionController
        AbstractController *controller = 
            new IntersectionController(tram, 
                                       string(centralName),
                                       string(intersectionName)
                                      );

        //set up sensors
        sensors.push_back(new Sensor(controller, 'q', SEN_NS_PED));
        sensors.push_back(new Sensor(controller, 'w', SEN_NS_STRAIGHT));
        sensors.push_back(new Sensor(controller, 'e', SEN_TRAM));

        sensors.push_back(new Sensor(controller, 'a', SEN_EW_PED));
        sensors.push_back(new Sensor(controller, 's', SEN_EW_STRAIGHT));
        sensors.push_back(new Sensor(controller, 'd', SEN_EW_RIGHT));

        //create a thread to listen to STDIN for Sensors
        pthread_create(&thread, NULL, sensorListener, NULL);
        
        //thread's resources will be released as soon as it closes
        pthread_detach(thread);

        //start up the controller (will block)
        controller->tick();

        //delete(controller);

        quit = 1;
        while (!sensors.empty())
        {
            delete sensors.back();
            sensors.pop_back();
        }


    } else {
        std::cout << "Invalid type\n";
        exit(-1);
    }


}

void GridLock::centralMenu()
{
    int option;

    do 
    {
        printf("gridlock\n");
        printf("========\n");
        printf("1. Change mode (all intersections)\n");
        printf("2. Configure intersections\n");
        printf("3. Monitor intersections\n");
        printf("-----------\n\n");

        option = readInt(0, 3);

        switch (option) {
            case 1:
                changeMode(NULL);
                break;
            case 2:
                showIntersections();
                break;
            case 3:
                monitorIntersections();
                break;
        }

    } while (true);


}

int GridLock::readInt(int min, int max)
{
    char line[LINE_SIZE];
    char *linePtr = (char *)line;

    if (fgets(line, LINE_SIZE, stdin))
    {
        int value =  (int)strtol(linePtr, NULL, 10);

        if (min <= value && max >= value)
            return value;
    }

    return -1;
}


void GridLock::changeMode(RemoteController *remote)
{
    printf("\n\n\n");
    printf("change mode\n");
    printf("-----------\n");
    printf("1. Timer mode\n");
    printf("2. Sensor mode\n");
    printf("3. Command mode\n");
    printf("-----------\n\n");

    int option = readInt(1, 3);
    int mode, command;

    switch (option)
    {
        case 1:
            mode = TIMER_MODE;
            break;
        case 2:
            mode = SENSOR_MODE;
            break;
        case 3:
            mode = COMMAND_MODE;
            if ((command = selectCommand()) < 1)
                //didn't select a command
                return;
            else
                if (NULL != remote)
                    remote->setFlag(command);
                else
                    for (unsigned int i = 0; i < remotes.size(); i++)
                        remotes[i]->setFlag(command);


            break;
        default:
            printf("Unrecognised option.\n");
            return;
    }

    if (NULL != remote)
        remote->setFlag(mode);
    else
        for (unsigned int i = 0; i < remotes.size(); i++)
            remotes[i]->setFlag(mode);
}

int GridLock::selectCommand()
{
    static const unsigned int range = CONTROLLER_FLAG_SENTINEL - 
                                      CONTROLLER_COMMAND_SENTINEL;
    static const unsigned int offset = CONTROLLER_COMMAND_SENTINEL;

    std::cout << "range: " << range << ", offset: " << offset << "\n";

    printf("\n\n\n");
    printf("select command\n");
    printf("--------------\n");
    for (unsigned int i = 1; i < range; i++)
        printf("%d. %s\n", i, controllerFlagNames[offset + i]);
    printf("--------------\n\n");
    
    int option = readInt(1, range);

    if (option > 0)
        return offset + option;
    else
        return option;
}

void GridLock::showIntersections()
{
    printf("\n\n\n");
    printf("change mode\n");
    printf("-----------\n");
    for (unsigned int i = 0 ; i < remotes.size(); i++)
        printf("%d. %s\n", i + 1, remotes[i]->getIntersectionName());
    printf("-----------\n\n");

    int option = readInt(1, 3);
}

void GridLock::monitorIntersections()
{
    cout << "monitorIntersections\n";
}


void GridLock::intersectionMenu(RemoteController *remote)
{
    cout << "intersectionMenu\n";
}

void GridLock::changeSequence(RemoteController *remote)
{
    cout << "changeSequence\n";
}


void *GridLock::remoteRunner(void *args) {
    RemoteController *remote = (RemoteController *)args;

    std::cout << "new runner thread for " << remote->getIntersectionName() << "\n";
    while (true) {
        sleep(1);
        remote->trigger();
    }

    return NULL;
}

void *GridLock::sensorListener(void *args) {

    char buff;
    unsigned int i;

    while (read(STDIN_FILENO, &buff, 1) > 0 && !quit) {
        for (i = 0; i < sensors.size(); i++) {
            if (write(sensors[i]->getWriteFD(), &buff, 1) != 1)
            {
                //there was a problem writing out to a pipe
                cout << "Failed to write to pipe, exiting." << "\n";
                break;
            }
        }

        if (i < sensors.size())
            //break must have been called
            break;
    }

    //clean up writeFDs
    for (i = 0; i < sensors.size(); i++)
        close(sensors[i]->getWriteFD());

    return NULL;
}
