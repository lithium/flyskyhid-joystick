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
    
    m_MouseState    = [[VHIDDevice alloc] initWithType:VHIDDeviceTypeJoystick
                                          pointerCount:6
                                           buttonCount:2
                                            isRelative:YES];
    NSLog(@"%@", m_MouseState);
    m_VirtualMouse  = [[FlySkyJoystick alloc] initWithHIDDescriptor:[m_MouseState descriptor]
                                                  productString:@"Virtual FlySky Joystick"];
    
    [m_MouseState setDelegate:self];
    if(m_VirtualMouse == nil || m_MouseState == nil)
        NSLog(@"error");
    
//    NSPoint pos = NSZeroPoint;
//    pos.x = 0.38;
//    [m_MouseState setPointer:0 position:pos];
    
    m_UsbUart = [NSFileHandle fileHandleForReadingAtPath:@"/dev/cu.SLAB_USBtoUART"];
    if (m_UsbUart == nil) {
        NSLog(@"Failed to open usb uart");
    } else {
    
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(newConnection:) 
                                                     name:NSFileHandleConnectionAcceptedNotification 
                                                   object:m_UsbUart];
        [m_UsbUart acceptConnectionInBackgroundAndNotify];
    }
    
}


- (void)VHIDDevice:(VHIDDevice*)device stateChanged:(NSData*)state
{
    [m_VirtualMouse updateHIDState:state];
}

- (void)newConnection:(NSNotification*)notification
{
    NSLog(@"connected to usb uart");
    

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(uartDataAvailable:)
                                                 name:NSFileHandleDataAvailableNotification
                                               object:m_UsbUart];
    [m_UsbUart waitForDataInBackgroundAndNotify];
}

- (void)uartDataAvailable:(NSNotification *)note
{
//    NSLog(@"uartDataAvailable");
    
    NSData *databuffer = [m_UsbUart readDataOfLength:18];
    NSString* s = [[NSString alloc] initWithData:databuffer encoding:NSASCIIStringEncoding];
    NSLog(@"one packet: %s", s);
    [m_UsbUart waitForDataInBackgroundAndNotify];


}
@end
