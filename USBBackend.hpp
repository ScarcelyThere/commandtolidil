/* USBBackend.h: Declarations for the USBBackend class
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

#ifndef USBBACKEND_H
#define USBBACKEND_H

#include <string>
#include <cups/sidechannel.h>
#include "Backend.hpp"

class USBBackend : public Backend
{
    public:
        USBBackend( );
        bool getDeviceID( std::string& );

        bool receive( const char*, size_t );
        bool receive( const char*, size_t, int );

    private:
        static const int bufSize = 1024;
        char buf[bufSize];
        std::string myDeviceID;

        std::string errorMsgFromStatus( cups_sc_status_t );
};

#endif

// vim: et sw=4
