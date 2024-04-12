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
#include "Pen.hpp"
#include "Backend.hpp"

class DeskJet3600
{
    public:
        DeskJet3600 ();
        DeskJet3600 (std::string uri);
        DeskJet3600 (DeskJet3600&);

        ~DeskJet3600 ();

        void printAlignmentPage ();
        void clean ();

        int update ();

        class Pens
        {
            public:
                Pens () { penArray = NULL, numPens = 0; curPen = 0; };
                Pens (Pen** pens, unsigned int length,
                      unsigned int current) {
                    penArray = pens;
                    numPens  = length;
                    curPen   = current;
                };
                Pens (Pens& other) {
                    penArray = other.penArray;
                    numPens  = other.numPens;
                    curPen   = other.curPen;
                };
                // We do not own the penArray, so we do nothing to it.
                ~Pens () = default;

                bool  operator!= (Pens&);
                Pens& operator++ () {
                    if (curPen < numPens)
                        curPen++;

                    return *this;
                };
                Pen* operator* () { return penArray[curPen]; };

            private:
                unsigned int numPens;
                unsigned int curPen;

                // This is owned by DeskJet3600, so we do not allocate
                //  or delete it.
                Pen** penArray;
        };

        Pens begin () { return Pens (pens, numPens, 0      ); };
        Pens end   () { return Pens (pens, numPens, numPens); };

    private:
        int parseStatus ();
        void clearPens ();

        // Was this initialized?
        bool validStatus;

        Backend* backend;

        std::string deviceUri;

        Pen* pens[2]; 
        unsigned int numPens;
        unsigned int curPen;

        char status[1024];
};

#endif

// vim: et sw=4
