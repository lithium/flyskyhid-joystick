//
//  FlySkyDeviceImpl.h
//  FlySkyJoystick
//
//  Created by Wiggins on 5/4/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <IOKit/IOKitLib.h>

typedef enum
{
    FlySkyDeviceMethodSelectorEnable                  = 0,
    FlySkyDeviceMethodSelectorDisable                 = 1,
    FlySkyDeviceMethodSelectorUpdateState             = 2,
    FlySkyDeviceMethodSelectorSetDeviceProductString  = 3
} FlySkyDeviceMethodSelector;

@interface FlySkyDeviceImpl : NSObject
{
@private
    io_connect_t m_Connection;
}

+ (BOOL)prepare;

- (BOOL)call:(FlySkyDeviceMethodSelector)selector;
- (BOOL)call:(FlySkyDeviceMethodSelector)selector data:(NSData*)data;

@end
