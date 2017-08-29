UNAME	:=	$(shell	uname)
CC=g++	#	was	4.2
#CFLAGS=-w	-O3	-mmacosx-version-min=10.9
OBJS	=	music.o	wav.o	image.o	newzpr.o	myMath.o	knnglut.o	clustKnn.o	audiodecoderbase.o	audiodecodercoreaudio.o
#CC	=	g++	-O3
DEBUG	=
CFLAGS	=	-fpermissive	-c	-w	$(DEBUG)	-O3	-mmacosx-version-min=10.9

LFLAGS	=	$(DEBUG)	-lm	-lstdc++

#UNAME	=	$(shell	uname)
ifeq	($(UNAME),Linux)
LFLAGS	=	$(DEBUG)	-lc	-lm	-lstdc++	-pthread	-lGL	-lglut	-lGLU
endif

#	OS	X
ifeq	($(UNAME),	Darwin)
LFLAGS	=	$(DEBUG)	-lm	-lstdc++	-framework	Cocoa	-framework	GLUT	-framework	openGL	-framework	CoreAudio	-framework	AudioToolbox	-framework	AudioUnit	-framework	Carbon	-lportaudio	-laudiodecoder	-L./	#	-L./	#-pthread
endif

music:	$(OBJS)
	$(CC)	$(OBJS)	$(LFLAGS)	-o	music.exe

music.o:	music.cpp
	$(CC)	$(CFLAGS)	music.cpp

wav.o:	wav.cpp	wav.h	../glut2/newzpr.h
	$(CC)	$(CFLAGS)	wav.cpp

myMath.o:	../myMath/myMath.cpp	../myMath/myMath.h
	$(CC)	$(CFLAGS)	../myMath/myMath.cpp

newzpr.o:	../glut2/newzpr.cpp	../glut2/newzpr.h	../knn/clustKnn.h	../knn/knnglut.h	../seg4/seg4.h	../music/wav.h
	$(CC)	$(CFLAGS)	../glut2/newzpr.cpp

image.o:	../image/image.cpp	../image/image.h
	$(CC)	$(CFLAGS)	../image/image.cpp

knnglut.o:	../knn/knnglut.cpp	../knn/knnglut.h	../glut2/newzpr.h
	$(CC)	$(CFLAGS)	../knn/knnglut.cpp

clustKnn.o:	../knn/clustKnn.cpp	../knn/clustKnn.h	../glut2/newzpr.h
	$(CC)	$(CFLAGS)	../knn/clustKnn.cpp

audiodecoderbase.o:	.//audiodecoder/audiodecoderbase.cpp
	$(CC)	$(CFLAGS)	./audiodecoder/audiodecoderbase.cpp

audiodecodercoreaudio.o:	.//audiodecoder/audiodecodercoreaudio.cpp
	$(CC)	$(CFLAGS)	./audiodecoder/audiodecodercoreaudio.cpp
