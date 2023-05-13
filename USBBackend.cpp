#include <cups/cups.h>
#include <cups/sidechannel.h>
#include "USBBackend.h"

USBBackend::USBBackend () = default;

USBBackend::~USBBackend () = default;

bool
USBBackend::getDeviceID (std::string& deviceID)
{
    char buf[1024];
    int  bufSize = sizeof (buf) - 1;
    cups_sc_status_t status;
    bool success = true;

    status = cupsSideChannelDoRequest (CUPS_SC_CMD_GET_DEVICE_ID,
            buf, &bufSize, 1.0);

    if (status == CUPS_SC_STATUS_OK && bufSize > 0)
    {
        buf[bufSize] = '\0';
        deviceID = buf;
    }
    else
        success = false;

    return success;
}

// vim: et sw=4
