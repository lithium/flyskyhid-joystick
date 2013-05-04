//
//  flysky_debug.h
//  FlySkyJoystick
//
//  Created by Wiggins on 5/3/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef FlySkyJoystick_flysky_debug_h
#define FlySkyJoystick_flysky_debug_h

#include <libkern/libkern.h>

#ifdef DEBUG

#define dmsg(message) \
printf("%s - %s (%d): %s\n", \
__FILE__, __PRETTY_FUNCTION__, __LINE__, message)

#define dmsgf(format, params ...) \
printf("%s - %s (%d): " format "\n", \
__FILE__, __PRETTY_FUNCTION__, __LINE__, params)

#else /* DEBUG */

#define dmsg(message)
#define dmsgf(format, params ...)

#endif /* DEBUG */


#endif
