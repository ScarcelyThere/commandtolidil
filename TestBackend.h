/* TestBackend.h: a Backend just for testing
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

#ifndef _TESTBACKEND_H
#define _TESTBACKEND_H

#include <string>
#include "Backend.h"

class TestBackend : public Backend
{
    public:
        TestBackend ();
        ~TestBackend ();

        bool getDeviceID (std::string&);

    private:
        std::string testCases[3] = {
            "Test;S:0380008000020020002c1481017c2501057;more stuff",
            "MFG:HP;MDL:Deskjet F4100 series;CMD:LDL,MLC,PML,DYN;CLS:"
                "PRINTER;1284.4DL:4d,4e,1;S:0380008000020020002c1480006c0000000"
                ";Z:007,0A20000;",
            "Test;S:038123812342000A34bc1481017c2501057;more stuff"
        };

        std::string testDescriptions[3] = {
            "Simple test from actual printer",
            "More complex test from printer with only black cartridge",
            "Invalid case to be sure we don't crash"
        };
};

#endif

// vim: et sw=4
