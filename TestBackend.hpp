/* TestBackend.h: a Backend just for testing
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

#ifndef TESTBACKEND_H
#define TESTBACKEND_H

#include <string>
#include "Backend.hpp"

class TestBackend : public Backend
{
    public:
        TestBackend( );

        bool getDeviceID( std::string& );

        bool receive( const char*, size_t );
        bool receive( const char*, size_t, int );

    private:
        std::string testCases[3];
        std::string testDescriptions[3];
};

#endif

// vim: et sw=4
