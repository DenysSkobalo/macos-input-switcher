#include "macos_bridge.h"
#include "Carbon/Carbon.h"
#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

const char* get_active_window_title(void) {
    @autoreleasepool {
        NSWorkspace *ws = [NSWorkspace sharedWorkspace];
        NSArray *apps = [ws runningApplications];
        for (NSRunningApplication *app in apps) {
            if ([app isActive]) {
                NSString *title = [app localizedName];
                if (title) {
                    char *c_title = strdup([title UTF8String]);
                    return c_title; 
                }
            }
        }
    }
    return strdup("Unknown");
}

