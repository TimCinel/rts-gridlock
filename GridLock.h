#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <vector>
#include <string>

#include <cstdio>
#include <cstring>

#include "RemoteController.h"
#include "IntersectionController.h"

namespace GridLock
{
    /*functions*/
    void centralMenu();

    void changeMode(RemoteController *remote);
    int selectCommand();
    void showIntersections();
    void monitorIntersections();

    void intersectionMenu(RemoteController *remote);
    void changeSequence(RemoteController *remote);

    int readInt(int min, int max);

    void *sensorListener(void *args);
    void *remoteRunner(void *args);

    /*instance variables/pointers*/
    const char *centralName;
    const char *intersectionName;
    static const int LINE_SIZE = 32;
    static bool quit = 0;

    /*stores remotes (each remote monitors a remote intersection)*/
    static std::vector<RemoteController *> remotes;
    
    /*stores sensors (used for intersections)*/
    static std::vector<Sensor *> sensors;
}

#endif
