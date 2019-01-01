# Makefile for the olc Pixel Game Engine example programs

OPENGL_FLAGS = $(shell pkg-config --cflags --libs gl)
PNG_FLAGS = $(shell pkg-config --cflags --libs libpng)
LIB_FLAGS = $(OPENGL_FLAGS) $(PNG_FLAGS)

PROGRAMS = \
	olcExampleProgram \
	OneLoneCoder_PGE_Balls2 \
	OneLoneCoder_PGE_ExtensionTestGFX2D \
	OneLoneCoder_PGE_PathFinding_WaveProp \
	OneLoneCoder_PGE_ShadowCasting2D \
	OneLoneCoder_PGE_SpriteTransforms

ifeq ($(OS),Windows_NT)
PROGRAMS += OneLoneCoder_PGE_SoundTest
else
UNAME := $(shell uname -s)
ifeq ($(UNAME),Linux)
X11_FLAGS = $(shell pkg-config --cflags --libs x11)
LIB_FLAGS += $(X11_FLAGS)
endif
PTHREAD_FLAGS = -lpthread
LIB_FLAGS += $(PTHREAD_FLAGS)
endif

BUILD_CXX_FLAGS = -fpermissive $(LIB_FLAGS) $(CXX_FLAGS)


all: $(PROGRAMS)

clean:
	rm -f $(PROGRAMS)

%: %.cpp
	@echo "Compiling $<"
	$(CXX) -o $@ $(BUILD_CXX_FLAGS) $<

.PHONY: all clean
