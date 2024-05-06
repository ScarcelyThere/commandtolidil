/* LidilCommand.hpp: Declaration for a class that builds and sends LIDIL commands
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

#ifndef _LIDILCOMMAND_H
#define _LIDILCOMMAND_H

#include <iostream>

typedef enum
{
    Clean          = 8,
    PrintAlignPage = 13
}
LidilCmdType;

class LidilCommand
{
    public:
        LidilCommand ()  = default;
        ~LidilCommand () = default;

        LidilCommand (std::ostream*);

        bool send (LidilCmdType);

    private:
        const size_t minPacketLen = 16;
        const char   packetFrame  = '$';

        std::ostream* destination;
};

#endif

// vim: et sw=4
