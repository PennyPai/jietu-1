#include "cscreenpublic.h"
#import <Foundation/Foundation.h>
#import <AppKit/NSScreen.h>
#import <AppKit/NSApplication.h>

QList<QRect> getWindownRectList()
{
    QList<QRect> rectList;
    NSArray *screenArray = [NSScreen screens];
    NSScreen *mainScreen = [NSScreen mainScreen];
    unsigned screenCount = [screenArray count];
    unsigned index  = 0;
    NSLog(@"screenCount #%d", screenCount);

    for (index; index < screenCount; index++)
    {
      NSScreen *screen = [screenArray objectAtIndex: index];
      NSRect screenRect = [screen visibleFrame];
      screenRect = [screen convertRectToBacking: screenRect];
      NSString *mString = ((mainScreen == screen) ? @"Main" : @"not-main");

      NSLog(@"Screen #%d (%@) Frame: %@", index, mString, NSStringFromRect(screenRect));
    }
    int windowCount = [[[NSApplication sharedApplication] windows] count];
    NSLog(@"windowCount #%d", windowCount);

    NSMutableArray *windows = (__bridge NSMutableArray *)CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);

    for (NSDictionary *window in windows) {
        NSString *name = [window objectForKey:@"kCGWindowName" ];
        CGRect bounds;
        CGRectMakeWithDictionaryRepresentation((CFDictionaryRef)[window objectForKey:@"kCGWindowBounds"], &bounds);
//        NSLog(@"TTT %@: %@",name,NSStringFromRect(bounds));
//        NSLog(@"rect x = %0.0f,y = %0.0f,w = %0.0f:h = %0.0f",bounds.origin.x,bounds.origin.y,bounds.size.width,bounds.size.height);
        QRect rect;
        rect.setX(bounds.origin.x);
        rect.setY(bounds.origin.y);
        rect.setWidth(bounds.size.width);
        rect.setHeight(bounds.size.height);
        if(!rectList.contains(rect))
        {
            rectList.append(rect);
        }
    }
    return rectList;
}

