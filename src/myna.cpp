#include <wx/xrc/xmlres.h>
#include <wx/image.h>
#include <wx/stattext.h>
#include <wx/translation.h>
#include <libintl.h>

#include "myna.h"
#include "MainFrame.h"

IMPLEMENT_APP(mynaApp);

bool mynaApp::OnInit()
{
    InitXmlResource();
    wxInitAllImageHandlers();
    MainFrame *mf = new MainFrame(0);
    mf->Show();
    SetTopWindow(mf);
    return true;
}
