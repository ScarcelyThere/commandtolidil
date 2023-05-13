#ifndef _BACKEND_H
#define _BACKEND_H

#include <string>

class Backend
{
    public:
        Backend () = default;
        virtual ~Backend () = default;

        // This is all we really need from outside.
        virtual bool getDeviceID (std::string&) = 0;
};

#endif

// vim: et sw=4
