CXXFLAGS = -std=c++0x -I lib -I src -I /usr/include/gtkmm-3.0 `pkg-config gtkmm-3.0 --cflags --libs`

OBJS =		src/Capit2.o 

LIBS +=		-lpthread
LIBS +=		-lpcap
LIBS +=		-lrt
LIBS +=		-lboost_system

TARGET =	capit

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS) 

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
