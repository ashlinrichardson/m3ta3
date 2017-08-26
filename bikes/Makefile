UNAME	:= $(shell uname)
OBJS = bikes.o image.o newzpr.o myMath.o  knnglut.o clustKnn.o rkf45.o
CC = g++ -O3
DEBUG =
CFLAGS =  -fpermissive -c -w $(DEBUG) -O3
LFLAGS =  $(DEBUG) -lm -lstdc++

LFLAGS  = $(DEBUG) -lc -lm -lstdc++
# OS X: special case
ifeq ($(UNAME), Darwin)
LFLAGS =  $(DEBUG) -lm  -lstdc++ -framework Cocoa -framework GLUT -framework openGL
endif

bikes:    $(OBJS)
	$(CC)	$(OBJS)	$(LFLAGS)	-o	bikes.exe

bikes.o:	bikes.cpp	bikes.h
	$(CC)	$(CFLAGS)	bikes.cpp

myMath.o:	../myMath/myMath.cpp	../myMath/myMath.h
	$(CC)	$(CFLAGS)	../myMath/myMath.cpp

newzpr.o:	../glut2/newzpr.cpp	../glut2/newzpr.h ../knn/clustKnn.h ../knn/knnglut.h ../seg3/seg3.h
	$(CC)	$(CFLAGS)	../glut2/newzpr.cpp

image.o:	../image/image.cpp	../image/image.h
	$(CC)	$(CFLAGS)	../image/image.cpp

knnglut.o: ../knn/knnglut.cpp	../knn/knnglut.h ../glut2/newzpr.h
	$(CC)	$(CFLAGS)	../knn/knnglut.cpp

clustKnn.o:	../knn/clustKnn.cpp	../knn/clustKnn.h ../glut2/newzpr.h
	$(CC)	$(CFLAGS)	../knn/clustKnn.cpp

rkf45.o:	../rkf45/rkf45.cpp	../rkf45/rkf45.h
	$(CC)	$(CFLAGS)	../rkf45/rkf45.cpp
