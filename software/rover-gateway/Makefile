.PHONY: configure-debug build run

BUILD_DIR := ../../build/rover-gateway

configure-debug:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug

build: configure-debug
	cmake --build $(BUILD_DIR)

run: build
	./$(BUILD_DIR)/rover-gateway