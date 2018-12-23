CC ?= clang
CXX ?= clang++
OBJC ?= clang++
RM = rm -f
AR ?= ar
TARGET_PREFIX ?= lib
STATIC_TARGET_SUFFIX ?= .a
DYNAMIC_TARGET_SUFFIX ?= .dylib

CXXFLAGS += -Ilib/glm
LDFLAGS += -framework OpenGL

include build/common.Makefile

