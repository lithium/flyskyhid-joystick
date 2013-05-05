//
//  AppDelegate.m
//  FlySkyJoy
//
//  Created by Wiggins on 5/4/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "AppDelegate.h"

@implementation AppDelegate

@synthesize window = _window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
    
    m_MouseState    = [[VHIDDevice alloc] initWithType:VHIDDeviceTypeMouse
                                          pointerCount:6
                                           buttonCount:2
                                            isRelative:YES];
    NSLog(@"%@", m_MouseState);
    m_VirtualMouse  = [[FlySkyJoystick alloc] initWithHIDDescriptor:[m_MouseState descriptor]
                                                  productString:@"Virtual FlySky Joystick"];
    
    [m_MouseState setDelegate:self];
    if(m_VirtualMouse == nil || m_MouseState == nil)
        NSLog(@"error");

}


- (void)VHIDDevice:(VHIDDevice*)device stateChanged:(NSData*)state
{
    [m_VirtualMouse updateHIDState:state];
}

@end
