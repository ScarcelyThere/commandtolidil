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
