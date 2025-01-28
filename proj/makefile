# Set this to your nest path
ENGINE_DIR := C:/Users/ocleo/Desktop/nest

# Do not alter any instructions below
MAIN_C := $(abspath main.c)

all:
	$(MAKE) -C $(ENGINE_DIR) MAIN_C=$(MAIN_C)

run:
	$(MAKE) -C $(ENGINE_DIR) MAIN_C=$(MAIN_C) run

clean:
	$(MAKE) -C $(ENGINE_DIR) MAIN_C=$(MAIN_C) clean

.PHONY: all clean