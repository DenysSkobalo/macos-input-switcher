#ifndef AGENT_KEYBOARD_H
#define AGENT_KEYBOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

typedef struct {
  const char *inputSourceID;
  const char *localizedName;
  const char *deviceType;
  int vendorID;
  int productID;
  int isEnabled;
} KeyboardInfo;

const char *get_active_input_source_id(void);
const char *get_active_input_source_name(void);

KeyboardInfo *list_connected_keyboards(size_t *count);
void free_keyboard_info_array(KeyboardInfo *infos, size_t count);

void listInputSources(void);
void currentKeyboardInputSource(void);

int switch_input_source(const char *inputSourceID);

#ifdef __cplusplus
}
#endif

#endif
