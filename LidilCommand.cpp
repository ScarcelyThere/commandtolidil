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

#include "LidilCommand.hpp"
#include <iostream>

LidilCommand::LidilCommand (std::ostream* dest)
{
    if (!dest)
    {
        // TODO: Throw something here
    }

    destination = dest;
}

bool
LidilCommand::send (LidilCmdType cmd)
{
    char* packet = new char[minPacketLen];

    // Frame the packet
    packet[0]                = packetFrame;
    packet[minPacketLen - 1] = packetFrame;

    // The second two bytes represent the length
    packet[1] = 0;
    packet[2] = minPacketLen;

    // Fourth byte is zero
    packet[3] = 0;

    // Fifth byte is also zero, for a command
    packet[4] = 0;

    // Sixth byte is the actual command
    packet[5] = cmd;

    // Pad to the end?
    for (int i = 6 ; i < minPacketLen - 1 ; i++)
        packet[i] = 0;

    destination->write (packet, minPacketLen);
    destination->flush ();

    return true;
}

/*
void
DeskJet3600::printAlignmentPage ()
{
    // Send the LIDIL packet to print the built-in page to the rest of CUPS
    //  via standard output
    
    // This is only valid for command packets, I think.
    // Packet format: F< size >< 0><cm><ac><      ><      >
    //  cm: Packet type (command)
    //  ac: The actual command byte
    // Header:        <          10 bytes                 >
    char command[] = "$\x00\x10\x00\x00\x0c\x00\x00\x00\x00\x11\xff\xff\xff\xff$";
    std::cout.write (command, sizeof (command));
    std::cout.flush ();
}

void
DeskJet3600::clean ()
{
    char command[] = "$\x00\x10\x00\x00\x08\x00\x00\x00\x00\x03\xff\xff\xff\xff$";
    std::cout.write (command, sizeof (command));
    std::cout.flush ();
}
*/



// vim: et sw=4
