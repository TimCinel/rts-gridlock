#include "GridLock.h"

using namespace std;
using namespace ControllerInfo;


using namespace GridLock;

int main(int argc, char **argv) {

    if (argc != 3)
    {
        std::cout << "Invalid arguments\n";
        exit(-1);
    }

    char *type = argv[1];
    char *name = argv[2];


    //used when creating RemoteController and IntersectionController
    pthread_t thread;

    if (strcmp(type, "central") == 0)
    {

        //create monitors to monitor intersection instances
        remotes.push_back(new RemoteController(string(string(name) + "1")));
        remotes.push_back(new RemoteController(string(string(name) + "2")));
        remotes.push_back(new RemoteController(string(string(name) + "3")));


        for (unsigned int i = 0; i < remotes.size(); i++)
        {
            //create a thread for each remote
            if (pthread_create(&thread, NULL, remoteRunner, remotes[i]) != 0)
            {
                //thread's resources will be released as soon as it closes
                pthread_detach(thread);
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
        AbstractController *controller = new IntersectionController(tram, name);

        //set up sensors
        sensors.push_back(new Sensor(controller, '1', TIMER_MODE));
        sensors.push_back(new Sensor(controller, '2', SENSOR_MODE));
        sensors.push_back(new Sensor(controller, '3', COMMAND_MODE));


        //create a thread to listen to STDIN for Sensors
        pthread_create(&thread, NULL, sensorListener, NULL);
        
        //thread's resources will be released as soon as it closes
        pthread_detach(thread);

        //start up the controller (will block)
        controller->tick();

        delete(controller);

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

    do {
        printf("gridlock\n");
        printf("========\n");
        printf("1. Change mode (all intersections)\n");
        printf("2. Configure intersections\n");
        printf("3. Monitor intersections\n");

        std::cin >> option;

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

        option = readInt(0, 3);

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
    cout << "changeMode\n";
}

void GridLock::showIntersections()
{
    cout << "showIntersections\n";
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

    remote->tick();

    return NULL;
}

void *GridLock::sensorListener(void *args) {

    char buff;
    int i;

    while (read(STDIN_FILENO, &buff, 1) > 0 && !quit) {
        for (i = 0; i < sensors.size(); i++) {
            if (write(sensors[i]->getWriteFD(), &buff, 1) != 1)
            {
                cout << "Failed to write to pipe, exiting." << "\n";
                break;
            }
        }

        if (i < sensors.size())
            //
            break;
    }

    //clean up writeFDs
    for (i = 0; i < sensors.size(); i++)
        close(sensors[i]->getWriteFD());
}
