APP_NAME       := TrackMyLayoutCLI
APP_EXECUTABLE := $(APP_NAME)

CC             := clang
CFLAGS         := -Wall -Wextra -O2 -ITrackMyLayoutApp/TrackMyLayoutApp/include
OBJCFLAGS      := -Wall -Wextra -O2 -ObjC -ITrackMyLayoutApp/TrackMyLayoutApp/include
LDFLAGS        := -framework Cocoa -framework IOKit -framework Carbon

BUILD_DIR      := build
SRC_DIR        := TrackMyLayoutApp/TrackMyLayoutApp/src

C_SOURCES      := main.c \
                $(wildcard $(SRC_DIR)/*.c)
OBJC_SOURCES   := $(wildcard $(SRC_DIR)/*.m)

C_OBJS         := $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
OBJC_OBJS      := $(patsubst %.m,$(BUILD_DIR)/%.o,$(OBJC_SOURCES))
OBJECTS        := $(C_OBJS) $(OBJC_OBJS)

DEPS := $(OBJECTS:.o=.d)

.PHONY: all clean run

all: $(BUILD_DIR)/$(APP_EXECUTABLE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/%.o: %.m | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(OBJCFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/$(APP_EXECUTABLE): $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@

-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR)
