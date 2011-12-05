CXXFLAGS = -std=c++0x -I lib

OBJS =		src/Capit2.o 

LIBS +=		-lpthread
LIBS +=		-lpcap
LIBS +=		-lrt
LIBS +=		/home/issle/Downloads/boost_1_48_0/stage/lib/libboost_system.a
LIBS +=		-lboost_system

TARGET =	capit

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
