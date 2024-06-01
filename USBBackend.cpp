/* USBBackend.cpp: Implements USBBackend, which communicates with CUPS's USB
 *  backend to read a printer's status
 * Copyright (C) 2023 Scarcely There.
 *
 * commandtolidil is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any other version.
 *
 * commandtolidil is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the the GNU General Public License along with
 * commandtolidil. If not, see <https://www.gnu.org/licenses/>.
 */

#include <cups/cups.h>
#include <cups/sidechannel.h>
#include "USBBackend.hpp"

USBBackend::USBBackend( )
{
    // We read the device ID here. We will throw a BackendException
    //  should we encounter any issues.
    cups_sc_status_t status;

    int requestSize = bufSize - 1;

    status = cupsSideChannelDoRequest( CUPS_SC_CMD_GET_DEVICE_ID,
                                       buf, &requestSize, 1.0 );

    if ( status == CUPS_SC_STATUS_OK && requestSize > 0 )
    {
        buf[requestSize] = '\0';
        myDeviceID = buf;
    }
    else
        throw BackendException ( errorMsgFromStatus( status ) );
}

std::string
USBBackend::errorMsgFromStatus( cups_sc_status_t status )
{
    switch ( status )
    {
        case CUPS_SC_STATUS_OK:
            return "No error was reported from the backend";
            break;

        case CUPS_SC_STATUS_IO_ERROR:
            return "Could not communicate with the printer via USB";
            break;

        case CUPS_SC_STATUS_NONE:
            return "No status was given";
            break;
            
        case CUPS_SC_STATUS_TIMEOUT:
            return "The printer did not respond in time";
            break;

        case CUPS_SC_STATUS_NO_RESPONSE:
            return "The printer didn't respond";
            break;

        default:
            return "There was an error we didn't bother describing";
            break;
    }
}

bool
USBBackend::getDeviceID( std::string& deviceID )
{
    deviceID = myDeviceID;
    return true;
}

// vim: et sw=4
