#ifndef FlySkyJoystick_h
#define FlySkyJoystick_h

#include <IOKit/IOService.h>
#include "flyskyjoy_config.h"

class FlySkyJoystick : public IOService
{
    OSDeclareDefaultStructors(FlySkyJoystick)
public:
    virtual bool start(IOService *provider);
    
    virtual bool handleOpen(
                            IOService       *forClient,
                            IOOptionBits     options,
                            void            *arg);
};


#endif