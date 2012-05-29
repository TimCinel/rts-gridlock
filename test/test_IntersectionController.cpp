#include "../IntersectionController.h"
#include <iostream>

int main() {
    IntersectionController *controller = new IntersectionController();

    controller->trigger();
    controller->trigger();
    controller->trigger();
    controller->trigger();
}
