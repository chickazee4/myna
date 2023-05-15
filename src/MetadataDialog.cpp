#include <cstdio>
#include <cstdlib>

#include <wx/msgdlg.h>
#include <wx/intl.h>
#include <wx/string.h>

#include "MetadataDialog.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

const long MetadataDialog::ID_STATICTEXT1 = wxNewId();
const long MetadataDialog::ID_STATICTEXT2 = wxNewId();
const long MetadataDialog::ID_STATICTEXT3 = wxNewId();
const long MetadataDialog::ID_STATICTEXT4 = wxNewId();
const long MetadataDialog::ID_STATICTEXT5 = wxNewId();
const long MetadataDialog::ID_STATICTEXT6 = wxNewId();
const long MetadataDialog::ID_STATICTEXT7 = wxNewId();
const long MetadataDialog::ID_STATICTEXT8 = wxNewId();
const long MetadataDialog::ID_STATICTEXT9 = wxNewId();
const long MetadataDialog::ID_GRID1 = wxNewId();

BEGIN_EVENT_TABLE(MetadataDialog,wxDialog)
    // EVENT TABLE
END_EVENT_TABLE()

MetadataDialog::MetadataDialog(wxWindow *parent, wxWindowID id, Starling_db *dbpo)
{
    dbp = dbpo;
    // set up basic elements
    wxFlexGridSizer *GridSizer1;
    Create(parent, wxID_ANY, _("Database metadata"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX, _T("wxID_ANY"));
    GridSizer1 = new wxFlexGridSizer(5, 2, 0, 0);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Date last modified:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    GridSizer1->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    DateModifiedST = new wxStaticText(this, ID_STATICTEXT2, _(RetrieveDateModified()), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    GridSizer1->Add(DateModifiedST, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT3, _("Description:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    GridSizer1->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    DbDescST = new wxStaticText(this, ID_STATICTEXT4, _(RetrieveDbDescription()), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    GridSizer1->Add(DbDescST, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxST_NO_AUTORESIZE, 5);
    StaticText3 = new wxStaticText(this, ID_STATICTEXT5, _("Number of records:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    GridSizer1->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    NumRecST = new wxStaticText(this, ID_STATICTEXT6, _((dbp ? std::to_string(dbp->rec_ct) : "0")), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    GridSizer1->Add(NumRecST, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText4 = new wxStaticText(this, ID_STATICTEXT7, _("Number of fields"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    GridSizer1->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    NumFieldsST = new wxStaticText(this, ID_STATICTEXT8, _((dbp ? std::to_string(dbp->hdr_ct) : "0")), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
    GridSizer1->Add(NumFieldsST, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText5 = new wxStaticText(this, ID_STATICTEXT9, _("Fields:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
    GridSizer1->Add(StaticText5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FieldsGrid = new wxGrid(this, ID_GRID1, wxDefaultPosition, wxDefaultSize, 0, _T("ID_GRID1"));
    FieldsGrid->CreateGrid(0,3);
    FieldsGrid->EnableEditing(false);
    FieldsGrid->EnableGridLines(true);
    FieldsGrid->SetColLabelValue(0, _("Internal name"));
    FieldsGrid->SetColLabelValue(1, _("Human name"));
    FieldsGrid->SetColLabelValue(2, _("Data type"));
    FieldsGrid->HideRowLabels();
    FieldsGrid->SetDefaultCellFont( FieldsGrid->GetFont() );
    FieldsGrid->SetDefaultCellTextColour( FieldsGrid->GetForegroundColour() );
    FieldsGridTable = new wxGridStringTable();
    FieldsGrid->SetTable(FieldsGridTable);
    FieldsGridTable->InsertCols(0, 3);
    GridSizer1->Add(FieldsGrid, 1, wxALL|wxEXPAND, 5);
    SetSizer(GridSizer1);
    GridSizer1->Fit(this);
    GridSizer1->SetSizeHints(this);
    DbDescST->Wrap(GridSizer1->GetColWidths()[1]);
    Center();

    if(dbp){
        for(int i = 0; i < dbp->hdr_ct; i++){
            FieldsGridTable->InsertRows(i);
            FieldsGridTable->SetValue(i, 0, dbp->hdrs[i].name);
            FieldsGridTable->SetValue(i, 1, (dbp->hdrs[i].human_name ? dbp->hdrs[i].human_name : "n/a"));
            FieldsGridTable->SetValue(i, 2, starling_fieldtypetostr(dbp->hdrs[i].type));
        }
    }
}

std::string
MetadataDialog::RetrieveDateModified()
{
    if(!dbp)
        return "??";
    else {
        char *out = (char *)malloc(11);
        sprintf(out, "%u/%u/%d", dbp->month, dbp->day, (dbp->year > 22 ? dbp->year + 1900 : dbp->year + 2000));
        out[11] = 0x00;
        return out;
    }
}

std::string
MetadataDialog::RetrieveDbDescription()
{
    if(!dbp || !(dbp->db_description))
        return "??";
    else return dbp->db_description;
}

MetadataDialog::~MetadataDialog()
{
//destroy
}

void MetadataDialog::OnQuit(wxCommandEvent& event)
{
    Close();
}
