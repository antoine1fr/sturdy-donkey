CC = clang
CXX = clang++
OBJC = clang++
RM = rm -f

TARGET = demo

SRC_DIR = src/
LIB_DIR = lib/

CFLAGS = \
					 -g3 \
					 -Ilib/gl3w \
					 -Wall \
					 -Werror \
					 -pedantic

CXXFLAGS = \
					 -g3 \
					 -Iinclude \
					 -Ilib/gl3w \
					 -Ilib/glm \
					 -std=c++14 \
					 -Wall \
					 -Werror \
					 -pedantic \
					`pkg-config --cflags sdl2`

LDFLAGS = \
					-g3 -O0 \
					 -framework OpenGL \
					`pkg-config --libs sdl2`

C_SOURCES = \
							lib/gl3w/GL/gl3w.c

CXX_SOURCES = \
							$(SRC_DIR)main.cpp

C_OBJECTS=$(C_SOURCES:.c=.o)
CXX_OBJECTS=$(CXX_SOURCES:.cpp=.o)

OBJECTS = \
					$(C_OBJECTS) \
					$(CXX_OBJECTS) \
					$(OBJC_OBJECTS)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

all: $(TARGET)

clean:
	$(RM) $(OBJECTS)

fclean: clean
	$(RM) $(TARGET)

re: fclean all

.PHONY: all clean fclean re

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

%.o: %.cpp
	$(CXX) -o $@ $(CXXFLAGS) -c $<

%.o: %.mm
	$(OBJC) -o $@ $(OBJCFLAGS) -c $<