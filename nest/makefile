CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Isrc
LDFLAGS = -Llib -lSDL2 #-mwindows

BUILD_DIR = $(dir $(MAIN_C))build
SRC_DIR = src
SDL2_DLL = lib/SDL2.dll
TARGET = $(BUILD_DIR)/build.exe

ENGINE_SRCS = $(wildcard $(SRC_DIR)/*.c)
ENGINE_OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(ENGINE_SRCS))

$(shell mkdir $(BUILD_DIR) 2> nul)

all: $(TARGET)

$(TARGET): $(MAIN_C) $(ENGINE_OBJS)
	$(CC) $(CFLAGS) $(MAIN_C) $(ENGINE_OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "Copying SDL2.dll to build directory..."
	@cp $(SDL2_DLL) $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
	@echo "Cleaning build files complete."

.PHONY: all clean
