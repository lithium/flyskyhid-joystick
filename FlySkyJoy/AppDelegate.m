//
//  AppDelegate.m
//  FlySkyJoy
//
//  Created by Wiggins on 5/4/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "AppDelegate.h"

#include <termios.h>

@implementation AppDelegate

@synthesize window = _window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
    
    m_HIDState    = [[VHIDDevice alloc] initWithType:VHIDDeviceTypeJoystick
                                          pointerCount:6
                                           buttonCount:2
                                            isRelative:YES];
    NSLog(@"%@", m_HIDState);
    m_VirtualJoystick  = [[FlySkyJoystick alloc] initWithHIDDescriptor:[m_HIDState descriptor]
                                                  productString:@"FlySky Transmitter"];
    
    [m_HIDState setDelegate:self];
    if(m_VirtualJoystick == nil || m_HIDState == nil)
        NSLog(@"error");
    

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
    [m_VirtualJoystick updateHIDState:state];
}

- (void)newConnection:(NSNotification*)notification
{
    NSLog(@"connected to usb uart");
    
    
    //set up fd for uart: 115200 N81
    struct termios toptions;
    int fd = [m_UsbUart fileDescriptor];
    
    if (tcgetattr(fd, &toptions) < 0) {
        NSLog(@"couldn't get term attributes");
        return;
    }
    
    cfsetispeed(&toptions, B115200);
    cfsetospeed(&toptions, B115200);
    
    toptions.c_oflag = 0;
    //
    // Input flags - Turn off input processing
    // convert break to null byte, no CR to NL translation,
    // no NL to CR translation, don't mark parity errors or breaks
    // no input parity check, don't strip high bit off,
    // no XON/XOFF software flow control
    //
    toptions.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK| ISTRIP | IXON);
    // No line processing:
    // echo off, echo newline off, canonical mode off, 
    // extended input processing off, signal chars off
    //
    toptions.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
    //
    // Turn off character processing
    // clear current char size mask, no parity checking,
    // no output processing, force 8 bit input
    //
    toptions.c_cflag &= ~(CSIZE | PARENB);
    toptions.c_cflag |= CS8;
    //
    // One input byte is enough to return from read()
    // Inter-character timer off
    //
    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 0;    
    
    if( tcsetattr(fd, TCSANOW, &toptions) < 0) {
        NSLog(@"couldn't set term attributes");
        return;
    }


    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(uartDataAvailable:)
                                                 name:NSFileHandleDataAvailableNotification
                                               object:m_UsbUart];
    [m_UsbUart waitForDataInBackgroundAndNotify];
}

- (void)uartDataAvailable:(NSNotification *)note
{    
    NSData *packet = [m_UsbUart readDataOfLength:18];    
    unsigned char *b =(unsigned char *)packet.bytes;
    
    if (b[0] == 85 && b[1] == 252) {
        // channel data packet
        unsigned int chksum = (b[16]<<8)|b[17];
        unsigned int chk=0;
        for (int i=2; i<16; i++) {
            chk += b[i];
        }
        if (chk == chksum) {
            unsigned int ch1 = ((b[2]<<8)|b[3])-1000;
            unsigned int ch2 = ((b[4]<<8)|b[5])-1000;
            unsigned int ch3 = ((b[6]<<8)|b[7])-1000;
            unsigned int ch4 = ((b[8]<<8)|b[9])-1000;
            unsigned int ch5 = ((b[10]<<8)|b[11])-1000;
            unsigned int ch6 = ((b[12]<<8)|b[13])-1000;
            
            [m_HIDState setPointer:0 position:NSMakePoint(ch1/1024.0,ch2/1024.0*-1)];
            [m_HIDState setPointer:1 position:NSMakePoint(ch3/1024.0,ch4/1024.0*-1)];
            [m_HIDState setPointer:2 position:NSMakePoint(ch5/1024.0,ch6/1024.0*-1)];
        } else {
            NSLog(@"Chksum mismatch!");
        }
    }
    
    
    
    [m_UsbUart waitForDataInBackgroundAndNotify];
}
@end
