#include "../IntersectionController.h"
#include <iostream>
#include <cstdlib>

using namespace ControllerInfo;

/*used to test controller, operations are all performed by the controller
object*/
int main() {
    IntersectionController *controller = new IntersectionController(TRAM,
        "wawaw");

    controller->tick();

    std::cout << "Test Over\n";
	
    delete controller;

    return EXIT_SUCCESS;
}
