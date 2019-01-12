BASE_TARGET = sturdy-donkey
STATIC_TARGET = $(TARGET_PREFIX)$(BASE_TARGET)$(STATIC_TARGET_SUFFIX)
DYNAMIC_TARGET = $(TARGET_PREFIX)$(BASE_TARGET)$(DYNAMIC_TARGET_SUFFIX)

SRC_DIR = src/
LIB_DIR = lib/

CFLAGS += \
	-g3 \
	-O0 \
	-Ilib/gl3w \
	-Wall \
	-Werror \
	-pedantic

CXXFLAGS += \
	-g3 \
	-O0 \
	-Iinclude \
	-Ilib/gl3w \
	-Ilib/tinyobjloader \
	-Ilib/imgui \
	-std=c++14 \
	-Wall \
	-Werror \
	-pedantic \
	`pkg-config --cflags sdl2 SDL2_image`

LDFLAGS += \
	-shared -fPIC \
	`pkg-config --libs sdl2 SDL2_image`

C_SOURCES = \
	lib/gl3w/GL/gl3w.c

CXX_SOURCES = \
	lib/imgui/imgui.cpp \
	lib/imgui/imgui_draw.cpp \
	lib/imgui/imgui_widgets.cpp \
	$(SRC_DIR)Buffer.cpp \
	$(SRC_DIR)BufferPool.cpp \
	$(SRC_DIR)Game.cpp \
	$(SRC_DIR)GameManager.cpp \
	$(SRC_DIR)MeshLoader.cpp \
	$(SRC_DIR)Scene.cpp \
	$(SRC_DIR)StackAllocator.cpp \
	$(SRC_DIR)render/AMaterial.cpp \
	$(SRC_DIR)render/CommandBucket.cpp \
	$(SRC_DIR)render/DebugHudRenderer.cpp \
	$(SRC_DIR)render/DeferredRenderer.cpp \
	$(SRC_DIR)render/Mesh.cpp \
	$(SRC_DIR)render/ResourceManager.cpp \
	$(SRC_DIR)render/TextureMaterialSlot.cpp \
	$(SRC_DIR)render/Window.cpp \
	$(SRC_DIR)render/gl/Driver.cpp \
	$(SRC_DIR)render/gl/Material.cpp \
	$(SRC_DIR)render/gl/Mesh.cpp \
	$(SRC_DIR)render/gl/ResourceManager.cpp \
	$(SRC_DIR)render/gl/State.cpp \
	$(SRC_DIR)render/gl/Texture.cpp

C_OBJECTS=$(C_SOURCES:.c=.o)
CXX_OBJECTS=$(CXX_SOURCES:.cpp=.o)

OBJECTS = \
	$(C_OBJECTS) \
	$(CXX_OBJECTS) \
	$(OBJC_OBJECTS)

$(DYNAMIC_TARGET): $(OBJECTS)
	$(CXX) -o $(DYNAMIC_TARGET) $(LDFLAGS) $(OBJECTS)

$(STATIC_TARGET): $(OBJECTS)
	$(AR) -r -s $(STATIC_TARGET) $(OBJECTS)

static: $(STATIC_TARGET)

dynamic: $(DYNAMIC_TARGET)

.PHONY: static dynamic

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

%.o: %.cpp
	$(CXX) -o $@ $(CXXFLAGS) -c $<

%.o: %.mm
	$(OBJC) -o $@ $(OBJCFLAGS) -c $<
