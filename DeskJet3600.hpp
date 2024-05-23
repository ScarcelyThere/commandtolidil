/* DeskJet3600.h: Declarations for the DeskJet3600 class, and a few lookup arrays
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

#ifndef DESKJET3600_H
#define DESKJET3600_H

#include <string>
#include "Pen.hpp"
#include "Backend.hpp"

class DeskJet3600
{
    public:
        DeskJet3600 ( );
        DeskJet3600 ( std::string uri );
        DeskJet3600 ( DeskJet3600& );

        ~DeskJet3600 ( );

        void printAlignmentPage( );
        void clean( );

        int update( );

        Pen* firstPen( );
        bool areMorePens( );
        Pen* nextPen ( );

    private:
        static const int    commandType = 0;
        static const int    resetType   = 6;

        static const int    handlePenCmd = 8;
        static const int    cleanLvl1Op  = 2;

        static const int    printBuiltinCmd   = 12;
        static const int    printInternalPgOp = 17;

        static const int    headerLen = 10;
        static const char   padByte   = 0xFF;

        static const size_t minLdlPktLen  = 16;

        int parseStatus( );
        void clearPens( );

        Backend* backend;

        std::string deviceUri;
        std::string deviceID;

        Pen* pens[2]; 
        unsigned int numPens;
        unsigned int curPen;

        void buildLidilHeader( size_t,  // Packet size
                               int,     // Packet type
                               int,     // Packet command
                               char* ); // Buffer

        void finishLidilPacket( size_t,  // Packet size
                                int,     // Offset to begin padding
                                char* ); // Buffer
};

#endif

// vim: et sw=4
