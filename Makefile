TARGET		:= rts
SRCS		:= Sensor.cpp
TESTSRCS	:= test/test_Sensor.cpp test/mock_Controller.h
OBJS		:= ${SRCS:.cpp=.o}


CC     		:= g++
CCFLAGS		:= -O0 
LDFLAGS		:= 
TESTFLAGS	:= -DMOCK_OBJECTS

${TARGET} : ${OBJS}
	${CC} ${LDFLAGS} -o $@ $^

${OBJS}: %.o: %.cpp
	${CC} ${CCFLAGS} -o $@ -c $< 

test_sensor: test/test_Sensor.cpp test/mock_Controller.h
	${CC} ${CCFLAGS} ${TESTFLAGS} ${SRCS} test/test_Sensor.cpp test/mock_Controller.h -o test_sensor

clean:
	rm -rf *.o *.out test_* ${TARGET}
