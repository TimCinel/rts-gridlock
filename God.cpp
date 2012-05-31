#include <iostream>
#include <vector>
#include <string>

#include "RemoteController.h"
#include "IntersectionController.h"

using namespace ControllerInfo;

void central();
void intersection();
void *sensorListener(void *args);

std::vector<RemoteController *> remotes;
std::vector<Sensor *> sensors;

int main(int argc, char **argv) {

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

        remotes.push_back(new RemoteController((string(name) + "1").c_str(), 'a'));
        pthread_create(&remote_thread[0], NULL, remoteRunner, remotes.back());

        remotes.push_back(new RemoteController((string(name) + "2").c_str(), 'A'));
        pthread_create(&remote_thread[1], NULL, remoteRunner, remotes.back());

        remotes.push_back(new RemoteController((string(name) + "3").c_str(), '0'));
        pthread_create(&remote_thread[2], NULL, remoteRunner, remotes.back());

        for (int i = 0; i < remotes.size(); i++)
        {
            std::vector<Sensor *> *remoteSensors = remotes[i].getSensors();

            for (int j = 0; j < remoteSensors->size(); j++)
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

void *remoteRunner(void *args) {
    RemoteController *remote = (RemoteController *)args;

    remote->tick();
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
