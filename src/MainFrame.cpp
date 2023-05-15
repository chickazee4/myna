#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>

#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/msgdlg.h>
#include <wx/string.h>

#include <starling/starling.h>
#include "MainFrame.h"
#include "MetadataDialog.h"

const long MainFrame::ID_GRID1 = wxNewId();
const long MainFrame::ImportDbfMBtnId = wxNewId();
const long MainFrame::ExportCsvMBtnId = wxNewId();
const long MainFrame::QuitMBtnId = wxNewId();
const long MainFrame::DeleteRowBtnId = wxNewId();
const long MainFrame::DeleteColBtnId = wxNewId();
const long MainFrame::SortAscendingMBtnId = wxNewId();
const long MainFrame::SortDescendingMBtnId = wxNewId();
const long MainFrame::ShowDeletedMBtnId = wxNewId();
const long MainFrame::ShowIndicesMBtnId = wxNewId();
const long MainFrame::ShowDbMetadataMBtnId = wxNewId();
const long MainFrame::HumanNamesMBtnId = wxNewId();

bool IsContentChanged = false;

BEGIN_EVENT_TABLE(MainFrame,wxFrame)
	//event table
END_EVENT_TABLE()

MainFrame::MainFrame(wxWindow *parent,wxWindowID id)
{
	Create(parent, id, _("myna"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxDEFAULT_FRAME_STYLE|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX|wxFRAME_SHAPED|wxMAXIMIZE_BOX|wxMINIMIZE_BOX, _T("id"));
    #if defined(unix) || defined(__unix__) || defined(__unix) || defined(__linux__)
    // unix macro is not defined on macOS, which is actually the desired behavior
    wxIcon FrameIcon;
    if(std::filesystem::exists(std::filesystem::path(APPLICATION_DATA) / std::filesystem::path("myna/icon.png"))){
        auto bmp = new wxBitmap((std::filesystem::path(APPLICATION_DATA) / std::filesystem::path("myna/icon.png")).string(), wxBITMAP_TYPE_PNG);
        FrameIcon.CopyFromBitmap(*bmp);
        SetIcon(FrameIcon);
    }
    #endif
	DbfGridView = new wxGrid(this, ID_GRID1, wxPoint(168,176), wxDefaultSize, wxVSCROLL|wxHSCROLL, _T("ID_GRID1"));
	DbfGridViewTable = new wxGridStringTable();
	DbfGridView->SetTable(DbfGridViewTable);
	ImportDialog = new wxFileDialog(this, _("Select database"), wxEmptyString, wxEmptyString, _("*.dbf"), wxFD_DEFAULT_STYLE|wxFD_OPEN, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
	ExportDialog = new wxFileDialog(this, _("Export CSV"), wxEmptyString, wxEmptyString, _("*.csv"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
	MainMenuBar = new wxMenuBar();
	Menu1 = new wxMenu();
	ImportDbfMBtn = new wxMenuItem(Menu1, ImportDbfMBtnId, _("Import .dbf"), wxEmptyString, wxITEM_NORMAL);
	Menu1->Append(ImportDbfMBtn);
	ExportCsvMBtn = new wxMenuItem(Menu1, ExportCsvMBtnId, _("Export .csv"), wxEmptyString, wxITEM_NORMAL);
	Menu1->Append(ExportCsvMBtn);
	Menu1->AppendSeparator();
	QuitMBtn = new wxMenuItem(Menu1, QuitMBtnId, _("Quit"), wxEmptyString, wxITEM_NORMAL);
	Menu1->Append(QuitMBtn);
	MainMenuBar->Append(Menu1, _("File"));
	Menu2 = new wxMenu();
	DeleteRowBtn = new wxMenuItem(Menu2, DeleteRowBtnId, _("Delete row"), wxEmptyString, wxITEM_NORMAL);
	Menu2->Append(DeleteRowBtn);
	DeleteColBtn = new wxMenuItem(Menu2, DeleteColBtnId, _("Delete column"), wxEmptyString, wxITEM_NORMAL);
	Menu2->Append(DeleteColBtn);
	SortAscendingMBtn = new wxMenuItem(Menu2, SortAscendingMBtnId, _("Sort by this column (ascending)"), wxEmptyString, wxITEM_NORMAL);
	Menu2->Append(SortAscendingMBtn);
	SortDescendingMBtn = new wxMenuItem(Menu2, SortDescendingMBtnId, _("Sort by this column (descending)"), wxEmptyString, wxITEM_NORMAL);
	Menu2->Append(SortDescendingMBtn);
	MainMenuBar->Append(Menu2, _("Edit"));
	Menu3 = new wxMenu();
	ShowDeletedMBtn = new wxMenuItem(Menu3, ShowDeletedMBtnId, _("Show deleted entries"), wxEmptyString, wxITEM_CHECK);
	Menu3->Append(ShowDeletedMBtn);
	ShowDeletedMBtn->Check(true);
	ShowIndicesMBtn = new wxMenuItem(Menu3, ShowIndicesMBtnId, _("Show indices column"), wxEmptyString, wxITEM_CHECK);
	Menu3->Append(ShowIndicesMBtn);
	ShowIndicesMBtn->Check(true);
	HumanNamesMBtn = new wxMenuItem(Menu3, HumanNamesMBtnId, _("Use human-readable column names (if available)"), wxEmptyString, wxITEM_CHECK);
	Menu3->Append(HumanNamesMBtn);
	HumanNamesMBtn->Check(true);
    ShowDbMetadataMBtn = new wxMenuItem(Menu3, ShowDbMetadataMBtnId, _("Database metadata..."), wxEmptyString, wxITEM_NORMAL);
	Menu3->Append(ShowDbMetadataMBtn);
	MainMenuBar->Append(Menu3, _("View"));
	SetMenuBar(MainMenuBar);

	Connect(id, wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&MainFrame::OnCloseWindow);
	Connect(ID_GRID1,wxEVT_GRID_CELL_CHANGED,(wxObjectEventFunction)&MainFrame::OnDbfGridViewChanged);
	Connect(ImportDbfMBtnId,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MainFrame::OnImportDbfMBtnSelected);
    Connect(ExportCsvMBtnId,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MainFrame::OnExportCsvMBtnSelected);
    Connect(QuitMBtnId,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MainFrame::OnQuitMBtnSelected);
	Connect(DeleteRowBtnId,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MainFrame::OnDeleteRowMBtnSelected);
	Connect(DeleteColBtnId,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MainFrame::OnDeleteColMBtnSelected);
    Connect(ShowDeletedMBtnId,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MainFrame::OnShowDeletedMBtnSelected);
    Connect(ShowIndicesMBtnId,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MainFrame::OnShowIndicesMBtnSelected);
    Connect(HumanNamesMBtnId,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MainFrame::OnHumanNamesMBtnSelected);
    Connect(ShowDbMetadataMBtnId,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MainFrame::OnShowDbMetadataMBtnSelected);

    // accelerators
    wxAcceleratorEntry accels[4];
    #ifdef __APPLE__
    #define ACCELCTRL wxACCEL_CMD
    #else
    #define ACCELCTRL wxACCEL_CTRL
    #endif
    accels[0].Set(ACCELCTRL, (int)'O', ImportDbfMBtnId);
    accels[1].Set(ACCELCTRL, (int)'S', ExportCsvMBtnId);
    accels[2].Set(ACCELCTRL, (int)'Q', QuitMBtnId);
    accels[3].Set(ACCELCTRL, (int)'H', ShowDbMetadataMBtnId);
    wxAcceleratorTable AccelTable(4, accels);
    this->SetAcceleratorTable(AccelTable);

	MainMenuBar->Show(true);
}

MainFrame::~MainFrame()
{
    //destroy
}

void
MainFrame::OnCloseWindow(wxCloseEvent &event)
{
    if(IsContentChanged && wxMessageBox(_("Contents have been changed! Quit anyway?"), _("Warning"), wxICON_QUESTION | wxYES_NO, this) == wxNO){
            event.Veto();
            return;
    }
    Destroy();
}

void
MainFrame::OnDbfGridViewChanged(wxCommandEvent &event)
{
    if(!IsContentChanged){
        this->SetTitle(this->GetTitle() + "*");
        IsContentChanged = true;
    }
}

void
MainFrame::OnImportDbfMBtnSelected(wxCommandEvent &event)
{
    if(ImportDialog->ShowModal() == wxID_CANCEL)
        return;
    else {
        // Load selected file
        std::string DbfPath;
        if(std::filesystem::exists((std::string)(ImportDialog->GetPath()))){
            DbfPath = (std::string)(ImportDialog->GetPath());
        } else {
            // error
            return;
        }
        std::string VarPath = ((std::filesystem::path)DbfPath).replace_extension(".var");
        if(!std::filesystem::exists(VarPath)){
            if(wxMessageBox(_("No .VAR file could not be found automatically. This is not necessarily a problem, but it may cause a crash if the database requires a .var file. Continue anyway?"), _("Warning"), wxICON_QUESTION | wxYES_NO, this) == wxNO)
                return;
            else VarPath = "";
        }
        int result;
        dbp = starling_parse_file(&result, &(DbfPath)[0], ((VarPath == "") ? nullptr : &(VarPath)[0]));
        std::filesystem::path InfPath = ((std::filesystem::path)DbfPath).replace_extension(".inf");
        if(std::filesystem::exists(InfPath))
            result &= starling_parse_inf(dbp, InfPath.c_str());
        if(result != (STARLING_OK & STARLING_OK))
            return; // parse error
        else {
            this->SetTitle("myna - " + DbfPath);
            starling_decode_all(dbp);
            PopulateTableView();
        }
    }
}

void
MainFrame::OnExportCsvMBtnSelected(wxCommandEvent &event)
{
    std::string CsvDelim = ",";
    if(ExportDialog->ShowModal() == wxID_CANCEL)
        return;
    else {
        // Need to implement a separate csv creation function from the one in libstarling
        // to preserve user modifications to the table, since they are only reflected in
        // the wxGrid and not the original database structure
        std::ofstream csv;
        csv.open((std::string)(ExportDialog->GetPath()), std::ofstream::out | std::ofstream::trunc);
        if(DbfGridView->GetRowLabelSize() > 0){
            csv << (HumanNamesMBtn->IsChecked() ? dbp->hdrs[0].human_name : dbp->hdrs[0].name);
            csv << CsvDelim;
        }
        for(int i = 0; i < DbfGridView->GetNumberCols(); i++){
            csv << DbfGridView->GetColLabelValue(i);
            if(i < DbfGridView->GetNumberCols() - 1)
                csv << CsvDelim;
        }
        csv << "\n";
        for(int i = 0; i < DbfGridView->GetNumberRows(); i++){
            if(DbfGridView->GetRowLabelSize() > 0){
                csv << DbfGridView->GetRowLabelValue(i);
                csv << CsvDelim;
            }
            for(int j = 0; j < DbfGridView->GetNumberCols(); j++){
                csv << DbfGridView->GetCellValue(i, j);
                if(j < DbfGridView->GetNumberCols() - 1)
                    csv << CsvDelim;
            }
            csv << "\n";
        }
        csv.close();
        IsContentChanged = false;
        if(this->GetTitle().Last() == '*')
           this->SetTitle(this->GetTitle().Before('*'));
    }
}

void
MainFrame::OnQuitMBtnSelected(wxCommandEvent &event)
{
    if(IsContentChanged && wxMessageBox(_("Contents have been changed! Quit anyway?"), _("Warning"), wxICON_QUESTION | wxYES_NO, this) == wxNO)
            return;
    Close();
}

void
MainFrame::OnDeleteRowMBtnSelected(wxCommandEvent &event)
{
    for(int i: DbfGridView->GetSelectedRows())
        DbfGridView->DeleteRows(i);
    IsContentChanged = true;
}


void
MainFrame::OnDeleteColMBtnSelected(wxCommandEvent &event)
{
    for(int i: DbfGridView->GetSelectedCols())
        DbfGridView->DeleteCols(i);
}

void
MainFrame::OnShowDeletedMBtnSelected(wxCommandEvent &event)
{
    PopulateTableView();
}

void
MainFrame::OnShowIndicesMBtnSelected(wxCommandEvent &event){
    if(ShowIndicesMBtn->IsChecked()) DbfGridView->SetRowLabelSize(wxGRID_AUTOSIZE);
    else DbfGridView->HideRowLabels();
}

void
MainFrame::OnHumanNamesMBtnSelected(wxCommandEvent &event)
{
    if(dbp){
        for(int hi = 1; hi < dbp->hdr_ct; hi++){
            DbfGridViewTable->SetColLabelValue(hi - 1, wxString::FromUTF8(((HumanNamesMBtn->IsChecked() && dbp->hdrs[hi].human_name) ? dbp->hdrs[hi].human_name : dbp->hdrs[hi].name)));
        }
    }
}

void
MainFrame::OnShowDbMetadataMBtnSelected(wxCommandEvent &event)
{
    MetadataDialog *md = new MetadataDialog(this, 0, dbp);
    md->Show();
}

void
MainFrame::PopulateTableView()
{
    DbfGridViewTable->Clear();
    DbfGridView->ClearGrid();
    if(dbp){
        // set up columns
        for(int hi = 1; hi < dbp->hdr_ct; hi++){
            if(DbfGridView->GetNumberCols() < hi)
                DbfGridViewTable->InsertCols(hi - 1, 1);
            DbfGridViewTable->SetColLabelValue(hi - 1, wxString::FromUTF8(((HumanNamesMBtn->IsChecked() && dbp->hdrs[hi].human_name) ? dbp->hdrs[hi].human_name : dbp->hdrs[hi].name)));
        }
        // set up rows
        int Ri = 0;
        Starling_sanitize_flags sf = { 1, 1, 1 };
        for(int ri = 0; ri < dbp->rec_ct; ri++){
            if(ShowDeletedMBtn->IsChecked() || !(dbp->recs[ri].is_deleted)){
                DbfGridViewTable->InsertRows(Ri, 1);
                DbfGridViewTable->SetRowLabelValue(Ri, starling_sanitize(dbp->recs[ri].entries[0].decoded_content, dbp->recs[ri].entries[0].true_length, &sf)); // usually the index - is it ever not? go check later
                for(int ei = 1; ei < dbp->hdr_ct; ei++){
                    DbfGridViewTable->SetValue(Ri, ei - 1, wxString::FromUTF8(starling_sanitize(dbp->recs[ri].entries[ei].decoded_content, dbp->recs[ri].entries[ei].true_length, &sf)));
                }
                Ri++;
            }
        }
    }
}
