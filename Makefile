APP_NAME        := MIS
APP_BUNDLE      := $(APP_NAME).app
APP_EXECUTABLE  := mis

CC              := clang
CFLAGS          := -Wall -Wextra -Iinclude -O2
OBJCFLAGS       := -Wall -Wextra -Iinclude -ObjC -O2
LDFLAGS         := -framework Cocoa -framework IOKit -framework Carbon

SRC_DIR         := src
BUILD_DIR       := build

C_SOURCES       := $(wildcard $(SRC_DIR)/**/*.c) $(wildcard $(SRC_DIR)/*.c)
OBJC_SOURCES    := $(wildcard $(SRC_DIR)/**/*.m)

C_OBJS          := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
OBJC_OBJS       := $(patsubst $(SRC_DIR)/%.m,$(BUILD_DIR)/%.o,$(OBJC_SOURCES))
OBJECTS         := $(C_OBJS) $(OBJC_OBJS)

DEPS            := $(OBJECTS:.o=.d)

.PHONY: all clean build link run

all: build_app

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.m | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(OBJCFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

link: $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $(BUILD_DIR)/$(APP_EXECUTABLE)

build_app: link
	rm -rf $(APP_BUNDLE)
	mkdir -p $(APP_BUNDLE)/Contents/MacOS
	cp $(BUILD_DIR)/$(APP_EXECUTABLE) $(APP_BUNDLE)/Contents/MacOS/$(APP_EXECUTABLE)
	chmod +x $(APP_BUNDLE)/Contents/MacOS/$(APP_EXECUTABLE)
	@if [ -x ./generate_plist.sh ]; then \
		chmod +x ./generate_plist.sh; \
		./generate_plist.sh $(APP_BUNDLE) $(APP_EXECUTABLE); \
	else \
		echo "[WARN] generate_plist.sh not found or not executable"; \
	fi

run: build_app
	./$(APP_BUNDLE)/Contents/MacOS/$(APP_EXECUTABLE)

clean:
	rm -rf $(BUILD_DIR) $(APP_BUNDLE)
