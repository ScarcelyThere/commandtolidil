/* Pen.cpp: Implements the Pen class, which parses part of a HP printer's status and
 *  represents a cartridge, ink tank, or print head
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

#include "Pen.hpp"

Pen::Pen () :
    level {0},

    myName       {"Error"},
    myMarkerType {""},
    myHex        {""}
{ }

Pen::Pen (Pen& source) :
    level        {source.getLevel ()},

    myName       {source.name ()      },
    myMarkerType {source.markerType ()},
    myHex        {source.toHex ()     }
{ }

Pen& Pen::operator= (Pen& source)
{
    if (this == &source)
        return *this;

    level = source.getLevel ();

    myName       = source.name ();
    myHex        = source.toHex ();
    myMarkerType = source.markerType ();

    return *this;
}

Pen::Pen (const unsigned int status) : Pen ()
{
    // Lowest byte is the level
    level = status & 0xFF;

    // The type ("kind" in HP parlance) is the top two bits
    unsigned int kind = (status & 0xC0000000) >> 30;
    switch (kind)
    {
        case Type::Printhead:
        case Type::Tank:
        case Type::Cartridge:
            myMarkerType = "ink";
            break;
        default:
            throw InvalidPenException ();
            break;
    }

    // The color ("type" in HP parlance) is the next six bits
    unsigned int hpType = (status & 0x3F000000) >> 24;
    switch (hpType)
    {
        case Color::Black:
            myName = "Black";
            myHex  = "#000000";
            break;
        case Color::CMY:
            myName = "Tri-color";
            myHex  = "#00FFFF#FF00FF#FFFF00";
            break;
        case Color::KCM:
            myName = "Photo";
            myHex  = "#000000#00FFFF#FF00FF";
            break;
        default:
            throw InvalidPenException ();
            break;
    }
}

std::string
Pen::markerType ()
{
    return myMarkerType;
}

std::string
Pen::toHex ()
{
    return myHex;
}

std::string
Pen::name ()
{
    return myName;
}

Pen::Pen (const unsigned short status) : Pen ()
{
    // TODO: Implement this so we can support more printers
    (void)status;
};

int
Pen::getLevel ()
{
    return level;
}

// vim: et sw=4
