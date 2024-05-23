/* DeskJet3600.cpp: Implements the DeskJet3600, which reads a status from a
 *  Backend and parses it into Pens
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

#include <iostream>
#include <sstream>
#include <string>
#include "DeskJet3600.hpp"
#include "USBBackend.hpp"

#ifdef TESTING
#  include "TestBackend.hpp"
#endif

#ifdef BUILD_HPMUD
#  include "HpmudBackend.hpp"
#endif

DeskJet3600::DeskJet3600( )
{
    backend = NULL;
    numPens = 0;
    curPen  = 0;
}

DeskJet3600::DeskJet3600( std::string uri )
{
    deviceUri = uri;
    numPens   = 0;
    curPen    = 0;

    if ( deviceUri.length( ) > 2 &&
         deviceUri.compare( 0, 3, "usb" ) == 0 )
    {
        backend = new USBBackend ( );
        std::cerr << "DEBUG: USB backend selected" << std::endl;
    }
#ifdef BUILD_HPMUD
    else if ( deviceUri.length( ) > 1 &&
              deviceUri.compare( 0, 2, "hp" ) == 0 )
    {
        backend = new HpmudBackend( uri.data( ) );
        std::cerr << "DEBUG: Selected HP backend" << std::endl;
    }
#endif
#ifdef TESTING
    else
    {
        backend = new TestBackend( );
        std::cerr << "DEBUG: Using test backend" << std::endl;
    }
#else
    else
        std::cerr << "ERROR: No compatible backend" << std::endl;
#endif
}

DeskJet3600::DeskJet3600( DeskJet3600& source )
{
    backend   = source.backend;
    deviceUri = source.deviceUri;
    numPens   = source.numPens;
    curPen    = source.curPen;

    for ( unsigned int i = 0 ; i < source.numPens ; i++ )
        pens[i] = new Pen( *source.pens[i] );
}

DeskJet3600::~DeskJet3600( )
{
    if ( backend )
        delete backend;

    clearPens( );
}

void
DeskJet3600::clearPens( )
{
    for ( unsigned int i = 0 ; i < numPens ; i++ )
        delete pens[i];

    numPens = 0;
    curPen  = 0;
}

Pen*
DeskJet3600::firstPen( )
{
    curPen = 0;
    return nextPen( );
}

bool
DeskJet3600::areMorePens( )
{
    return ( curPen <= numPens );
}

Pen*
DeskJet3600::nextPen( )
{
    return pens[curPen++];
}

int
DeskJet3600::update( )
{
    if ( backend == NULL )
        return 0;

    if ( backend->getDeviceID( deviceID ) )
    {
        clearPens( );
        return parseStatus( );
    }
    else
        return 0;
}

int
DeskJet3600::parseStatus( )
{
    // Both offsets are in hexadecimal digits
    const int  penDataOffset = 16,
               penDataLength = 8;

    size_t offset;

    // Where's the relevant hex status string?
    offset = deviceID.find( ";S:" );
    if ( offset == deviceID.npos )
        return 0;

    offset += 3;
    if ( deviceID.length( ) < offset + penDataOffset )
        return 0;

    unsigned int revision;
    std::stringstream ( deviceID.substr( offset, 2 ) ) >> std::hex
        >> revision;
    offset += 2;

    // DeskJet 3600 is revision 3, so anything else won't work here
    if ( revision != 3 )
        return 0;

    unsigned int penCount,
                 curRawPen;

    // The pen data is found 16 characters after the revision
    offset += penDataOffset;

    // First nybble is the number of pens, which we obtain and then
    //  skip over
    std::stringstream ( deviceID.substr( offset++, 1 ) ) >> std::hex
        >> penCount;

    // DeskJet 3600 has two cartridge slots. Any more and it's not that
    //  printer model.
    if ( penCount > 2 )
        return 0;

    Pen* pen;
    for ( unsigned int i = 0 ; i < penCount ; i++) 
    {
        std::stringstream( deviceID.substr( offset, penDataLength ) )
            >> std::hex >> curRawPen;

        offset += penDataLength;

        try
        {
            pen = new Pen ( curRawPen );
            pens[numPens++] = pen;
        }
        catch ( const InvalidPenException &e )
        {
            std::cout << "DEBUG: Invalid Pen discovered" << std::endl;
        }
    }

    return 1;
}

void
DeskJet3600::printAlignmentPage( )
{
    char packet[minLdlPktLen];

    buildLidilHeader( commandType, packet );
}

void
DeskJet3600::clean( )
{
    char packet[minLdlPktLen];

    buildLidilHeader( resetType, packet );
}

void
DeskJet3600::buildLidilHeader( int type, char* buffer )
{
    // Frame the packet
    buffer[0] = '$';

    // Next two bytes are the size
    buffer[1] = '\0';
    buffer[2] = minLdlPktLen;

    // Byte four is zero?
    buffer[3] = '\0';

    // Fifth byte is the packet type
    buffer[4] = type;

    // Bytes seven through ten are still a mystery to me.
    for ( size_t i = 5 ; i < 10 ; i++ )
        buffer[i] = '\0';
}

void
DeskJet3600::finishLidilPacket( size_t packetLength,
                                int offset,
                                char* buffer )
{
    for ( int i = offset ; i < packetLength - 2 ; i++ )
        buffer[i] = padByte;

    buffer[packetLength - 1] = '$';
}

// vim: et sw=4
