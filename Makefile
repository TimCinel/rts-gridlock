TARGET		:= rts
SRCS		:= $(wildcard *.cpp)
OBJS		:= ${SRCS:.cpp=.o}


CC     		:= g++
CCFLAGS		:= -O0 -g
LDFLAGS		:= 
TESTFLAGS	:= -DMOCK_OBJECTS

${TARGET} : ${OBJS}
	${CC} ${LDFLAGS} -o $@ $^

${OBJS}: %.o: %.cpp
	${CC} ${CCFLAGS} -o $@ -c $< 

test_sensor: test/test_Sensor.cpp test/mock_Controller.h
	${CC} ${CCFLAGS} ${TESTFLAGS} ${SRCS} test/test_Sensor.cpp test/mock_Controller.h -o test_sensor

test_lighthandler: test/test_LightHandler.cpp 
	${CC} ${CCFLAGS} ${TESTFLAGS} ${SRCS} test/test_LightHandler.cpp -o test_lighthandler

test_intersectioncontroller: test/test_IntersectionController.cpp IntersectionController.cpp LightHandler.cpp Sensor.cpp
	${CC} ${CCFLAGS} ${TESTFLAGS} AbstractController.cpp IntersectionController.cpp test/test_IntersectionController.cpp LightHandler.cpp Sensor.cpp -o test_intersectioncontroller

test_intersectionsensor: test/test_IntersectionController.cpp IntersectionController.cpp  LightHandler.cpp Sensor.cpp
	${CC} ${CCFLAGS} AbstractController.cpp IntersectionController.cpp test/test_IntersectionSensor.cpp LightHandler.cpp Sensor.cpp -o test_intersectionsensor

clean:
	rm -rf *.o *.out test_* ${TARGET}
