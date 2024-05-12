/* TestBackend.cpp: Testing Backend for further DeskJet3600 and Pen tests
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
#include <ctime>
#include <cstdlib>
#include "TestBackend.hpp"

TestBackend::TestBackend ()
{
    testCases[0] = "Test;S:0380008000020020002c1481017c2501057;more stuff";
    testDescriptions[0] = "Simple test from actual printer";

    testCases[1] = "MFG:HP;MDL:Deskjet F4100 series;CMD:LDL,MLC,PML,DYN;CLS:"
        "PRINTER;1284.4DL:4d,4e,1;S:0380008000020020002c1480006c0000000"
        ";Z:007,0A20000;";
    testDescriptions[1] = "More complex test from printer with only black cartridge";
   
    testCases[2] = "Test;S:038123812342000A34bc1481017c2501057;more stuff";
    testDescriptions [2] = "Invalid case to be sure we don't crash";
}

TestBackend::~TestBackend ()
{ }

bool
TestBackend::getDeviceID (std::string& deviceID)
{
    std::srand (std::time (NULL));
    int testCase = rand () / ((RAND_MAX + 1u) / 3);
    std::cerr << "DEBUG: testCase is " << testCase << std::endl;

    deviceID = testCases[testCase];
    std::cerr << "ERROR: " << testDescriptions[testCase] <<  std::endl;

    return true;
}

// vim: et sw=4
