CC = gcc
CFLAGS = -std=c11 -pedantic
TARGET = dns_proxy_server
SRCDIR = src
INCDIR = include
OBJDIR = obj


sources := $(wildcard $(SRCDIR)/*.c)
objects = $(sources:$(SRCDIR)/%.c=$(OBJDIR)/%.o)


.PHONY: all clean


all: $(TARGET)


$(TARGET): $(objects)
	$(CC) $(CFLAGS) -o $@ $^ -l json-c


$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) -c $(CFLAGS) -I $(INCDIR) -o $@ $<


$(OBJDIR):
	mkdir -p $(OBJDIR)


clean:
	rm -rf $(OBJDIR) $(TARGET)


rebuild: clean all