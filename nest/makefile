CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Isrc
LDFLAGS = -Llib -lSDL2 -lSDL2_image -mwindows

BUILD_DIR = $(dir $(MAIN_C))build
SRC_DIR = src
SDL2_DLL = lib/SDL2.dll
SDL2I_DLL = lib/SDL2_image.dll
TARGET = $(BUILD_DIR)/build.exe

ENGINE_SRCS = $(wildcard $(SRC_DIR)/*.c)
ENGINE_OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(ENGINE_SRCS))

$(shell mkdir $(BUILD_DIR) 2> nul)

all: $(TARGET)

$(TARGET): $(MAIN_C) $(ENGINE_OBJS)
	@echo "Building nest and executable..."
	$(CC) $(CFLAGS) $(MAIN_C) $(ENGINE_OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "Copying dll's to build directory..."
	@cp $(SDL2_DLL) $(BUILD_DIR)
	@cp $(SDL2I_DLL) $(BUILD_DIR)
	@echo "Build complete."

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	@echo "Running the application..."
	$(TARGET)

clean:
	@echo "Cleaning build files..."
	rm -rf $(BUILD_DIR)
	@echo "Cleaning complete."

.PHONY: all clean run
