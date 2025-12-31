#import <Cocoa/Cocoa.h>
#import <Carbon/Carbon.h>
#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>
#import <IOKit/hid/IOHIDManager.h>
#import "agent_keyboard.h"

void listInputSources(void) {
    CFArrayRef sources = TISCreateInputSourceList(NULL, false);
    for (CFIndex i = 0; i < CFArrayGetCount(sources); i++) {
        TISInputSourceRef source = (TISInputSourceRef)CFArrayGetValueAtIndex(sources, i);
        NSString *name = (__bridge NSString *)TISGetInputSourceProperty(source, kTISPropertyLocalizedName);
        NSString *id = (__bridge NSString *)TISGetInputSourceProperty(source, kTISPropertyInputSourceID);
        BOOL isEnabled = [(NSNumber *)CFBridgingRelease(TISGetInputSourceProperty(source, kTISPropertyInputSourceIsEnabled)) boolValue];
        if (isEnabled) {
            NSLog(@"Input source: %@ (%@)", name, id);
        }
    }
    if (sources) CFRelease(sources);
}

const char *get_active_input_source_id(void) {
    TISInputSourceRef source = TISCopyCurrentKeyboardInputSource();
    NSString *sourceID = (__bridge NSString*)TISGetInputSourceProperty(source, kTISPropertyInputSourceID);
    if (source) CFRelease(source);
    return sourceID ? strdup([sourceID UTF8String]) : strdup("Unknown");
}

const char* get_active_input_source_name(void) {
    TISInputSourceRef source = TISCopyCurrentKeyboardInputSource();
    NSString *name = (__bridge NSString*)TISGetInputSourceProperty(source, kTISPropertyLocalizedName);
    if (source) CFRelease(source);
    return name ? strdup([name UTF8String]) : strdup("Unknown");
}

void free_keyboard_info_array(KeyboardInfo *infos, size_t count) {
    if (!infos) return;
    for (size_t i = 0; i < count; i++) {
        free((void*)infos[i].inputSourceID);
        free((void*)infos[i].localizedName);
        free((void*)infos[i].deviceType);
    }
    free(infos);
}

KeyboardInfo* list_connected_keyboards(size_t *count) {
    IOHIDManagerRef manager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    IOHIDManagerSetDeviceMatching(manager, NULL); 
    IOHIDManagerOpen(manager, kIOHIDOptionsTypeNone);

    CFSetRef devices = IOHIDManagerCopyDevices(manager);
    CFIndex numDevices = devices ? CFSetGetCount(devices) : 0;
    *count = (size_t)numDevices;

    KeyboardInfo *infos = calloc(*count, sizeof(KeyboardInfo));
    if (devices && numDevices > 0) {
        IOHIDDeviceRef *deviceArray = malloc(sizeof(IOHIDDeviceRef) * numDevices);
        CFSetGetValues(devices, (const void **)deviceArray);

        for (CFIndex i = 0; i < numDevices; i++) {
            IOHIDDeviceRef dev = deviceArray[i];

            CFTypeRef vendor = IOHIDDeviceGetProperty(dev, CFSTR(kIOHIDVendorIDKey));
            CFTypeRef product = IOHIDDeviceGetProperty(dev, CFSTR(kIOHIDProductIDKey));
            infos[i].vendorID = vendor ? [(NSNumber*)CFBridgingRelease(vendor) intValue] : 0;
            infos[i].productID = product ? [(NSNumber*)CFBridgingRelease(product) intValue] : 0;

            infos[i].deviceType = strdup("Keyboard");

            CFStringRef name = IOHIDDeviceGetProperty(dev, CFSTR(kIOHIDProductKey));
            infos[i].localizedName = name ? strdup([(__bridge NSString*)name UTF8String]) : strdup("Unknown");

            infos[i].inputSourceID = strdup("Unknown");
            infos[i].isEnabled = 1;
        }

        free(deviceArray);
        CFRelease(devices);
    }

    CFRelease(manager);
    return infos;
}

int switch_input_source(const char* inputSourceID) {
    if (!inputSourceID) {
        fprintf(stderr, "[ERROR] switch_input_source: inputSourceID is NULL\n");
        return 0;
    }

    CFStringRef targetID = CFStringCreateWithCString(NULL, inputSourceID, kCFStringEncodingUTF8);
    if (!targetID) {
        fprintf(stderr, "[ERROR] CFStringCreateWithCString failed\n");
        return 0;
    }

    CFDictionaryRef dict = CFDictionaryCreate(NULL,
        (const void**)&kTISPropertyInputSourceID,
        (const void**)&targetID,
        1,
        &kCFTypeDictionaryKeyCallBacks,
        &kCFTypeDictionaryValueCallBacks);

    if (!dict) {
        fprintf(stderr, "[ERROR] CFDictionaryCreate failed\n");
        CFRelease(targetID);
        return 0;
    }

    CFArrayRef sources = TISCreateInputSourceList(dict, false);
    CFRelease(dict);

    if (!sources || CFArrayGetCount(sources) == 0) {
        fprintf(stderr, "[ERROR] Input source '%s' not found\n", inputSourceID);
        if (sources) CFRelease(sources);
        CFRelease(targetID);
        return 0;
    }

    TISInputSourceRef source = (TISInputSourceRef)CFArrayGetValueAtIndex(sources, 0);
    OSStatus status = TISSelectInputSource(source);

    CFRelease(sources);
    CFRelease(targetID);

    if (status != noErr) {
        fprintf(stderr, "[ERROR] TISSelectInputSource failed with %d\n", (int)status);
        return 0;
    }

    return 1;
}
