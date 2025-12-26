#import <Cocoa/Cocoa.h>
#import "layout_switcher.h"

int startApplicationActivityAgent() {
    @autoreleasepool {
        NSApplication *app = [NSApplication sharedApplication];

        [[[NSWorkspace sharedWorkspace] notificationCenter] addObserverForName:NSWorkspaceDidActivateApplicationNotification
                                                                         object:nil
                                                                          queue:[NSOperationQueue mainQueue]
                                                                     usingBlock:^(NSNotification *note) {
            NSRunningApplication *activeApp = note.userInfo[NSWorkspaceApplicationKey];
            NSString *appName = activeApp.localizedName;
            if (appName) {
                layout_switcher_on_active_app_changed([appName UTF8String]);
            }
        }];

        NSLog(@"Agent started. Watching active applications...");
        [app run];
    }
    return 0;
}
