#include "../IntersectionController.h"
#include <iostream>

using namespace ControllerInfo;

int main() {
    IntersectionController *controller = new IntersectionController();

    for (int i = 0; i < T_STARTUP; i++) 
        controller->tick();

    controller->setFlag(COMMAND_MODE);
    controller->setFlag(CMD_EW_STRAIGHT);

    for (int i = 0; i < T_NS_CLEAR; i++) 
        controller->tick();

    std::cout << "Test Over\n";

	
    delete controller;
}
