/* HpmudBackend.cpp: Implements the HpmudBackend, which reads a printer's status
 *  using HPLIP's hpmud library
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

#include <iostream>
#include <string>
#include "hpmud.h"
#include "HpmudBackend.hpp"

HpmudBackend::HpmudBackend( std::string& deviceUri )
{
    HPMUD_RESULT result;
    deviceOpen = false;

    // I do not know if hpmud_query_model alters the URI, so we'll keep
    //  our own copy of the URI.
    int smallerStringLength = deviceUri.length( );
    // We account for the NULL-termination space our copy will need.
    if ( uriLength - 1 < smallerStringLength )
        smallerStringLength = uriLength - 1;

    int i;
    for ( i = 0 ; i < smallerStringLength ; i++ )
        uri[i] = deviceUri[i];
    uri[i] = '\0';

    result = hpmud_query_model( uri, &modelAttrs );
    if ( result == HPMUD_R_OK )
        validUri = true;
    else
    {
        validUri = false;
        std::cerr << messageFromHpmudResult( result ) << std::endl;
    }
}

// TODO: Internationalize any of this at all.
std::string
HpmudBackend::messageFromHpmudResult( HPMUD_RESULT result )
{
    switch ( result )
    {
        case HPMUD_R_OK:
            return "INFO: No errors from HP's HPMUD backend";
            break;

        case HPMUD_R_IO_ERROR:
            return "ERROR: Could not communicate with the printer";
            break;

        case HPMUD_R_INVALID_DEVICE:
            return "ERROR: Not a valid printer";
            break;

        case HPMUD_R_INVALID_URI:
            return "ERROR: Not a valid URI for a printer";
            break;

        case HPMUD_R_INVALID_DESCRIPTOR:
            return "ERROR: Not a valid descriptor for a printer";
            break;

        case HPMUD_R_INVALID_LENGTH:
            return "ERROR: HP's backend reported an invalid length";
            break;

        case HPMUD_R_INVALID_DEVICE_OPEN:
            return "ERROR: Invalid device open";
            break;

        default:
            return "ERROR: HP's HPMUD backend reported an error";
            break;
    }
}

HpmudBackend::~HpmudBackend( )
{
    if ( deviceOpen )
    {
        // Just try anyway.
        std::cerr << "DEBUG: HPMUD device was still open" << std::endl;
        ( void )hpmud_close_device( device );
    }
}

bool
HpmudBackend::getDeviceID( std::string& deviceID )
{
    char buf[1024];
    int bufSize = sizeof ( buf ) - 1;
    int bytesRead;

    if ( !validUri )
        return false;

    HPMUD_RESULT result;
    result = hpmud_open_device( uri, modelAttrs.prt_mode, &device );

    if ( result != HPMUD_R_OK )
    {
        std::cerr << messageFromHpmudResult( result ) << std::endl;
        return false;
    }

    result = hpmud_get_device_id( device, buf, bufSize,
            &bytesRead );
    // We note whether or not we were able to close the device.
    //  We'll try again later in the destructor if need be.
    deviceOpen = ( HPMUD_R_OK != hpmud_close_device( device ) );

    // No valid result?
    if ( result != HPMUD_R_OK || bytesRead < 1 )
        return false;

    // HP makes sure the status is NULL-terminated for us
    deviceID = buf;
    return true;
}

// vim: et sw=4
