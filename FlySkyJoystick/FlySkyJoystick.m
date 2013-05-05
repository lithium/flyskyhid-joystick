//
//  FlySkyJoystick.m
//  FlySkyJoystick
//
//  Created by Wiggins on 5/4/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "FlySkyJoystick.h"

@implementation FlySkyJoystick

+ (BOOL)prepare
{
    return [FlySkyDeviceImpl prepare];
}

- (id)init
{
//    [[super init] release];
    return nil;
}

- (id)initWithHIDDescriptor:(NSData*)HIDDescriptor
{
    return [self initWithHIDDescriptor:HIDDescriptor productString:nil];
}

- (id)initWithHIDDescriptor:(NSData*)HIDDescriptor productString:(NSString*)productString
{
    self = [super init];
    if(self == nil)
        return nil;
    
    m_Impl = [[FlySkyDeviceImpl alloc] init];
    if(m_Impl == nil)
    {
//        [self release];
        return nil;
    }
    
    if(productString != nil)
    {
        const char *str     = [productString UTF8String];
        size_t      strSize = strlen(str) + 1; // zero-terminator
        NSData     *data    = [NSData dataWithBytes:str length:strSize];
        
        if(![m_Impl call:FlySkyDeviceMethodSelectorSetDeviceProductString data:data])
        {
//            [self release];
            return nil;
        }
    }
    
    if(![m_Impl call:FlySkyDeviceMethodSelectorEnable data:HIDDescriptor])
    {
//        [self release];
        return nil;
    }
    
    return self;
}

//- (void)dealloc
//{
//    [m_Impl release];
//    [super dealloc];
//}

- (BOOL)updateHIDState:(NSData*)HIDState
{
    return [m_Impl call:FlySkyDeviceMethodSelectorUpdateState data:HIDState];
}

@end
