CPP=g++
AR=ar

LIB_SOURCES=$(shell find src -name "*.cpp")
LIB_OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))

DEMO_SOURCES=demo_shell.cpp
DEMO_OBJECTS=$(patsubst %.cpp,%.o,$(DEMO_SOURCES))

CPP_FLAGS=-g -c -std=c++11 -Wall -Iinclude
LD_FALGS=
AR_FLAGS=rc

all: demo_shell lib

demo_shell: $(DEMO_OBJECTS) 
	@echo Linking with $? ...
	$(CPP) $(LD_FALGS) $? -o $@

lib: $(OBJECTS)
	@echo Building libcrnepl ...
	$(AR) $(AR_FLAGS) libcrnepl.a $(OBJECTS)

.cpp.o:
	@echo Compiling $< ...
	$(CPP) $(CPP_FLAGS) $< -o $@

.PHONY:clean
clean:
	$(RM) $(OBJECTS) demo_shell.o demo_shell libcrnepl.a

