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

#include "Pen.h"

Pen::Pen () :
    color {Color::None},
    type  {Type::Invalid},
    level {0}
{ }

Pen::Pen (Pen& source) :
    color {source.getColor () },
    type  {source.getType ()  },
    level {source.getLevel () }
{ }

Pen& Pen::operator= (Pen& source)
{
    if (this == &source)
        return *this;

    color = source.getColor ();
    type  = source.getType ();
    level = source.getLevel ();

    return *this;
}

Pen::Pen (const unsigned int status) : Pen ()
{
    // Lowest byte is the level
    level = status & 0xFF;

    // The type ("kind" in HP parlance) is the top two bits
    unsigned int kind = (status & 0xC0000000) >> 30;
    type = static_cast<Type>(kind);     // We verify this next
    switch (kind)
    {
        case Type::Invalid:
            myMarkerType = "Invalid";
            break;
        case Type::Printhead:
        case Type::Tank:
        case Type::Cartridge:
            myMarkerType = "ink";
            break;
        default:
            type = Type::Unknown;
    }

    // The color ("type" in HP parlance) is the next six bits
    unsigned int hpType = (status & 0x3F000000) >> 24;
    color = static_cast<Color>(hpType); // Verifying this next
    switch (hpType)
    {
        case Color::None:
            myName = "None";
            myHex  = "";
            break;
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
        default:
            color = Color::Error;
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

Pen::Color
Pen::getColor ()
{
    return color;
}

bool
Pen::exists ()
{
    // So far, if there's any marking Pen with no color, it
    //  may not be installed and not actually exist in the
    //  device.
    bool marking = type == Type::Cartridge ||
                   type == Type::Tank ||
                   type == Type::Printhead;

    return (marking && color != Color::None
                    && color != Color::Error);
}

Pen::Type
Pen::getType (void)
{
    return type;
}

// vim: et sw=4
