BUILD_DIR = build
TARGET = server
MAKEFLAGS += --no-print-directory

all: build

build:
	@mkdir -p $(BUILD_DIR)
	@cmake -S . -B $(BUILD_DIR)
	@cmake --build $(BUILD_DIR)

run: build
	@./$(BUILD_DIR)/$(TARGET)

clean:
	@rm -rf $(BUILD_DIR)

re: clean build

.PHONY: all build run clean re
