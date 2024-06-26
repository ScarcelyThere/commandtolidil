/* Test.cpp: Tests DeskJet3600 and Pen parsing ability
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

// A simple test harness for the Pen class.
#include "Pen.hpp"
#include "DeskJet3600.hpp"
#include "CUPS_Utilities.hpp"
#include <iostream>

int
main( void )
{
    // An empty string instantiates a testing backend.
    DeskJet3600 testPrinter ( "" );

    if ( testPrinter.update( ) )
        sendCupsLevels( testPrinter );
    else
        std::cerr << "DEBUG: testPrinter.update( ) failed!" << std::endl;

    return 0;
}

// vim: et sw=4
