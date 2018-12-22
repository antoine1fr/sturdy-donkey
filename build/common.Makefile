TARGET = $(TARGET_PREFIX)engine$(TARGET_SUFFIX)

SRC_DIR = src/
LIB_DIR = lib/

CFLAGS += \
					 -g3 \
					 -Ilib/gl3w \
					 -Wall \
					 -Werror \
					 -pedantic

CXXFLAGS += \
					 -g3 \
					 -Iinclude \
					 -Ilib/gl3w \
					 -std=c++14 \
					 -Wall \
					 -Werror \
					 -pedantic \
					`pkg-config --cflags sdl2 SDL2_image`

C_SOURCES = \
							lib/gl3w/GL/gl3w.c

CXX_SOURCES = \
							$(SRC_DIR)Game.cpp \
							$(SRC_DIR)GameManager.cpp \
							$(SRC_DIR)Scene.cpp \
							$(SRC_DIR)Buffer.cpp \
							$(SRC_DIR)BufferPool.cpp \
							$(SRC_DIR)render/AMaterial.cpp \
							$(SRC_DIR)render/gl/Material.cpp \
							$(SRC_DIR)render/Window.cpp \
							$(SRC_DIR)render/ResourceManager.cpp \
							$(SRC_DIR)render/gl/ResourceManager.cpp \
							$(SRC_DIR)render/TextureMaterialSlot.cpp \
							$(SRC_DIR)render/CommandBucket.cpp \
							$(SRC_DIR)render/gl/Driver.cpp \
							$(SRC_DIR)render/gl/Mesh.cpp \
							$(SRC_DIR)render/gl/Texture.cpp \
							$(SRC_DIR)render/Mesh.cpp \
							$(SRC_DIR)render/DeferredRenderer.cpp

C_OBJECTS=$(C_SOURCES:.c=.o)
CXX_OBJECTS=$(CXX_SOURCES:.cpp=.o)

OBJECTS = \
					$(C_OBJECTS) \
					$(CXX_OBJECTS) \
					$(OBJC_OBJECTS)

$(TARGET): $(OBJECTS)
	$(AR) -r -s $(TARGET) $(OBJECTS) 

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