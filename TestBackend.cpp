/* TestBackend.cpp: Testing Backend for further DeskJet3600 and Pen tests
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

#include <iostream>
#include "TestBackend.h"

static const int numTests = 3;
static const char testStatus[numTests][256] = {
    // A simple test, from an actual printer, without much around it
    "Test;S:0380008000020020002c1481017c2501057;more stuff",
    // A more complex test, taken from a printer that had a cartridge removed (only
    //  the Black cartridge was installed)
    "MFG:HP;MDL:Deskjet F4100 series;CMD:LDL,MLC,PML,DYN;CLS:PRINTER;1284.4DL:4d,4e,1;SN:CN7AV4S3ZZ04TJ;S:0380008000020020002c1480006c0000000;Z:007,0A20000;",
    // I've randomly changed digits in the following case to be sure we will not crash.
    "Test;S:038123812342000A34bc1481017c2501057;more stuff"
};

TestBackend::TestBackend () = default;
TestBackend::~TestBackend () = default;

bool
TestBackend::getDeviceID (std::string& deviceID)
{
    deviceID = testStatus[0];
    return true;
}

// vim: et sw=4
