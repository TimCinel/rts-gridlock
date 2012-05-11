#include <iostream>
#include "../Sensor.h"


int main(void) {
    Controller *mock = new Controller();

    for (int i = 0; i < 26; i++) {
        std::cout << "Creating sensor #" << i << "\n";
        Sensor *sensor = new Sensor(mock, 'a' + i, i);
    }

    while(true)
        sleep(5);

}
