TARGET		:= rts
SRCS		:= $(wildcard *.cpp)
OBJS		:= ${SRCS:.cpp=.o}

CC     		:= g++
CCFLAGS		:= -O0 -g -Wall -pedantic
LDFLAGS		:= 
TESTFLAGS	:= -DMOCK_OBJECTS

${TARGET} : ${OBJS}
	${CC} ${LDFLAGS} -o $@ $^

${OBJS}: %.o: %.cpp
	${CC} ${CCFLAGS} -o $@ -c $< 

test_sensor: test/test_Sensor.cpp test/mock_Controller.h
	${CC} ${CCFLAGS} ${TESTFLAGS} test/test_Sensor.cpp test/mock_Controller.h Sensor.cpp -o test_sensor

test_lighthandler: test/test_LightHandler.cpp 
	${CC} ${CCFLAGS} ${TESTFLAGS} test/test_LightHandler.cpp -o test_lighthandler

test_intersectioncontroller: test/test_IntersectionController.cpp LightHandler.cpp Sensor.cpp
	${CC} ${CCFLAGS} ${TESTFLAGS} AbstractController.cpp IntersectionController.cpp test/test_IntersectionController.cpp LightHandler.cpp Sensor.cpp -o test_intersectioncontroller -lpthread

test_intersectionsensor: test/test_IntersectionController.cpp IntersectionController.cpp  LightHandler.cpp Sensor.cpp
	${CC} ${CCFLAGS} AbstractController.cpp IntersectionController.cpp test/test_IntersectionSensor.cpp LightHandler.cpp Sensor.cpp -o test_intersectionsensor

test_queue: test/test_Queue.cpp Queue.cpp Queue.h
	${CC} ${CCFLAGS} ${TESTFLAGS} test/test_Queue.cpp Queue.cpp -o test_queue -lpthread


clean:
	rm -rf *.o *.out test_* ${TARGET}
