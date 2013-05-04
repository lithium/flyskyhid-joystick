//
//  FlySkyJoystickUserClient.cpp
//  FlySkyJoystick
//
//  Created by Wiggins on 5/3/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "FlySkyJoystick.h"
#include "FlySkyJoystickUserClient.h"
#include "FlySkyJoystickDevice.h"
#include "flyskyjoy_debug.h"
#include "flyskyjoy_config.h"


#define super IOUserClient

OSDefineMetaClassAndStructors(FlySkyJoystickUserClient, super)



static bool checkString(const char *str, size_t maxLength)
{
    while(maxLength > 0)
    {
        if(*str == 0)
            return true;
        
        maxLength--;
        str++;
    }
    
    return false;
}


bool FlySkyJoystickUserClient::initWithTask(
                                        task_t           owningTask,
                                        void            *securityToken,
                                        UInt32           type,
                                        OSDictionary    *properties)
{
    if(!super::initWithTask(owningTask, securityToken, type, properties))
        return false;
    
    m_Owner = 0;
    m_Device = 0;
    m_DeviceProductString = OSString::withCString("FlySky Virtual HID Joystick");
    
    dmsg("initWithTask");
    return true;
}

void FlySkyJoystickUserClient::free()
{
    if(m_DeviceProductString != 0)
        m_DeviceProductString->release();
    
    dmsg("free");
    super::free();
}

bool FlySkyJoystickUserClient::start(IOService *provider)
{
    FlySkyJoystick *owner = OSDynamicCast(FlySkyJoystick, provider);
    if(owner == 0)
        return false;
    
    if(!super::start(provider))
        return false;
    
    if(!openOwner(owner))
    {
        super::stop(provider);
        return false;
    }
    
    dmsgf("start, provider: %p", provider);
    return true;
}

void FlySkyJoystickUserClient::stop(IOService *provider)
{
    dmsgf("stop, provider: %p", provider);
    closeOwner();
    super::stop(provider);
}

IOReturn FlySkyJoystickUserClient::clientClose()
{
    dmsg("clientClose");
    closeOwner();
    terminate();
    return kIOReturnSuccess;
}

bool FlySkyJoystickUserClient::didTerminate(IOService *provider, IOOptionBits options, bool *defer)
{
    dmsg("didTerminate");
    closeOwner();
	*defer = false;
	return super::didTerminate(provider, options, defer);
}

IOReturn FlySkyJoystickUserClient::externalMethod(
                                              uint32_t selector,
                                              IOExternalMethodArguments *arguments,
                                              IOExternalMethodDispatch *dispatch,
                                              OSObject *target,
                                              void *reference)
{
    dmsg("externalMehtod");
    
    if(selector < externalMethodCount)
    {
        dispatch = const_cast< IOExternalMethodDispatch* >(&externalMethodDispatchTable[selector]);
        if(target == 0)
            target = this;
    }
    
	return super::externalMethod(selector, arguments, dispatch, target, reference);
}




bool FlySkyJoystickUserClient::openOwner(FlySkyJoystick *owner)
{
    if(owner == 0 || isInactive())
        return false;
    
    if(!owner->open(this))
        return false;
    
    m_Owner = owner;
    return true;
}

bool FlySkyJoystickUserClient::closeOwner()
{
    if(m_Owner != 0)
    {
        if(m_Owner->isOpen(this))
            m_Owner->close(this);
        
        m_Owner = 0;
    }
    
    disableDevice();
    return true;
}



IOReturn FlySkyJoystickUserClient::enableDevice(const void *hidDescriptorData, uint32_t hidDescriptorDataSize)
{
    dmsgf("enableDevice, param size = %d", hidDescriptorDataSize);
    
    if(m_Device != 0)
    {
        IOReturn result = disableDevice();
        if(result != kIOReturnSuccess)
            return result;
    }
    
    m_Device = FlySkyJoystickDevice::withHidDescriptor(
                                                   hidDescriptorData,
                                                   hidDescriptorDataSize,
                                                   m_DeviceProductString);
    
    if(m_Device == 0)
        return kIOReturnDeviceError;
    
    if(!m_Device->attach(this))
    {
        m_Device->release();
        m_Device = 0;
        return kIOReturnDeviceError;
    }
    
    if(!m_Device->start(this))
    {
        m_Device->detach(this);
        m_Device->release();
        m_Device = 0;
        return kIOReturnDeviceError;
    }
    
    return kIOReturnSuccess;
}

IOReturn FlySkyJoystickUserClient::disableDevice()
{
    dmsg("disableDevice");
    
    if(m_Device != 0)
    {
        m_Device->terminate(kIOServiceRequired);
        m_Device->release();
        m_Device = 0;
    }
    
    return kIOReturnSuccess;
}

IOReturn FlySkyJoystickUserClient::updateDeviceState(const void *hidData, uint32_t hidDataSize)
{
    dmsgf("updateDeviceState, param size = %d", hidDataSize);
    
    if(m_Device == 0)
        return kIOReturnNoDevice;
    
    if(!m_Device->updateState(hidData, hidDataSize))
        return kIOReturnDeviceError;
    
    return kIOReturnSuccess;
}

IOReturn FlySkyJoystickUserClient::setDeviceProductString(const void *productString, uint32_t productStringSize)
{
    dmsgf("setDeviceProductString, productString size = %d", productStringSize);
    
    if(m_Device != 0)
        return kIOReturnBusy;
    
    if(!checkString(static_cast< const char* >(productString), productStringSize))
        return kIOReturnInvalid;
    
    OSString *newStr = OSString::withCString(static_cast< const char* >(productString));
    if(newStr == 0)
        return kIOReturnNoMemory;
    
    if(m_DeviceProductString != 0)
        m_DeviceProductString->release();
    
    m_DeviceProductString = newStr;
    
    dmsgf("newProductString = %s", newStr->getCStringNoCopy());
    return kIOReturnSuccess;
}


const IOExternalMethodDispatch FlySkyJoystickUserClient::externalMethodDispatchTable[externalMethodCount] =
{
    {
        (IOExternalMethodAction) FlySkyJoystickUserClient::_enableDevice,
        0, kIOUCVariableStructureSize, 0, 0
    },
    
    {
        (IOExternalMethodAction) FlySkyJoystickUserClient::_disableDevice,
        0, 0, 0, 0
    },
    
    {
        (IOExternalMethodAction) FlySkyJoystickUserClient::_updateDeviceState,
        0, kIOUCVariableStructureSize, 0, 0
    },
    
    {
        (IOExternalMethodAction) FlySkyJoystickUserClient::_setDeviceProductString,
        0, kIOUCVariableStructureSize, 0, 0
    }
};

IOReturn FlySkyJoystickUserClient::_enableDevice(FlySkyJoystickUserClient *target, void *reference, IOExternalMethodArguments *args)
{
    return target->enableDevice(args->structureInput, args->structureInputSize);
}

IOReturn FlySkyJoystickUserClient::_disableDevice(FlySkyJoystickUserClient *target, void *reference, IOExternalMethodArguments *args)
{
    return target->disableDevice();
}

IOReturn FlySkyJoystickUserClient::_updateDeviceState(FlySkyJoystickUserClient *target, void *reference, IOExternalMethodArguments *args)
{
    return target->updateDeviceState(args->structureInput, args->structureInputSize);
}

IOReturn FlySkyJoystickUserClient::_setDeviceProductString(FlySkyJoystickUserClient *target, void *reference, IOExternalMethodArguments *args)
{
    return target->setDeviceProductString(args->structureInput, args->structureInputSize);
}

