/* Pen.cpp: Implements the Pen class, which parses part of a HP printer's status and
 *  represents a cartridge, ink tank, or print head
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

// iostream is only needed for debugging statements here.
#include <iostream>
#include <sstream>
#include "Pen.hpp"

Pen::Pen( )
{
    level        = 0;
    myName       = "Error";
    myMarkerType = "";
    myHex        = "";
}

Pen::Pen( Pen& source )
{
    level        = source.level;
    myName       = source.myName;
    myMarkerType = source.myMarkerType;
    myHex        = source.myHex;
}

Pen&
Pen::operator=( Pen& source )
{
    if ( this != &source )
    {
        level        = source.level;
        myName       = source.myName;
        myHex        = source.myHex;
        myMarkerType = source.myMarkerType;
    }

    return *this;
}

Pen::Pen( const unsigned int status )
{
    // Assume a marking Pen; this is the most common case
    bool isMarking = true;

    // Lowest byte is the level
    level = status & 0xFF;

    // The type ("kind" in HP parlance) is the top two bits
    unsigned int kind = ( status & 0xC0000000 ) >> 30;
    switch ( kind )
    {
        case Printhead:
        case Tank:
        case Cartridge:
            myMarkerType = "ink";
            break;
        default:
            // This isn't a marking Pen. We'll know more about
            //  its validity after we get the color.
            myMarkerType = "";
            isMarking    = false;
            break;
    }

    // The color ("type" in HP parlance) is the next six bits
    unsigned int hpType = ( status & 0x3F000000 ) >> 24;
    switch ( hpType )
    {
        case Black:
            myName = "Black";
            myHex  = "#000000";
            break;
        case CMY:
            myName = "Tri-color";
            myHex  = "#00FFFF#FF00FF#FFFF00";
            break;
        case KCM:
            myName = "Photo";
            myHex  = "#000000#00FFFF#FF00FF";
            break;
        default:
            // Any marking Pen without a valid color isn't
            //  valid itself.
            if ( isMarking )
            {
                std::cerr << "DEBUG: Marking pen with no color found!"
                          << std::endl;
                throw InvalidPenException ( );
            }

            break;
    }
}

std::string
Pen::markerType( )
{
    return myMarkerType;
}

std::string
Pen::toHex( )
{
    return myHex;
}

std::string
Pen::name( )
{
    return myName;
}

Pen::Pen( const unsigned short status )
{
    // TODO: Implement this so we can support more printers
    ( void )status;
}

std::string
Pen::getLevel( )
{
    std::stringstream levelConverter;
    levelConverter << level;
    return levelConverter.str( );
}

// vim: et sw=4
