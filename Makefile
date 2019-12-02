CPP=g++

SOURCES=$(shell find . -name "*.cpp")
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))

CPP_FLAGS=-g -c -std=c++11 -Iinclude
LD_FALGS=

demo_shell: $(OBJECTS)
	@echo Linking with $(OBJECTS) ...
	$(CPP) $(LD_FALGS) $(OBJECTS) -o $@

.cpp.o:
	@echo Compiling $< ...
	$(CPP) $(CPP_FLAGS) $< -o $@

.PHONY:clean
clean:
	$(RM) $(OBJECTS) demo_shell

