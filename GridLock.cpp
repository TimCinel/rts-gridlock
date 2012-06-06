#include "GridLock.h"

using namespace std;
using namespace ControllerInfo;

using namespace GridLock;

int main(int argc, char **argv)
{
    /*test for command line arguments*/
    if (argc != 4)
    {
        std::cout << "Invalid arguments\n";
        exit(-1);
    }

    char *type = argv[1];
    centralName = argv[2];
    intersectionName = argv[3];

    /*used when creating RemoteController and IntersectionController*/
    pthread_t thread;

    /*create central controller*/
    if (strcmp(type, "central") == 0)
    {
        /*create monitors to monitor intersection instances*/
        string centralPrefix = string(centralName);
        string intersectionPrefix = string(intersectionName);

        /*create remote controllers*/
        remotes.push_back(new RemoteController("/c1",
            "/net/gridlock1/dev/mqueue/i1"));
        remotes.push_back(new RemoteController("/c2",
            "/net/gridlock2/dev/mqueue/i2"));
        remotes.push_back(new RemoteController("/c3",
            "/net/gridlock3/dev/mqueue/i3"));

        std::cout << "#1: " << remotes[0]->getIntersectionName() << "\n";
        std::cout << "#2: " << remotes[1]->getIntersectionName() << "\n";
        std::cout << "#3: " << remotes[2]->getIntersectionName() << "\n";

        /*create a thread for each remote*/
        for (unsigned int i = 0; i < remotes.size(); i++)
        {
            std::cerr << "Creating thread for " <<
                remotes[i]->getIntersectionName() << "\n";

            if (pthread_create(&thread, NULL, remoteRunner, remotes[i]) != 0)
            {
                /*thread's resources will be released as soon as it closes*/
                pthread_detach(thread);
                std::cerr << "Detaching " <<
                    remotes[i]->getIntersectionName() << "\n";
            }
        }

        /*show the menu to the user (will block)*/
        centralMenu();

        /*clean up*/
        quit = true;
        while (!remotes.empty())
        {
            delete remotes.back();
            remotes.pop_back();
        }

    }

    /*create intersection controller*/
    else if (strncmp(type, "intersection", strlen("intersection")) == 0)
    {
        /*if the user specified "intersectiont" then it's a tram intersection*/
        int tram = ('t' == type[strlen("intersection")] ? TRAM : NOTRAM);

        /*create the IntersectionController*/
        AbstractController *controller = new IntersectionController(tram, 
            string(centralName), string(intersectionName));

        /*set up sensors*/
        sensors.push_back(new Sensor(controller, 'q', SEN_NS_PED));
        sensors.push_back(new Sensor(controller, 'w', SEN_NS_STRAIGHT));
        sensors.push_back(new Sensor(controller, 'e', SEN_TRAM));
        sensors.push_back(new Sensor(controller, 'a', SEN_EW_PED));
        sensors.push_back(new Sensor(controller, 's', SEN_EW_STRAIGHT));
        sensors.push_back(new Sensor(controller, 'd', SEN_EW_RIGHT));

        /*create a thread to listen to STDIN for Sensors, thread's resources
        will be released as soon as it closes*/
        pthread_create(&thread, NULL, sensorListener, NULL);
        pthread_detach(thread);

        /*start up the controller (will block)*/
        controller->tick();

        /*delete(controller);*/

        /*clean up*/
        quit = 1;
        while (!sensors.empty())
        {
            delete sensors.back();
            sensors.pop_back();
        }
    }

    /*invalid type*/
    else
    {
        std::cout << "Invalid type\n";
        exit(-1);
    }
}

/*main menu loop*/
void GridLock::centralMenu()
{
    int option;

    /*loop forever*/
    while (1) 
    {
        /*print menu*/
        printf("gridlock\n");
        printf("========\n");
        printf("1. Change mode (all intersections)\n");
        printf("2. Configure intersections\n");
        printf("3. Monitor intersections\n");
        printf("-----------\n\n");

        /*read option and call appropriate sub-menu*/
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
    }
}

/*read a single integer from stdin, validate input*/
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


/*sub menu to change a single controller to a new mode*/
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
            /*didn't select a command*/
            if ((command = selectCommand()) < 1)
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

/*set a controller to a new mode when in command mode*/
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

/*list the available intersections*/
void GridLock::showIntersections()
{
    printf("\n\n\n");
    printf("remote intersections\n");
    printf("--------------------\n");
    for (unsigned int i = 0 ; i < remotes.size(); i++)
        printf("%d. %s\n", i + 1, remotes[i]->getIntersectionName());
    printf("--------------------\n\n");

    int option = readInt(1, remotes.size());

    if (option > 0)
        intersectionMenu(remotes[(option - 1) % remotes.size()]);
}

/*unused*/
void GridLock::monitorIntersections()
{
    cout << "monitorIntersections\n";
}

/*allows the changing of mode or sequence for an already determined
intersection*/
void GridLock::intersectionMenu(RemoteController *remote)
{
    printf("\n\n\n");
    printf("%s\n", remote->getIntersectionName());
    printf("--------------------\n");
    printf("1. Change mode\n");
    printf("2. Change sequence\n");
    printf("--------------------\n\n");

    int option = readInt(1, 2);

    if (1 == option)
        changeMode(remote);
    else if (2 == option)
        changeSequence(remote);

}

/*imodify the sequence of a pre-determined controller*/
void GridLock::changeSequence(RemoteController *remote)
{
    printf("\n\n\n");
    printf("Change Sequence for %s\n", remote->getIntersectionName());
    printf("---------------------------\n");

    static int range = CONTROLLER_COMMAND_SENTINEL -
                       CONTROL_SEQUENCE_SENTINEL - 1;
    static int offset = CONTROL_SEQUENCE_SENTINEL + 1;

    int i, j, option;

    do
    {
        for (i = 0; i < range; i++) 
            printf("%d. Set %s\n", i + 1, controllerFlagNames[i + offset]);

        printf("- - - - - - - - - - \n");

        for (j = 0; j < range; j++) 
            printf("%d. Clear %s\n", i + j + 1, controllerFlagNames[j +
                offset]);

        printf("- - - - - - - - - - \n");

        printf("%d. Done\n", j + i + 1);
        printf("---------------------------\n\n");

        option = readInt(1, i + j);

        if (option > 0 && option <= i) 
            remote->setFlag(offset + option - 1);
        else if (option > i)
            remote->clearFlag(offset + option - 1 - i);

    } while (option > 0);
}

/*thread to run remote controller*/
void *GridLock::remoteRunner(void *args) {
    RemoteController *remote = (RemoteController *)args;

    std::cerr << "new runner thread for " << remote->getIntersectionName()
        << "\n";

    while (true)
    {
        sleep(1);
        remote->trigger();
    }

    return NULL;
}

/*thread to listen to each of the keys used to trigger the sensors in the
intersection controllers*/
void *GridLock::sensorListener(void *args)
{
    char buff;
    unsigned int i;

    while (read(STDIN_FILENO, &buff, 1) > 0 && !quit)
    {
        for (i = 0; i < sensors.size(); i++)
        {
            /*there was a problem writing out to a pipe*/
            if (write(sensors[i]->getWriteFD(), &buff, 1) != 1)
            {
                cout << "Failed to write to pipe, exiting." << "\n";
                break;
            }
        }

        /*break must have been called*/
        if (i < sensors.size())
            break;
    }

    /*clean up writeFDs*/
    for (i = 0; i < sensors.size(); i++)
        close(sensors[i]->getWriteFD());

    return NULL;
}

