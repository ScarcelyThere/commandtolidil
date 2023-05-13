#ifndef _USBBACKEND_H
#define _USBBACKEND_H

#include <string>
#include "Backend.h"

class USBBackend : public Backend
{
    public:
        USBBackend ();
        ~USBBackend ();

        bool getDeviceID (std::string&);
};

#endif

// vim: et sw=4
