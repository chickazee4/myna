#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/filedlg.h>
#include <wx/frame.h>
#include <wx/grid.h>
#include <wx/menu.h>

#include <starling/starling.h>

class MainFrame: public wxFrame
{
	public:
		MainFrame(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~MainFrame();
		Starling_db *dbp;

		wxFileDialog * ExportDialog;
		wxFileDialog * ImportDialog;
		wxGrid * DbfGridView;
		wxGridStringTable * DbfGridViewTable;
		wxMenu * Menu1;
		wxMenu * Menu2;
		wxMenu * Menu3;
		wxMenuBar * MainMenuBar;
		wxMenuItem * DeleteColBtn;
		wxMenuItem * DeleteRowBtn;
		wxMenuItem * ExportCsvMBtn;
		wxMenuItem * ImportDbfMBtn;
		wxMenuItem * QuitMBtn;
		wxMenuItem * ShowDbMetadataMBtn;
		wxMenuItem * ShowDeletedMBtn;
		wxMenuItem * ShowIndicesMBtn;
		wxMenuItem * HumanNamesMBtn;
		wxMenuItem * SortAscendingMBtn;
		wxMenuItem * SortDescendingMBtn;
	protected:
		static const long ID_GRID1;
		static const long ImportDbfMBtnId;
		static const long ExportCsvMBtnId;
		static const long QuitMBtnId;
		static const long DeleteRowBtnId;
		static const long DeleteColBtnId;
		static const long SortAscendingMBtnId;
		static const long SortDescendingMBtnId;
		static const long ShowDeletedMBtnId;
		static const long ShowIndicesMBtnId;
		static const long ShowDbMetadataMBtnId;
		static const long HumanNamesMBtnId;
	private:
		void OnImportDbfMBtnSelected(wxCommandEvent &event);
		void OnExportCsvMBtnSelected(wxCommandEvent &event);
		void OnQuitMBtnSelected(wxCommandEvent &event);
		void OnDeleteRowMBtnSelected(wxCommandEvent &event);
		void OnDeleteColMBtnSelected(wxCommandEvent &event);
		void OnShowDeletedMBtnSelected(wxCommandEvent &event);
	    void OnShowIndicesMBtnSelected(wxCommandEvent &event);
	    void OnHumanNamesMBtnSelected(wxCommandEvent &event);
	    void OnShowDbMetadataMBtnSelected(wxCommandEvent &event);
	    void PopulateTableView();

		DECLARE_EVENT_TABLE()
};

#endif
