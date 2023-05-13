#include <iostream>
#include <cstring>
#include "DeskJet3600.h"
#include "USBBackend.h"

#ifdef BUILD_HPMUD
#  include "HpmudBackend.h"
#endif

DeskJet3600::DeskJet3600 () :
    validStatus {false},
    backend     {NULL},
    numPens     {0},
    curPen      {0}
{ }

DeskJet3600::DeskJet3600 (std::string uri) :
    validStatus {false},
    deviceUri   {uri},
    numPens     {0},
    curPen      {0}
{
    if (deviceUri.length () > 1 &&
            deviceUri.compare (0, 2, "hp") == 0)
    {
#ifdef BUILD_HPMUD
        backend = new HpmudBackend (uri.data ());
        std::cerr << "DEBUG: Selected HP backend" << std::endl;
#else
        // We're using the HP backend, but we didn't build support
        //  for that. We don't really have any good way to get around
        //  this.
        backend = NULL;
        std::cerr << "ERROR: Detected HP backend, but support was "
                     "not built for that" << std::endl;
#endif
    }
    else
    {
        backend = new USBBackend ();
        std::cerr << "DEBUG: Selected USB backend" << std::endl;
    }
}
 
DeskJet3600::~DeskJet3600 ()
{
    if (backend)
        delete backend;

    for (Pen* p = firstPen () ; p != NULL ; p = nextPen ())
    {
        std::cerr << "DEBUG: Deleting Pen" << std::endl;
        delete p;
    }
}

// TODO: this is gross and only for testing
int
DeskJet3600::updateStatus (char* s)
{
    if (strlen (s) > 1023)
        return 0;

    strcpy (status, s);
    status[1023] = '\0';    // make sure it's NULL-terminated
    validStatus = true;

    return parseStatus ();
}

Pen*
DeskJet3600::firstPen ()
{
    curPen = 0;
    return nextPen ();
}

Pen*
DeskJet3600::nextPen ()
{
    if (curPen < numPens)
        return pens[curPen++];

    return NULL;
}

int
DeskJet3600::updateStatus ()
{
    std::string deviceID;

    if (backend == NULL)
        return 0;

    if (backend->getDeviceID (deviceID))
    {
        strncpy (status, deviceID.c_str (), sizeof(status));
        validStatus = true;
    }
    else
        validStatus = false;

    return parseStatus ();
}

int
DeskJet3600::parseStatus ()
{
    const int  penDataOffset = 18,        // in hex digits
               penDataLength = 8;         // in hex digits
    const char penDataFormat[] = "%8x";   // reads two bytes

    if (!validStatus)
        return 0;

    char* statusChunk = strstr (status, ";S:");
    if (!statusChunk)
        return 0;

    // The pens are found at string offset 18, after the ";S:"
    statusChunk += 3;
    if (strlen (statusChunk) < penDataOffset)
        return 0;

    unsigned int revision;
    sscanf (statusChunk, "%2x", &revision);

    // DeskJet 3600 is revision 3, so anything else won't work here
    if (revision != 3)
        return 0;

    unsigned int penCount,
                 curRawPen;

    statusChunk += penDataOffset;

    // First nybble is the number of pens, which we obtain and then
    //  skip over
    sscanf (statusChunk++, "%1x", &penCount);

    // DeskJet 3600 has two cartridge slots. Any more and it's not that
    //  printer model.
    if (penCount > 2)
        return 0;

    Pen* pen;
    for (unsigned int i = 0 ; i < penCount ; i++)
    {
        sscanf (statusChunk, penDataFormat, &curRawPen);

        pen = new Pen (curRawPen);
        if (pen->exists ())
        {
            std::cerr << "DEBUG: Found existent Pen with type "
                      << pen->getType () << std::endl;
            // The pen may not exist. Don't add it unless it does.
            // A note:
            //  Whether or not there's a cartridge installed in the printer itself, the
            //  status string will always say there are two pens, each of the Cartridge
            //  type. The Pen itself knows if it exists (at least that's where I
            //  determine this.) So far, any marking (ink, toner, etc.) Pen that has
            //  no valid Color is likely not installed.
            pens[numPens++] = pen;
        }
        else
            delete pen;

        statusChunk += penDataLength;
    }

    return 1;
}

// The following LIDIL packets were lifted from HP's own ldl.py, by Don
//  Welch. Someday, I'd love to add some of this functionality to the
//  hp backend, but for now, I'm just implementing it here.

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

// vim: et sw=4
