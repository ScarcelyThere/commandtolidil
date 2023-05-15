/* HpmudBackend.h: Declarations for the HpmudBackend class
 *  Copyright (C) 2023 Scarcely There.
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

#ifndef _HPMUDBACKEND_H
#define _HPMUDBACKEND_H

#include <string>
#include "hpmud.h"
#include "Backend.h"

class HpmudBackend : public Backend
{
    public:
        HpmudBackend ();
        HpmudBackend (const char*);
        ~HpmudBackend ();

        bool getDeviceID (std::string&);

    private:
        hpmud_model_attributes modelAttrs;
        HPMUD_DEVICE device;
        std::string uri;
        bool validUri;
        bool deviceOpen;
};

#endif

// vim: et sw=4
