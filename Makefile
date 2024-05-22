BUILD=build
PROJECT=MyProject

CXX = g++

.PHONY: init clean test run

all: cl cpu cuda

init:
	@cmake -S . -B $(BUILD)

cuda: init
	@cmake --build $(BUILD) --target $(PROJECT)CUDA -j 10

cl: init
	@cp src/cl/kernel.cl $(BUILD)/src/cl/kernel.cl
	@cmake -S . -B $(BUILD) -DCMAKE_CXX_COMPILER=$(CXX)
	@cmake --build $(BUILD) --target $(PROJECT)CL -j 10

cpu: init
	@cmake --build $(BUILD) --target $(PROJECT)CPU -j 10

test:
	ctest --test-dir $(BUILD) --output-on-failure

clean:
	rm -rf .cache Testing build

watch:
	find src/ test/ | entr -s "make"
