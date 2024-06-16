# Compiler
CC = gcc

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin
THIRDPARTYDIR = third-party/tomlc99

# Files
SOURCES = $(wildcard $(SRCDIR)/*.c) $(THIRDPARTYDIR)/toml.c
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))
EXECUTABLE = $(BINDIR)/interactive_novel_engine

# Flags
CFLAGS = -I$(SRCDIR) -I$(THIRDPARTYDIR)
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# Default target
all: $(EXECUTABLE)

# Link object files to create the executable
$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile third-party source files to object files
$(OBJDIR)/%.o: $(THIRDPARTYDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Run the executable
run: $(EXECUTABLE)
	./$(EXECUTABLE)

# Clean up generated files
clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean run
