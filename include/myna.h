#ifndef MYNAAPP_H
#define MYNAAPP_H

#include <wx/app.h>

class mynaApp : public wxApp
{
    public:
        virtual bool OnInit();
};

void InitXmlResource();

#endif // MYNAAPP_H
