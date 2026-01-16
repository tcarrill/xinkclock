CXX = g++

CXXFLAGS = -Wall -Wextra \
           -I/opt/homebrew/include \
           -I/opt/homebrew/include/SDL2
CXXFLAGS += -std=c++17

LDFLAGS = -L/opt/homebrew/lib \
          -lSDL2 -lSDL2_gfx -lm

TARGET = xclock
BUILDDIR = build

# Source files
SRC = \
    main.cpp \
    ../core/xclock.cpp \
    ../render/render_sdl.cpp \
    ../core/trig_table.cpp \
    ../platform/hal_desktop.cpp 

# Map sources to *normalized* object paths
OBJ = \
    build/main.o \
    build/core/xclock.o \
    build/core/trig_table.o \
    build/render/render_sdl.o \
    build/platform/hal_desktop.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

# Explicit, normalized rules (this is the key)
build/main.o: main.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/core/xclock.o: ../core/xclock.cpp
	@mkdir -p build/core
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/core/trig_table.o: ../core/trig_table.cpp
	@mkdir -p build/core
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/render/render_sdl.o: ../render/render_sdl.cpp
	@mkdir -p build/render
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/platform/hal_desktop.o: ../platform/hal_desktop.cpp
	@mkdir -p build/platform
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build $(TARGET)

.PHONY: all clean
