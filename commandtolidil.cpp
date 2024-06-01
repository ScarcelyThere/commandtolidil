/* commandtolidil: an ink status utility for CUPS-powered DeskJet F4180 all-in-ones
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

#include <iostream>
#include <fstream>
#include <string>
#include <signal.h>
#include "Pen.hpp"
#include "DeskJet3600.hpp"
#include "CUPS_Utilities.hpp"

int
main( int argc, char* argv[] )
{
    signal( SIGPIPE, SIG_IGN );

    if ( argc > 7 || argc < 6 )
    {
        // No way we're being called from CUPS.
        std::cerr << "ERROR: insufficient or too many arguments" << std::endl;
        return 1;
    }

    char* deviceUri = getenv( "DEVICE_URI" );
    if ( ! deviceUri )
    {
        std::cerr << "ERROR: no DEVICE_URI environment variable" << std::endl;
        return 1;
    }

    // Should there be no file argument, we read from standard input. I'm
    //  assuming any issues with the supplied filename doesn't imply defer
    //  to standard input, but complain.
    std::istream* jobFile = &std::cin;
    bool readingFromCmdFile = false;
    if ( argc == 7 )
    {
        jobFile = new std::ifstream ( argv[6] );
        if ( ! *jobFile )
        {
            std::cerr << "ERROR: Could not open file containing commands" << std::endl;
            delete jobFile;
            return 1;
        }
        else
            readingFromCmdFile = true;
    }

    int retVal = 0;
    try
    {
        DeskJet3600 printer ( deviceUri );

        std::string jobLine;
        while ( *jobFile >> jobLine )
        {
            std::cerr << "DEBUG: jobLine is " << jobLine << std::endl;

            // "Clean all" instead of "Clean" as specified threw me off a little
            if ( jobLine == "Clean" )
                printer.clean( );
            else if ( jobLine == "PrintSelfTestPage" )
                printer.printAlignmentPage( );
            // We should also support this command.
            else if ( jobLine == "PrintAlignmentPage" )
                printer.printAlignmentPage( );
            else if ( jobLine == "ReportLevels" )
            {
                if ( printer.update( ) )
                    sendCupsLevels( printer );
                else
                {
                    // This is probably bad enough that we just stop.
                    std::cerr << "ERROR: Could not read supply levels from the printer"
                              << std::endl;
                    retVal = 1;
                    break;
                }
            }
            // Any other line is a comment or unsupported. Ignore it.
        }
    }
    catch ( BackendException& e )
    {
        std::cerr << "ERROR: " << e.what( ) << std::endl;
        retVal = 1;
    }

    if ( readingFromCmdFile )
        delete jobFile;

    return retVal;
}

// vim: et sw=4
