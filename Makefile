# Set compiler and flags
CC = gcc

CFLAGS=\
	-Wall \
	-Wextra \
	-pedantic \
	-I"src" \
	-I"deps/SDL-main/include" \
	-I"deps/gl3w/include" \
	-I"deps/cglm/include" \
	-I"deps/stb_image"

LDFLAGS=-L"deps/SDL-main/build" -lSDL3 -lm

# Sources
SRC_DIR = src
OBJ_DIR = objects
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Scenes
SCENES_SRCS = $(wildcard $(SRC_DIR)/scenes/*.c)
SCENES_OBJS = $(SCENES_SRCS:$(SRC_DIR)/scenes/%.c=$(OBJ_DIR)/scenes/%.o)

# Shaders
SHADERS=image basic
SHADERS_DIR=shaders
SHADERS_OUT_DIR=src/shaders/compiled
MINIFIER=mono tools/shader_minifier.exe --no-inlining --format c-variables --preserve-externals
FS_SHADERS = $(patsubst %, $(SHADERS_OUT_DIR)/%_fs.h, $(SHADERS))
VS_SHADERS = $(patsubst %, $(SHADERS_OUT_DIR)/%_vs.h, $(SHADERS))

SHADERS_SRCS = $(wildcard $(SRC_DIR)/shaders/*.c)
SHADERS_OBJS = $(SHADERS_SRCS:$(SRC_DIR)/shaders/%.c=$(OBJ_DIR)/shaders/%.o)

# Program rules
all: $(FS_SHADERS) $(VS_SHADERS) $(OBJS) $(SCENES_OBJS) $(SHADERS_OBJS) objects/gl3w.o
	mkdir -p bin
	$(CC) -o bin/tetris.bin objects/*.o objects/scenes/*.o objects/shaders/*.o $(LDFLAGS)
	cp -r resources/ bin/

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/scenes/%.o: $(SRC_DIR)/scenes/%.c
	@mkdir -p $(OBJ_DIR)/scenes
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

$(OBJ_DIR)/shaders/%.o: $(SRC_DIR)/shaders/%.c
	@mkdir -p $(OBJ_DIR)/shaders
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

objects/gl3w.o: deps/gl3w/src/gl3w.c
	mkdir -p objects
	$(CC) -c deps/gl3w/src/gl3w.c ${CFLAGS} -o objects/gl3w.o

# Shader rules
$(SHADERS_OUT_DIR)/%_fs.h: $(SHADERS_DIR)/%.fs
	mkdir -p src/shaders/compiled
	$(MINIFIER) $< -o $@

$(SHADERS_OUT_DIR)/%_vs.h: $(SHADERS_DIR)/%.vs
	mkdir -p src/shaders/compiled
	$(MINIFIER) $< -o $@

# deps
gl3w:
	rm -rf deps/gl3w/build && mkdir -p deps/gl3w/build
	cd deps/gl3w/build && cmake -S .. -B . && cmake --build .

run:
	cd bin && ./tetris.bin

clean:
	rm -rf bin/*.bin
	rm -rf bin/resources
	rm -rf objects/*.o
	rm -rf objects/scenes/*.o
	rm -rf objects/shaders/*.o
	rm -rf src/shaders/compiled/*.h

.PHONY: all run clean gl3w
