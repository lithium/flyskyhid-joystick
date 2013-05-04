//
//  FlySkyJoystickUserClient.h
//  FlySkyJoystick
//
//  Created by Wiggins on 5/3/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef FlySkyJoystick_FlySkyJoystickUserClient_h
#define FlySkyJoystick_FlySkyJoystickUserClient_h

#include <IOKit/IOUserClient.h>
#include "flyskyjoy_config.h"

class FlySkyJoystick;
class FlySkyJoystickDevice;


class FlySkyJoystickUserClient : public IOUserClient
{
    OSDeclareDefaultStructors(FlySkyJoystickUserClient)
    
public:
    virtual bool initWithTask(
                              task_t           owningTask,
                              void            *securityToken,
                              UInt32           type,
                              OSDictionary    *properties);
    
    virtual bool start(IOService *provider);
    virtual void stop(IOService *provider);
    
    virtual IOReturn clientClose();
    virtual bool didTerminate(IOService *provider, IOOptionBits options, bool *defer);
    
    virtual IOReturn externalMethod(
                                    uint32_t selector,
                                    IOExternalMethodArguments *arguments,
									IOExternalMethodDispatch *dispatch,
                                    OSObject *target,
                                    void *reference);
    
protected:
    virtual void free();
    

private:
    static const size_t                     externalMethodCount = 4;
    static const IOExternalMethodDispatch   externalMethodDispatchTable[externalMethodCount];

    
    FlySkyJoystick *m_Owner;
    FlySkyJoystickDevice *m_Device;
    OSString *m_DeviceProductString;

    
    bool openOwner(FlySkyJoystick *owner);
    bool closeOwner();
    
    
    static IOReturn _enableDevice(FlySkyJoystickUserClient *target, void *reference, IOExternalMethodArguments *args);
    static IOReturn _disableDevice(FlySkyJoystickUserClient *target, void *reference, IOExternalMethodArguments *args);
    static IOReturn _updateDeviceState(FlySkyJoystickUserClient *target, void *reference, IOExternalMethodArguments *args);
    static IOReturn _setDeviceProductString(FlySkyJoystickUserClient *target, void *reference, IOExternalMethodArguments *args);

    IOReturn enableDevice(const void *hidDescriptorData, uint32_t hidDescriptorDataSize);
    IOReturn disableDevice();
    IOReturn updateDeviceState(const void *hidData, uint32_t hidDataSize);
    IOReturn setDeviceProductString(const void *productString, uint32_t productStringSize);

};

#endif
