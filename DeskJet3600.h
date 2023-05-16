/* DeskJet3600.h: Declarations for the DeskJet3600 class, and a few lookup arrays
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

#ifndef _DESKJET3600_H
#define _DESKJET3600_H

#include <string>
#include "Pen.h"
#include "Backend.h"

class DeskJet3600
{
    public:
        DeskJet3600 ();
        DeskJet3600 (std::string uri);
        DeskJet3600 (DeskJet3600& source);

        ~DeskJet3600 ();

        void printAlignmentPage ();
        void clean ();

        int updateStatus ();
        bool isValidStatus ();

        Pen* firstPen ();
        Pen* nextPen  ();

        // TODO: get rid of this. It's for testing
        int updateStatus (char* s);

    private:
        int parseStatus ();

        // Was this initialized?
        bool validStatus;

        Backend* backend;

        std::string deviceUri;

        // This printer will never have more than 2 cartridges.
        int  numPens;
        Pen* pens[2];
        int  curPen;    // for the iterator, as it were

        char status[1024];
        int statusLen;
};

#endif

// vim: et sw=4
