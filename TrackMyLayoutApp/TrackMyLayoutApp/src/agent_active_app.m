#import <Cocoa/Cocoa.h>
#import "agent_active_app.h"

static id observer = nil;
static active_app_callback_t g_callback = NULL;

int agent_active_app_start(active_app_callback_t callback) {
    if (observer) return 0; 

    g_callback = callback;

    observer = [[[NSWorkspace sharedWorkspace] notificationCenter]
        addObserverForName:NSWorkspaceDidActivateApplicationNotification
        object:nil
        queue:[NSOperationQueue mainQueue]
        usingBlock:^(NSNotification *note) {
            NSRunningApplication *app =
                note.userInfo[NSWorkspaceApplicationKey];
            if (app && g_callback) {
                g_callback(app.localizedName.UTF8String);
            }
        }];

    NSLog(@"[Agent] Started");
    return 1;
}

void agent_active_app_stop(void) {
    if (!observer) return;

    [[[NSWorkspace sharedWorkspace] notificationCenter]
        removeObserver:observer];
    observer = nil;
    g_callback = NULL;

    NSLog(@"[Agent] Stopped");
}
