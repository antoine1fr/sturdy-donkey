CC ?= gcc
CXX ?= g++
OBJC ?= g++
RM = rm -f
AR ?= ar
TARGET_PREFIX ?= lib
STATIC_TARGET_SUFFIX ?= .a
DYNAMIC_TARGET_SUFFIX ?= .so

LDFLAGS += -lgl

include build/common.Makefile
