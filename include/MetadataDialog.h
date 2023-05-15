#ifndef METADATADIALOG_H
#define METADATADIALOG_H

#include <wx/dialog.h>
#include <wx/grid.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <starling/starling.h>

#include "myna.h"

class MetadataDialog : public wxDialog
{
    public:
        MetadataDialog(wxWindow *parent,wxWindowID id = -1, Starling_db *dbpo = nullptr);
        virtual ~MetadataDialog();
    private:
        void OnQuit(wxCommandEvent& event);
        std::string RetrieveDateModified();
        std::string RetrieveDbDescription();
        Starling_db *dbp;

        static const long ID_STATICTEXT1;
        static const long ID_STATICTEXT2;
        static const long ID_STATICTEXT3;
        static const long ID_STATICTEXT4;
        static const long ID_STATICTEXT5;
        static const long ID_STATICTEXT6;
        static const long ID_STATICTEXT7;
        static const long ID_STATICTEXT8;
        static const long ID_STATICTEXT9;
        static const long ID_GRID1;

        wxGrid* FieldsGrid;
        wxGridStringTable *FieldsGridTable;
        wxStaticText* DateModifiedST;
        wxStaticText* DbDescST;
        wxStaticText* NumFieldsST;
        wxStaticText* NumRecST;
        wxStaticText* StaticText1;
        wxStaticText* StaticText2;
        wxStaticText* StaticText3;
        wxStaticText* StaticText4;
        wxStaticText* StaticText5;

        DECLARE_EVENT_TABLE()
};

#endif
