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
#include <cstring>
#include <signal.h>
#include "Pen.hpp"
#include "DeskJet3600.hpp"

static void
sendCupsLevels( DeskJet3600& printer )
{
    // I thought about a CUPS object, but why?
    std::string markerTypes  = "ATTR:marker-types=";
    std::string markerLevels = "ATTR:marker-levels=";
    std::string markerColors = "ATTR:marker-colors=";
    std::string markerNames  = "ATTR:marker-names=";
    
    bool firstPass = true;
    for ( Pen* curPen = printer.firstPen( ) ; printer.morePens( ) ;
               curPen = printer.nextPen( ) )
    {
        if ( firstPass )
        {
            // We'll need commas after this
            firstPass = false;
        }
        else
        {
            // commas needed
            markerTypes  += ",";
            markerLevels += ",";
            markerColors += ",";
            markerNames  += ",";
        }

        markerTypes  += curPen->markerType( );
        markerLevels += curPen->getLevel ();
        markerColors += curPen->toHex( );
        markerNames  += curPen->name( );
    }

    std::cerr << markerTypes  << std::endl;
    std::cerr << markerLevels << std::endl;
    std::cerr << markerColors << std::endl;
    std::cerr << markerNames  << std::endl;
}

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

    DeskJet3600 printer ( deviceUri );

    std::string jobLine;
    int retVal = 0;
    while ( *jobFile >> jobLine )
    {
        std::cerr << "DEBUG: jobLine is " << jobLine << std::endl;

        // "Clean all" instead of "Clean" as specified threw me off a little
        if ( jobLine == "Clean" )
            printer.clean ();
        else if ( jobLine == "PrintSelfTestPage" )
            printer.printAlignmentPage ();
        else if ( jobLine == "ReportLevels" )
        {
            if( printer.update( ) )
                sendCupsLevels( printer );
            else
            {
                // This is probably bad enough that we just stop.
                std::cerr << "ERROR: Could not get supply levels" << std::endl;
                retVal = 1;
                break;
            }
        }
        // Any other line is unsupported or a comment. Ignore it.
    }

    if( readingFromCmdFile )
        delete jobFile;

    return retVal;
}

// vim: et sw=4
