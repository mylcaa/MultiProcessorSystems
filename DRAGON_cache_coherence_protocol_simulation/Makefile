# Compiler
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -Iinc -I/lib/include

# Target
TARGET := dragon_emulator

# Sources
SRC := \
    src/write_buffer.cpp \
    src/ott.cpp \
    src/dragon.cpp \
    src/coherence_controller.cpp \
    src/processor.cpp \
    src/cache.cpp \
    src/node.cpp \
    src/bus.cpp \
    src/memory.cpp \
	src/visual.cpp \
    src/console.cpp \
	src/draw_dragon_fsm.cpp \
    main.cpp

# Libraries
LIBDIRS := -L/lib
LIBS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Default rule
all: $(TARGET)

# One-shot compile + link
$(TARGET):
	$(CXX) $(CXXFLAGS) $(SRC) $(LIBDIRS) $(LIBS) -o $@

# Clean
clean:
	rm -f $(TARGET)

.PHONY: all clean
