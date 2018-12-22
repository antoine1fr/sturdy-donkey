CC ?= clang
CXX ?= clang++
OBJC ?= clang++
RM = rm -f
AR ?= ar
TARGET_PREFIX ?= lib
TARGET_SUFFIX ?= .a

CXXFLAGS += -Ilib/glm

include build/common.Makefile

