CXXFLAGS = -std=c++0x -I lib -I src 

OBJS =		src/Capit2.o 

LIBS +=		-lpcap
LIBS +=		-lrt
LIBS +=		-lboost_system
LIBS +=		-lboost_regex
LIBS +=		-lboost_thread

TARGET =	capit

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS) 

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
