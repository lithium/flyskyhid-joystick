#include "FlySkyJoystickDriver.h"
#include "flyskyjoy_debug.h"

#define super IOService

OSDefineMetaClassAndStructors(FlySkyJoystickDriver, super)

bool FlySkyJoystickDriver::start(IOService *provider)
{
    if(!super::start(provider))
        return false;
    
    registerService();
    dmsg("started");
    return true;
}

bool FlySkyJoystickDriver::handleOpen(
                            IOService       *forClient,
                            IOOptionBits     options,
                            void            *arg)
{
    dmsg("handleOpen");
    return (!isOpen(forClient));
}
