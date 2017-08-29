UNAME	:=	$(shell	uname)
CC=g++	#	was	4.2
#CFLAGS=-w	-O3	-mmacosx-version-min=10.9
OBJS	=	audiodecoderbase.o	audiodecodercoreaudio.o	audiodecodermediafoundation.o
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
LFLAGS	=	$(DEBUG)	-lm	-lstdc++	-framework	Cocoa	-framework	CoreAudio	-framework	AudioToolbox	-framework	AudioUnit	-framework	Carbon	#-lportaudio	-L./	#	-L./	#-pthread
endif

libaudiodecoder.a:	audiodecoder.o
	ar	-d	-rsv	libaudiodecoder.a	audiodecoder.o

audiodecoder.o:	audiodecoderbase.o	audiodecodercoreaudio.o	audiodecodermediafoundation.o
	ld	-r	audiodecoderbase.o	audiodecodercoreaudio.o	audiodecodermediafoundation.o	-o	audiodecoder.o

audiodecoderbase.o:	audiodecoderbase.cpp
	$(CC)	$(CFLAGS)	audiodecoderbase.cpp

audiodecodercoreaudio.o:	audiodecodercoreaudio.cpp
	$(CC)	$(CFLAGS)	audiodecodercoreaudio.cpp

audiodecodermediafoundation.o:	audiodecodermediafoundation.cpp
	$(CC)	$(CFLAGS)	audiodecodermediafoundation.cpp
