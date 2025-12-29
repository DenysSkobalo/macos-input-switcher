APP_NAME      := MIS
APP_BUNDLE    := $(APP_NAME).app
APP_EXEC      := mis

CC            := clang
CFLAGS        := -Wall -Wextra -Iinclude -O2
OBJCFLAGS     := -Wall -Wextra -Iinclude -ObjC -O2
LDFLAGS       := -framework Cocoa -framework IOKit -framework Carbon

SRC_DIR       := src
BUILD_DIR     := build

SRC_C         := $(wildcard $(SRC_DIR)/*.c)
SRC_OBJC      := $(wildcard $(SRC_DIR)/*.m)

OBJ_C         := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_C))
OBJ_OBJC      := $(patsubst $(SRC_DIR)/%.m,$(BUILD_DIR)/%.o,$(SRC_OBJC))

OBJS          := $(OBJ_C) $(OBJ_OBJC)

DEPS          := $(OBJS:.o=.d)

.PHONY: all clean mis agent run

all: agent

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# ---------- Compile C ----------
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# ---------- Compile Obj-C ----------
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.m | $(BUILD_DIR)
	$(CC) $(OBJCFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

# ---------- Link ----------
mis: $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $(BUILD_DIR)/$(APP_EXEC)

# ---------- macOS Utility Agent ----------
agent: mis
	rm -rf $(APP_BUNDLE)
	mkdir -p $(APP_BUNDLE)/Contents/MacOS
	mkdir -p $(APP_BUNDLE)/Contents

	cp $(BUILD_DIR)/$(APP_EXEC) \
	   $(APP_BUNDLE)/Contents/MacOS/$(APP_EXEC)

	chmod +x $(APP_BUNDLE)/Contents/MacOS/$(APP_EXEC)

	@if [ -x ./generate_plist.sh ]; then \
		chmod +x ./generate_plist.sh; \
		./generate_plist.sh $(APP_BUNDLE) $(APP_EXEC); \
	else \
		echo "[WARN] generate_plist.sh not found or not executable"; \
	fi

# ---------- Run ----------
run: agent
	./$(APP_BUNDLE)/Contents/MacOS/$(APP_EXEC)

# ---------- Clean ----------
clean:
	rm -rf $(BUILD_DIR) $(APP_BUNDLE)
