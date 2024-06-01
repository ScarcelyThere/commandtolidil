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

        // Returs true when successful, false when not.
        // The device ID will be in the provided
        //  std::string reference.
        virtual bool getDeviceID( std::string& ) = 0;

        // Sends size_t characters from the const char* buffer.
        //  Returns true when successful, false when not.
        virtual bool send( const char*, size_t );
        // Receives size_t characters to the const char* buffer.
        //  Returns true when anything can be read.
        virtual bool receive( const char*, size_t ) = 0;
        // Receives size_t characters to the const char* buffer.
        //  The final int is a timeout. This will block.
        // Returns true when anything can be read.
        virtual bool receive( const char*, size_t, int ) = 0;
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
