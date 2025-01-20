# Set this to your nest path
ENGINE_DIR := C:/Users/ocleo/Desktop/nest

# Do not alter any instructions below
MAIN_C := $(abspath main.c)

all:
	@echo "Building nest and executable..."
	$(MAKE) -C $(ENGINE_DIR) MAIN_C=$(MAIN_C)

clean:
	@echo "Cleaning build files..."
	$(MAKE) -C $(ENGINE_DIR) MAIN_C=$(MAIN_C) clean

.PHONY: all clean