#include <iostream>
#include <string>
#include "hpmud.h"
#include "HpmudBackend.h"

HpmudBackend::HpmudBackend (const char* deviceUri) :
    deviceOpen {false}
{
    HPMUD_RESULT result;
    uri = std::string (deviceUri);

    result = hpmud_query_model (uri.data (), &modelAttrs);
    validUri = (result == HPMUD_R_OK);
}

HpmudBackend::~HpmudBackend ()
{
    if (deviceOpen)
    {
        // Just try anyway.
        std::cerr << "DEBUG: HPMUD device was still open" << std::endl;
        (void)hpmud_close_device (device);
    }
}

bool
HpmudBackend::getDeviceID (std::string& deviceID)
{
    char buf[1024];
    int bufSize = sizeof (buf) - 1;
    int bytesRead;

    if (!validUri)
        return false;

    HPMUD_RESULT result;
    result = hpmud_open_device (uri.data (),
            modelAttrs.prt_mode, &device);

    if (result != HPMUD_R_OK)
        return false;

    result = hpmud_get_device_id (device, buf, bufSize,
            &bytesRead);
    // We note whether or not we were able to close the device.
    //  We'll try again later in the destructor if need be.
    deviceOpen = HPMUD_R_OK != hpmud_close_device (device);

    // No valid result?
    if (result != HPMUD_R_OK || bytesRead < 1)
        return false;

    // HP makes sure the status is NULL-terminated for us
    deviceID = buf;
    return true;
}

// vim: et sw=4
