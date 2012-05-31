#include <iostream>
#include <vector>

#include "IntersectionRecord.h"
#include "IntersectionController.h"

void central(vector<string> names);
void intersection(string name, int type);
void *sensorListener(vector<intersection> *intersections);

int main(int argc, char *argv) {

#ifdef CENTRAL
    //we're creating the central controller, yay!
    central();
#else
    //we're creating an intersection controller
    intersection();
#endif
}

void central()
{
    vector<RemoteController> remotes;

    char sensorChar = '0';

    intersections.push_back(
            new RemoteController("gridlock1", sensorChar),
            );
    sensorChar += 9;

    char sensorChar = 'a';

    intersections.push_back(
            new RemoteController("gridlock2", sensorChar),
            );

    char sensorChar = 'A';
    intersections.push_back(
            new RemoteController("gridlock3", sensorChar),
            );

}

void intersection(string name)
{
    int type = NOTRAM;
#ifdef TRAM
    type = TRAM;
#endif

}



void *sensorListener(vector<intersection> *intersections) {

    char buff;
    while (read(STDIN_FILENO, &buff, 1) > 0) {
        for (int i = 0; i < intersections->size(); i++)
        {
            vector<Sensor> *sensors = intersections[i]->getSensors();

            for (int j = 0; j < sensors->size(); j++) {
                if (write(sensors[j]->getWriteFD(), &buff, 1) != 1)
                {
                    std::cout << "Failed to write to pipe. Bailing!\n";
                }
            }
        }
    }
}
