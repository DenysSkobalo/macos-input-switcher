CC = clang
CFLAGS = -Wall -Wextra -Iinclude -std=c17
OBJCFLAGS = -Wall -Wextra -Iinclude -ObjC

SRC_C = src/main.c
SRC_OBJC = src/macos_bridge.m
OBJ = build/main.o build/macos_bridge.o
TARGET = mis

all: $(TARGET)

build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: src/%.m
	mkdir -p build
	$(CC) $(OBJCFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) -lobjc -framework Foundation -framework AppKit -framework Carbon

clean:
	rm -rf build $(TARGET)
