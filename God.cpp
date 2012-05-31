#include <iostream>
#include <vector>
#include <string>

#include "RemoteController.h"
#include "IntersectionController.h"

using namespace std;
using namespace ControllerInfo;

void central();
void intersection();

void centralMenu();
void sendCommands();
void monitorIntersections();

void *sensorListener(void *args);
void *remoteRunner(void *args);

std::vector<RemoteController *> remotes;
std::vector<Sensor *> sensors;

int main(int argc, char **argv) {
    centralMenu();

    if (argc != 3)
    {
        std::cout << "Invalid arguments\n";
        exit(-1);
    }

    char *type = argv[1];
    char *name = argv[2];

    

    AbstractController *controller;

    if (strcmp(type, "central") == 0)
    {
        pthread_t remote_thread[3];

        remotes.push_back(new RemoteController(string(string(name) + "1"), 'a'));
        pthread_create(&remote_thread[0], NULL, remoteRunner, remotes.back());

        remotes.push_back(new RemoteController(string(string(name) + "2"), 'A'));
        pthread_create(&remote_thread[1], NULL, remoteRunner, remotes.back());

        remotes.push_back(new RemoteController(string(string(name) + "3"), '0'));
        pthread_create(&remote_thread[2], NULL, remoteRunner, remotes.back());

        for (unsigned int i = 0; i < remotes.size(); i++)
        {
            std::vector<Sensor *> *remoteSensors = remotes[i]->getSensors();

            for (unsigned int j = 0; j < remoteSensors->size(); j++)
            {
                sensors.push_back((*remoteSensors)[j]);
            }
        }
    }

    else if (strcmp(type, "intersection") == 0)
    {
        controller = new IntersectionController(NOTRAM, name);

        sensors.push_back(new Sensor(controller, '1', TIMER_MODE));
        sensors.push_back(new Sensor(controller, '2', SENSOR_MODE));
        sensors.push_back(new Sensor(controller, '3', COMMAND_MODE));

    }
    else if (strcmp(type, "intersection_t") == 0)
    {
        controller = new IntersectionController(TRAM, name);


    } else {
        std::cout << "Invalid type\n";
        exit(-1);
    }

    pthread_t thread;
    pthread_create(&thread, NULL, sensorListener, NULL);

    controller->tick();

}

void centralMenu()
{
    int option;

    do {
        printf("gridlock\n");
        printf("========\n");
        printf("1. Monitor intersections\n");
        printf("2. Send command to intersection\n");
        printf("3. Exit\n");

        std::cin >> option;

        switch (option) {
            case 1:
                monitorIntersections();
                break;
            case 2:
                sendCommands();
                break;
        }

    } while (option != 3);


}

void monitorIntersections()
{
    printf("monitor sections...\n");
}

void sendCommands()
{
    printf("send commands...\n");
}

void *remoteRunner(void *args) {
    RemoteController *remote = (RemoteController *)args;

    remote->tick();

    return NULL;
}

void *sensorListener(void *args) {

    char buff;
    while (read(STDIN_FILENO, &buff, 1) > 0) {
        for (int i = 0; i < sensors.size(); i++) {
            if (write(sensors[i]->getWriteFD(), &buff, 1) != 1)
            {
                std::cout << "Failed to write to pipe. Bailing!\n";
            }
        }
    }
}
