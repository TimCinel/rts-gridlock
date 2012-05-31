#include "../IntersectionController.h"
#include <iostream>
#include <cstdlib>

using namespace ControllerInfo;

int main() {
    IntersectionController *controller = new IntersectionController(TRAM, "wawaw");

    //controller->setFlag(COMMAND_MODE);
    //controller->setFlag(CMD_EW_STRAIGHT);

    controller->tick();

    std::cout << "Test Over\n";
	
    delete controller;

    return EXIT_SUCCESS;
}
