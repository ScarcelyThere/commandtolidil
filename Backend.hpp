/* Backend.h: A base for Backend objects, which communicate with the printer
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

#ifndef BACKEND_H
#define BACKEND_H

#include <string>

class Backend
{
    public:
        Backend( );
        virtual ~Backend( );

        // This is all we really need from outside.
        virtual bool getDeviceID( std::string& ) = 0;
};

class BackendException
{
    public:
        BackendException( std::string );
        std::string what( );

    private:
        std::string errorMsg;
};

#endif

// vim: et sw=4
