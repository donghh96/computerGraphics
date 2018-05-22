HEADERS = -I/usr/local/Cellar/glfw/3.2.1/include  -I/usr/local/Cellar/glm/include -I./include/
LIBS = -L/usr/local/Cellar/glfw/3.2.1/lib/ -lglfw -framework OpenGL

BUILD = ./obj
SRC = ./src
CC = g++ -O3

ASSIGNMENT = main

${ASSIGNMENT}: ${BUILD}/${ASSIGNMENT}.o ${BUILD}/shader.o ${BUILD}/utils.o ${BUILD}/image.o ${BUILD}/model.o
	${CC} ${BUILD}/${ASSIGNMENT}.o ${BUILD}/shader.o ${BUILD}/utils.o ${BUILD}/image.o ${BUILD}/model.o -o ${ASSIGNMENT} ${LIBS}

${BUILD}/${ASSIGNMENT}.o: ${SRC}/${ASSIGNMENT}.cpp
	${CC} -c ${SRC}/${ASSIGNMENT}.cpp -o ${BUILD}/${ASSIGNMENT}.o ${HEADERS}

${BUILD}/shader.o: ${SRC}/shader.cpp
	${CC} -c ${SRC}/shader.cpp -o ${BUILD}/shader.o ${HEADERS}

${BUILD}/utils.o: ${SRC}/utils.cpp
	${CC} -c ${SRC}/utils.cpp -o ${BUILD}/utils.o ${HEADERS}

${BUILD}/model.o: ${SRC}/model.cpp
	${CC} -c ${SRC}/model.cpp -o ${BUILD}/model.o ${HEADERS}

${BUILD}/image.o: ${SRC}/image.cpp
	${CC} -c ${SRC}/image.cpp -o ${BUILD}/image.o ${HEADERS}

clean:
	rm -f ${BUILD}/*.o
	rm -f ${ASSIGNMENT}
