//
//  FlySkyJoystickDevice.h
//  FlySkyJoystick
//
//  Created by Wiggins on 5/3/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef FlySkyJoystick_FlySkyJoystickDevice_h
#define FlySkyJoystick_FlySkyJoystickDevice_h

#include <IOKit/hid/IOHIDDevice.h>
#include "flyskyjoy_config.h"

class FlySkyJoystickDevice : public IOHIDDevice
{
    OSDeclareDefaultStructors(FlySkyJoystickDevice)
public:
    static FlySkyJoystickDevice *withHidDescriptor(
                                               const void *hidDescriptorData,
                                               size_t hidDescriptorDataSize,
                                               OSString *productString);
    
    virtual bool init(
                      const void *hidDescriptorData,
                      size_t hidDescriptorDataSize,
                      OSString *productString,
                      OSDictionary *dictionary = 0);
    
    virtual IOReturn newReportDescriptor(IOMemoryDescriptor **descriptor) const;
    virtual OSString *newManufacturerString() const;
    virtual OSString *newProductString() const;
    virtual OSString *newTransportString() const;
    virtual OSNumber *newPrimaryUsageNumber() const;
    virtual OSNumber *newPrimaryUsagePageNumber() const;
    virtual OSNumber *newLocationIDNumber() const;
    
    bool updateState(const void *hidData, size_t hidDataSize);
    
protected:
    virtual void free();
    
private:
    static const uint32_t locationIdBase = 0xFAFAFAFA;
    
    OSString *m_ProductString;
    HIDCapabilities m_Capabilities;
    IOBufferMemoryDescriptor *m_HIDReportDescriptor;
    IOBufferMemoryDescriptor *m_StateBuffer;
    uint32_t m_LocationID;
    
    bool parseHidDescriptor(
                            const void *hidDescriptorData,
                            size_t hidDescriptorDataSize);
};


#endif
