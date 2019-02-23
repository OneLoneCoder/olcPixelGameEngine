# Example makefile, could use some descriptive header documentation.

PGE_PROJECTS = Balls2 ExtensionTestGFX2D PathFinding_WaveProp \
	PolygonCollisions1 ShadowCasting2D SoundTest          \
	SpriteTransforms

PROGRAMS = olcExampleProgram $(PGE_PROJECTS:%=OneLoneCoder_PGE_%)

LDLIBS = -lX11 -lGL -lpthread -lpng
CPPFLAGS = -g
all: $(PROGRAMS)

# This is some black magic, but it essentualy does what u think it should do.
OneLoneCoder_PGE_SoundTest: -lasound

clean:
	rm -f $(PROGRAMS)

.PHONY: all clean

