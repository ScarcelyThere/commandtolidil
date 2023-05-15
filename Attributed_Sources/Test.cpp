/* Test.cpp: Tests DeskJet3600 and Pen parsing ability
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

// A simple test harness for the Pen class.
#include "Pen.h"
#include "DeskJet3600.h"
#include <iostream>

const int numTests = 3;
char testStatus[numTests][256] = {
    // A simple test, from an actual printer, without much around it
    "Test;S:0380008000020020002c1481017c2501057;more stuff",
    // A more complex test, taken from a printer that had a cartridge removed (only
    //  the Black cartridge was installed)
    "MFG:HP;MDL:Deskjet F4100 series;CMD:LDL,MLC,PML,DYN;CLS:PRINTER;1284.4DL:4d,4e,1;SN:CN7AV4S3ZZ04TJ;S:0380008000020020002c1480006c0000000;Z:007,0A20000;",
    // I've randomly changed digits in the following case to be sure we will not crash.
    "Test;S:038123812342000A34bc1481017c2501057;more stuff"
};

int
main (void)
{
    DeskJet3600 testPrinter[numTests];

    for (int i = 0 ; i < numTests ; ++i)
    {
        testPrinter[i].updateStatus (testStatus[i]);

        std::cout << " ********************* Test no. " << std::to_string (i)
                  << std::endl;

        for (Pen* p = testPrinter[i].firstPen () ; p != NULL ;
                p = testPrinter[i].nextPen ())
        {
            std::cout << " *** Pen found!" << std::endl;
            std::cout << "  Level: " << std::to_string (p->getLevel ())
                      << std::endl;
            std::cout << "  Name: " << p->name () << std::endl;
            std::cout << "   Hex: " << p->toHex () << std::endl;
            std::cout << "  Type: " << p->markerType () << std::endl;
        }
    }
}

// vim: et sw=4
