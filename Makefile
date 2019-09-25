# +------------------+
# | Cherryl Makefile |
# +------------------+

SOURCE = cherryl.cpp
OUT = cherryl
FLAGS = -std=c++11
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

all: 
	$(CXX) -g $(SOURCE) -o $(OUT) $(FLAGS) $(LFLAGS)

clean:
	rm -f cherryl.o cherryl

install: cherryl
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 775 cherryl $(DESTDIR)$(PREFIX)/bin/
	ln -sf $(DESTDIR)$(PREFIX)/bin/cherryl $(DESTDIR)$(PREFIX)/bin/chy

uninstall:
	rm $(DESTDIR)$(PREFIX)/bin/cherryl
	rm $(DESTDIR)$(PREFIX)/bin/chy
