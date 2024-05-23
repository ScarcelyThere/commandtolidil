/* Pen.h: Declarations for the Pen class
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

/* I guess here goes nothing?
 */
#ifndef PEN_H
#define PEN_H

#include <string>

class Pen
{
    public:
        typedef enum
        {
            None,
            Black,
            CMY,
            KCM,               // Often called a Photo cartridge
            Error = 0x3f
        } Color;

        typedef enum
        {
            Invalid,
            Printhead,
            Tank,
            Cartridge,
            Unknown = 0x3e
        } Type;    // There are MANY more types, but we care not (yet)

        // Create an uninitialized Pen
        Pen ( );

        Pen ( Pen& );

        Pen& operator=( Pen& );
 
        // TODO: Implement this
        // Create a Pen from a two-byte S-status
        Pen ( const unsigned short status );
 
        // Create a Pen from a Rev. 3 S-status, which is 4 bytes
        Pen ( const unsigned int status );

        // Accessors
        std::string getLevel( );

        // Returns a hexadecimal color code for the Pen
        // TODO: This is pretty CUPS-y. Does this properly belong here?
        std::string toHex( );

        // The pen's name (a human-readable identifier)
        // TODO: i18n this would be nice.
        std::string name( );

        // The pen's type ("ink", for example)
        std::string markerType( );

    private:
        int level;

        std::string myName;
        std::string myMarkerType;
        std::string myHex;
};

class InvalidPenException { };

#endif

// vim: et sw=4
