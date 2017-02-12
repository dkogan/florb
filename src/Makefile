all: main
.PHONY: main

PROGRAM = florb

CXX := $(shell fltk-config --cxx)
CXXFLAGS := -std=c++11 -Wall -O2 -funroll-loops \
    -DLOCALEDIR=\"$(LOCALEDIR)\" \
    -DRESOURCEDIR=\"$(RESOURCEDIR)\" \
    $(shell fltk-config --use-images --cxxflags) \
    $(shell curl-config --cflags) \
    $(shell pkg-config --cflags tinyxml2) \
    $(shell pkg-config --cflags libgps) \
    $(shell pkg-config --cflags x11) \
    $(shell pkg-config --cflags xpm) \
    $(shell pkg-config --cflags yaml-cpp)

LDFLAGS := \
    $(shell fltk-config --use-images --ldflags) \
    $(shell curl-config --libs) \
    $(shell pkg-config --libs tinyxml2) \
    $(shell pkg-config --libs libgps) \
    $(shell pkg-config --libs x11) \
    $(shell pkg-config --libs yaml-cpp) \
    $(shell pkg-config --libs xpm) \
    -lpthread \
    -lboost_system \
    -lboost_filesystem \
    -lboost_thread

.PHONY: clean install i18nupdate i18ncompile

CXXFLAGS += -I.

# Object files to be combined into the program binary. The last line combines
# fluid and regular source objects and eliminates duplicates for when fluid has
# already generated .cpp files.
OBJS_FLUID := $(wildcard fluid/*.fl)
OBJS_CPP   := $(wildcard *.cpp)
OBJS_RES   := $(wildcard res/*.res)
OBJS       := $(OBJS_FLUID) $(OBJS_CPP) $(OBJS_RES)

# fluid files depend on each other. So I generate ALL of them before compiling
# any of them
$(OBJS_FLUID:.fl=.o): $(OBJS_FLUID:.fl=.hpp)

version.hpp:
	./mkversionhpp.sh

%.cpp %.hpp: %.fl
	fluid -c -o $*.cpp -h $*.hpp $<

%.o: %.res
	$(LD) -r -b binary -o $@ $<

main: $(OBJS_CPP:.cpp=.o) $(OBJS_FLUID:.fl=.o) $(OBJS_RES:.res=.o)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)
