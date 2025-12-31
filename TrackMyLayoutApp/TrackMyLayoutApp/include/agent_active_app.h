#ifndef AGENT_ACTIVE_APP_H
#define AGENT_ACTIVE_APP_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*active_app_callback_t)(const char *appName);

int agent_active_app_start(active_app_callback_t callback);
void agent_active_app_stop(void);

#ifdef __cplusplus
}
#endif

#endif
