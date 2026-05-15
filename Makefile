
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c2x


# Project layout
SRCDIR = src
BUILDDIR = build
TARGET = $(BUILDDIR)/expression-parser
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRCS))

SYSTEM_COLL_PREFIX ?= /usr/local
SYSTEM_STATIC := $(wildcard $(SYSTEM_COLL_PREFIX)/lib/libcollections.a)


COLL_LIB_DIR := $(SYSTEM_COLL_PREFIX)/lib
COLL_INC := -I$(SYSTEM_COLL_PREFIX)/include
COLL_STATIC := $(COLL_LIB_DIR)/libcollections.a

LIBS = $(COLL_STATIC)
  LIBS = -L$(COLL_LIB_DIR) -lcollections
  LDFLAGS += -Wl,-rpath,$(COLL_LIB_DIR)

# Ensure math library is linked after object files
LIBS += -lm

# Add local include and collection includes
CFLAGS += -Iinclude $(COLL_INC)

.PHONY: all clean test

all: $(TARGET)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LIBS) -o $@

clean:
	rm -rf $(BUILDDIR)

test: all
	@echo "No tests configured"

