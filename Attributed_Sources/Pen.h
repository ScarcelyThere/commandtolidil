/* 
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

/* I guess here goes nothing?
 */
#ifndef _PEN_H
#define _PEN_H

#include <string>

class Pen
{
    public:
        typedef enum : unsigned int
        {
            None,
            Black,
            CMY,
            KCM,               // Often called a Photo cartridge
            lastValidColor,    // Bounds-checking for the number we receive
            Error = 0x3f,
            maxColor
        } Color;

        typedef enum : unsigned int
        {
            Invalid,
            Printhead,
            Tank,
            Cartridge,
            lastValidType,     // Bounds-checking for the number we receive
            Unknown = 0x3e,
            maxType
        } Type;    // There are MANY more types, but we care not (yet)

        typedef struct ColorInfo_S
        {
            char rgbDesc[22];    // long enough for 18 hex digits, 3 octothorpes,
                                 //  and NULL-termination
            char enDesc[20];     // Should be long enough for any English
                                 //  description
            Color color;
        } ColorInfo;

        typedef struct TypeInfo_S
        {
            char name[20];
            Type type;
        } TypeInfo;

        // Create an uninitialized Pen
        Pen ();

        Pen (Pen&);

        Pen& operator= (Pen&);
 
        // TODO: Implement this
        // Create a Pen from a two-byte S-status
        Pen (const unsigned short status);
 
        // Create a Pen from a Rev. 3 S-status
        Pen (const unsigned int status);

        // Accessors
        Color getColor ();
        Type  getType  ();
        int   getLevel ();

        bool  exists   ();

        // Returns a hexadecimal color code for the Pen
        // TODO: This is pretty CUPS-y. Does this properly belong here?
        std::string toHex ();

        // The pen's name (a human-readable identifier)
        // TODO: i18n this would be nice.
        std::string name ();

        // The pen's type ("ink", for example)
        std::string markerType ();

    private:
        Color color;
        Type  type;
        int   level;

        // These should remain in sync with their respective enum declarations.
        static constexpr ColorInfo ColorDescs[] = {
            /* None  */ { "",                      "None",      Color::None  },
            /* Black */ { "#000000",               "Black",     Color::Black },
            /* CMY   */ { "#00FFFF#FF00FF#FFFF00", "Tri-color", Color::CMY   },
            /* KCM   */ { "#000000#00FFFF#FF00FF", "Photo",     Color::KCM   }
        };

        static constexpr TypeInfo TypeDescs[] = {
            /* Invalid   */ { "unknown", Type::Invalid   },
            /* Printhead */ { "ink",     Type::Printhead },
            /* Tank      */ { "ink",     Type::Tank      },
            /* Cartridge */ { "ink",     Type::Cartridge }
        };
};

#endif

// vim: et sw=4
