CXX=g++
CPPFLAGS=-g -fpermissive -I/usr/include/libusb-1.0 -I../../libpixyusb2/include -I../../arduino/libraries/Pixy2
LDLIBS=../../../../build/libpixyusb2/libpixy2.a -lusb-1.0 -lSDL2

SRCS=display_lines.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: get_lines

clean:
	rm -f *.o display_lines

get_lines: $(OBJS)
	$(CXX) $(LDFLAGS) -o display_lines $(OBJS) $(LDLIBS)
