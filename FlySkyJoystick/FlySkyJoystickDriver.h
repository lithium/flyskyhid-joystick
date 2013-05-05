//
//  FlySkyJoystickDriver.h
//  FlySkyJoystick
//
//  Created by Wiggins on 5/4/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef FlySkyJoystick_FlySkyJoystickDriver_h
#define FlySkyJoystick_FlySkyJoystickDriver_h

#include <IOKit/IOService.h>
#include "flyskyjoy_config.h"

class FlySkyJoystickDriver : public IOService
{
    OSDeclareDefaultStructors(FlySkyJoystickDriver)
public:
    virtual bool start(IOService *provider);
    
    virtual bool handleOpen(
                            IOService       *forClient,
                            IOOptionBits     options,
                            void            *arg);
};


#endif
