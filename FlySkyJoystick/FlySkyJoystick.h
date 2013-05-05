//
//  FlySkyJoystick.h
//  FlySkyJoystick
//
//  Created by Wiggins on 5/4/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "FlySkyDeviceImpl.h"

@interface FlySkyJoystick : NSObject
{
@private
    FlySkyDeviceImpl *m_Impl;
}

+ (BOOL)prepare;

- (id)initWithHIDDescriptor:(NSData*)HIDDescriptor;
- (id)initWithHIDDescriptor:(NSData*)HIDDescriptor productString:(NSString*)productString;

- (BOOL)updateHIDState:(NSData*)HIDState;

@end
