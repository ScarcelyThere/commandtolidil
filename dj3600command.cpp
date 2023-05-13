#include <iostream>
#include <fstream>
#include <cstring>
#include <signal.h>
#include "Pen.h"
#include "DeskJet3600.h"

// Just a little forward declaration of a local function
void sendCupsLevels (DeskJet3600&);

int main (int argc, char* argv[])
{
    int retVal = 0;

    signal (SIGPIPE, SIG_IGN);

    if (argc != 6 && argc != 7)
    {
        // No way we're being called from CUPS.
        std::cerr << "ERROR: insufficient or too many arguments" << std::endl;
        return 1;
    }

    char* deviceUri = getenv ("DEVICE_URI");
    if (!deviceUri)
    {
        std::cerr << "ERROR: no DEVICE_URI environment variable" << std::endl;
        return 1;
    }

    // Sixth argument from CUPS is the job file. Should it not be present, we
    //  accept our commands on standard input.

    bool destroyFileLater = false;
    // This feels like a gross way to do this, but we use stream-based I/O
    //  elsewhere, so we should be consistent and not go opening stdin on
    //  our own.
    std::istream* jobFile = &std::cin;
    if (argc == 7)
    {
        jobFile = new std::fstream (argv[6], std::ios_base::in);
        if (!jobFile->fail ())
        {
            // The file couldn't be opened, for whatever reason, so we
            //  bail out.
            delete jobFile;
            return 1;
        }
        else
            // Open was successful, make sure we destroy it before we're
            //  done
            destroyFileLater = true;
    }

    DeskJet3600 printer (deviceUri);

    char jobLine[1024];
    while (jobFile->getline (jobLine, 1023))
    {
        std::cerr << "DEBUG: jobLine is " << jobLine << std::endl;

        // "Clean all" instead of "Clean" as specified threw me off a little
        if (0 == strcmp (jobLine, "Clean all"))
            printer.clean ();
        else if (0 == strcmp (jobLine, "PrintSelfTestPage"))
            printer.printAlignmentPage ();
        else if (0 == strcmp (jobLine, "ReportLevels"))
        {
            if (printer.updateStatus ())
                sendCupsLevels (printer);
            else
            {
                // This is probably bad enough that we just stop.
                retVal = 1;
                std::cerr << "ERROR: Could not get supply levels" << std::endl;
                break;
            }
        }
        // Any other line is unsupported or a comment. Ignore it.
        //  Fun aside, other backends include cups-private.h and use
        //  supplied functions there to parse these. Can we use them?
    }

    if (destroyFileLater)
        delete jobFile;

    return retVal;
}

void
sendCupsLevels (DeskJet3600& printer)
{
    // I thought about a CUPS object, but why?
    std::string markerTypes  = "ATTR:marker-types=";
    std::string markerLevels = "ATTR:marker-levels=";
    std::string markerColors = "ATTR:marker-colors=";
    std::string markerNames  = "ATTR:marker-names=";
    
    Pen* curPen;
    bool firstPass = true;
    for (curPen = printer.firstPen () ; curPen != NULL ;
            curPen = printer.nextPen ())
    {
        if (firstPass)
        {
            // We'll need commas after this
            firstPass = false;
        }
        else
        {
            // commas needed
            markerTypes  += ",";
            markerLevels += ",";
            markerColors += ",";
            markerNames  += ",";
        }

        markerTypes  += curPen->markerType ();
        markerLevels += std::to_string (curPen->getLevel ());
        markerColors += curPen->toHex ();
        markerNames  += curPen->name ();
    }

    std::cerr << markerTypes  << std::endl;
    std::cerr << markerLevels << std::endl;
    std::cerr << markerColors << std::endl;
    std::cerr << markerNames  << std::endl;
}

// vim: et sw=4
