#ifndef MACOS_BRIDGE_H
#define MACOS_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

const char *get_active_window_title(void);

void switch_keyboard_layout(const char *layout_identifier);

#ifdef __cplusplus
}
#endif

#endif
