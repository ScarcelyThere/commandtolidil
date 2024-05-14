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

HpmudBackend::HpmudBackend( const char* deviceUri )
{
    deviceOpen = false;

    HPMUD_RESULT result;
    uri = std::string ( deviceUri );

    result = hpmud_query_model( uri.data( ), &modelAttrs );
    validUri = ( result == HPMUD_R_OK );
}

HpmudBackend::~HpmudBackend( )
{
    if( deviceOpen )
    {
        // Just try anyway.
        std::cerr << "DEBUG: HPMUD device was still open" << std::endl;
        ( void )hpmud_close_device (device);
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
    result = hpmud_open_device( uri.data( ),
            modelAttrs.prt_mode,
            &device );

    if ( result != HPMUD_R_OK )
        return false;

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
