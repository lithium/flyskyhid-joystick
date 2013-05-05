//
//  AppDelegate.h
//  FlySkyJoy
//
//  Created by Wiggins on 5/4/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <FlySkyJoystick/VHIDDevice.h>
#import <FlySkyJoystick/FlySkyJoystick.h>

@interface AppDelegate : NSObject <NSApplicationDelegate,VHIDDeviceDelegate>
{
    @private
        VHIDDevice *m_MouseState;
        FlySkyJoystick *m_VirtualMouse;

        NSFileHandle *m_UsbUart;
}

@property (assign) IBOutlet NSWindow *window;

@end
