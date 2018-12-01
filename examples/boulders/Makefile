CXX = clang++
RM = rm -f

TARGET = boulders

SRC_DIR = src/
LIB_DIR = ../../lib/

CXXFLAGS = \
	-g3 \
	-Iinclude \
	-I../../include \
	-I../../lib/gl3w \
	-I../../lib/glm \
	-I../../lib/tinyobjloader \
	-std=c++14 \
	-Wall \
	-Werror \
	-pedantic \
	`pkg-config --cflags sdl2 SDL2_image`

LDFLAGS = \
	-g3 -O0 \
	-framework OpenGL \
	`pkg-config --libs sdl2 SDL2_image` \
	-L../../ -lengine

CXX_SOURCES = \
							$(SRC_DIR)ResourceLoaderDelegate.cpp \
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

%.o: %.cpp
	$(CXX) -o $@ $(CXXFLAGS) -c $<
