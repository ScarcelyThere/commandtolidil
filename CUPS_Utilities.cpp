/* CUPS_Utilities.cpp: implementation of common CUPS stuff
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
#include <string>
#include "Pen.hpp"
#include "DeskJet3600.hpp"

void
sendCupsLevels( DeskJet3600& printer )
{
    // I thought about a CUPS object, but why?
    std::string markerTypes  = "ATTR:marker-types=";
    std::string markerLevels = "ATTR:marker-levels=";
    std::string markerColors = "ATTR:marker-colors=";
    std::string markerNames  = "ATTR:marker-names=";

    bool firstPass = true;
    for ( Pen* curPen = printer.firstPen( ) ; printer.areMorePens( ) ;
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
        markerLevels += curPen->getLevel( );
        markerColors += curPen->toHex( );
        markerNames  += curPen->name( );
    }

    std::cerr << markerTypes  << std::endl;
    std::cerr << markerLevels << std::endl;
    std::cerr << markerColors << std::endl;
    std::cerr << markerNames  << std::endl;
}

// vim: et sw=4
