# +------------------+
# | Cherryl Makefile |
# +------------------+

OBJS = cherryl.o
SOURCE = cherryl.cpp
OUT = cherryl
FLAGS = -g -c -Wall -std=gnu++11
#Flags for static compilation on Windows and Linux (not Android)
ifneq ($(shell uname -s),Darwin)
ifneq ($(shell uname -o),Android)
	LFLAGS = -static-libgcc -static-libstdc++
endif
endif
#PREFIX is environment variable, but if it is not set, then set default value
ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

all: $(OBJS)
	$(CXX) -g $(OBJS) -o $(OUT) $(LFLAGS)

cherryl.o: cherryl.cpp
	$(CXX) $(FLAGS) $(SOURCE)


clean:
	rm -f cherryl.o cherryl

install: cherryl
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 775 cherryl $(DESTDIR)$(PREFIX)/bin/
	ln -s $(DESTDIR)$(PREFIX)/bin/cherryl $(DESTDIR)$(PREFIX)/bin/chy

uninstall:
	rm $(DESTDIR)$(PREFIX)/bin/cherryl
	rm $(DESTDIR)$(PREFIX)/bin/chy
