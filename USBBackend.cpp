/* USBBackend.cpp: Implements USBBackend, which communicates with CUPS's USB
 *  backend to read a printer's status
 * Copyright (C) 2023 Scarcely There.
 *
 * dj3600command is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any other version.
 *
 * dj3600command is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the the GNU General Public License along with
 * dj3600command. If not, see <https://www.gnu.org/licenses/>.
 */

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
