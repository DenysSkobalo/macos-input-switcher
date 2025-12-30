#ifndef LAYOUT_SWITCHER_H
#define LAYOUT_SWITCHER_H

#include "rules_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

void layout_switcher_init(RulesConfig *cfg);
void layout_switcher_on_active_app_changed(const char *appName);
void layout_switcher_reload_rules(RulesConfig *cfg);
void layout_switcher_free(void);

#ifdef __cplusplus
}
#endif

#endif
