CXX = g++
CXXFLAGS = -O2 -Wall
LIBS = -lz -lSDL
HDRS = -I include

SRCS = $(wildcard $(SRC)/*.cpp)
OBJS = $(patsubst %.c, %.o, $(shell find . -name \*.c))
OBJS += $(patsubst %.cpp, %.o, $(shell find . -name \*.cpp))
OBJS += $(patsubst %.S, %.o, $(shell find . -name \*.S))

OUT = bin/
all: build

build: $(OBJS)
	@mkdir -p $(OUT)
	$(CXX) $(HDRS) $(LIBS) $(OBJS) -o $(OUT)/kspire

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(HDRS) -c $< -o $@
run: build
	$(OUT)/kspire
clean:
	@rm -rf $(OUT)/kspire
	@rm -rf $(OBJS)
