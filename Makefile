CPP=g++
AR=ar

SOURCES=$(shell find src -name "*.cpp")
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))

CPP_FLAGS=-g -c -std=c++11 -Wall -Iinclude
LD_FALGS=
AR_FLAGS=rc

demo_shell: demo_shell.o 
	@echo Linking with $(OBJECTS) ...
	$(CPP) $(LD_FALGS) $(OBJECTS) -o $@

lib: $(OBJECTS)
	@echo Building libcrnepl ...
	$(AR) $(AR_FLAGS) libcrnepl.a $(OBJECTS)

.cpp.o:
	@echo Compiling $< ...
	$(CPP) $(CPP_FLAGS) $< -o $@

.PHONY:clean
clean:
	$(RM) $(OBJECTS) demo_shell.o demo_shell libcrnepl.a

