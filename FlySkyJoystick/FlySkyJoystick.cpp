#include "FlySkyJoystick.h"
#include "flyskyjoy_debug.h"

#define super IOService

OSDefineMetaClassAndStructors(FlySkyJoystick, super)

bool FlySkyJoystick::start(IOService *provider)
{
    if(!super::start(provider))
        return false;
    
    registerService();
    dmsg("started");
    return true;
}

bool FlySkyJoystick::handleOpen(
                            IOService       *forClient,
                            IOOptionBits     options,
                            void            *arg)
{
    dmsg("handleOpen");
    return (!isOpen(forClient));
}
