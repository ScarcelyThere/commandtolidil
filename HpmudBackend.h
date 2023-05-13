#ifndef _HPMUDBACKEND_H
#define _HPMUDBACKEND_H

#include <string>
#include "hpmud.h"
#include "Backend.h"

class HpmudBackend : public Backend
{
    public:
        HpmudBackend ();
        HpmudBackend (const char*);
        ~HpmudBackend ();

        bool getDeviceID (std::string&);

    private:
        hpmud_model_attributes modelAttrs;
        HPMUD_DEVICE device;
        std::string uri;
        bool validUri;
        bool deviceOpen;
};

#endif

// vim: et sw=4
