TARGET   = ifj2018
SRCDIR   = .
OBJDIR   = .
CC       = gcc
CFLAGS   = -std=c99 -I$(SRCDIR)/ -Wall -s -Wextra -c -O3
LINKER   = gcc
LFLAGS   = -Wall -s -I$(SRCDIR)/ -lm
GREEN    = tput setaf 2
RESET    = tput sgr0

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -rf

all: $(TARGET) clean

$(TARGET): $(OBJECTS)
	
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@$(GREEN)
	@echo "Binary file \"$(TARGET)\" created"
	@$(RESET)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
