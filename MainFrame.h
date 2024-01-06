#pragma once
#include <wx/wx.h>
#include <wx/textentry.h>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);
private:
	wxPanel* panel;
	wxStaticText* linspaceText;
	wxStaticText* linspaceLRText;
	wxStaticText* linspaceURText;
	wxStaticText* linspaceIText;
	wxTextCtrl* linspaceLRtc;
	wxTextCtrl* linspaceURtc;
	wxTextCtrl* linspaceItc;
	wxStaticText* aText;
	wxStaticText* lambdaText;
	wxTextCtrl* Atc;
	wxTextCtrl* LAMBDAtc;
	wxStaticText* vibPerText;
	wxTextCtrl* vibPerTC;
	wxButton* submitButton;
	wxStaticText* XVALUEStext;
	wxTextCtrl* XVALUES;
	wxStaticText* YVALUEStext;
	wxTextCtrl* YVALUES;
	wxButton* clearButton;

	void CreateControls();
	void Clear(wxCommandEvent& evt);

	void BindControls();
	void OnSubmitClick(wxCommandEvent& evt);
	void OnSpin(wxSpinEvent& evt);
	void OnType(wxCommandEvent& evt);
	void PrintXValues();
	void FillXArray();
	void FillYArray();
	void PrintYValues();
	void ActivateFormulas();
};

