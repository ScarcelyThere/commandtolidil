/* HpmudBackend.h: Declarations for the HpmudBackend class
 *  Copyright (C) 2023 Scarcely There.
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

#ifndef HPMUDBACKEND_H
#define HPMUDBACKEND_H

#include <string>
#include "hpmud.h"
#include "Backend.hpp"

class HpmudBackend : public Backend
{
    public:
        HpmudBackend( );
        HpmudBackend( std::string& );

        ~HpmudBackend( );

        bool getDeviceID( std::string& );

        bool receive( const char*, size_t );
        bool receive( const char*, size_t, int );

    private:
        std::string messageFromHpmudResult( HPMUD_RESULT );

        static const int uriLength = 1024;
        char uri[uriLength];

        hpmud_model_attributes modelAttrs;
        HPMUD_DEVICE device;

        bool validUri;
        bool deviceOpen;
};

#endif

// vim: et sw=4
