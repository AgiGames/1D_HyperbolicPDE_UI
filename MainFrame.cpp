#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/dynarray.h>
#include <wx/textentry.h>

double y[100][100], x[100];
int stringlen, h, a, k, tRange, lowRange, lambda, outputActivated = 0;
double vibPeriod;

enum IDs {
	LRspinID = 2,
	URspinID = 3,
	IspinID = 4,
	AspinID = 5,
	LAMBDAspinID = 6,
	VIBPERtcID = 7
};

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	CreateControls();
	BindControls();
}

void MainFrame::CreateControls() {
	panel = new wxPanel(this);

	wxStaticText* outputText = new wxStaticText(panel, wxID_ANY, "OUTPUT", wxPoint(15, 250), wxSize(765, -1), wxALIGN_CENTRE);
	outputText->SetBackgroundColour(*wxRED);

	linspaceText = new wxStaticText(panel, wxID_ANY, "Lower-range(LR), upper-range(UR), and increment(I) value for x:", wxPoint(15, 20), wxSize(400, 100));
	linspaceText->SetBackgroundColour(*wxLIGHT_GREY);

	linspaceLRText = new wxStaticText(panel, wxID_ANY, "LR", wxPoint(40, 70));

	linspaceURText = new wxStaticText(panel, wxID_ANY, "UR", wxPoint(140, 70));

	linspaceIText = new wxStaticText(panel, wxID_ANY, "I", wxPoint(240, 70));

	linspaceLRsp = new wxSpinCtrl(panel, LRspinID, "", wxPoint(25, 90), wxSize(50, 20), 16384L, 0, 999, 0);

	linspaceURsp = new wxSpinCtrl(panel, URspinID, "", wxPoint(125, 90), wxSize(50, 20), 16384L, 0, 999, 0);

	linspaceIsp = new wxSpinCtrl(panel, IspinID, "", wxPoint(225, 90), wxSize(50, 20), 16384L, 0, 999, 0);

	lambdaText = new wxStaticText(panel, wxID_ANY, "Enter LAMBDA:", wxPoint(500, 20), wxSize(100, 100));
	lambdaText->SetBackgroundColour(*wxLIGHT_GREY);

	LAMBDAsp = new wxSpinCtrl(panel, LAMBDAspinID, "", wxPoint(515, 70), wxSize(50, 20), 16384L, 0, 999, 0);

	aText = new wxStaticText(panel, wxID_ANY, "Enter a:", wxPoint(685, 20), wxSize(100, 100));
	aText->SetBackgroundColour(*wxLIGHT_GREY);

	Asp = new wxSpinCtrl(panel, AspinID, "", wxPoint(700, 70), wxSize(50, 20), 16384L, 0, 999, 0);

	vibPerText = new wxStaticText(panel, wxID_ANY, "Enter the vibration period:", wxPoint(600, 140), wxSize(100, 100));
	vibPerText->SetBackgroundColour(*wxLIGHT_GREY);

	wxTextValidator validator(wxFILTER_NUMERIC);
	vibPerTC = new wxTextCtrl(panel, VIBPERtcID, "", wxPoint(615, 210), wxSize(50, 20), 0L, validator);

	submitButton = new wxButton(panel, wxID_ANY, "SUBMIT", wxPoint(60, 170), wxSize(300, 50));

	clearButton = new wxButton(panel, wxID_ANY, "CLEAR", wxPoint(680, 575), wxSize(80, 25));

	CreateStatusBar();
}


void MainFrame::BindControls() {
	linspaceLRsp->Bind(wxEVT_SPINCTRL, &MainFrame::OnSpin, this);

	linspaceURsp->Bind(wxEVT_SPINCTRL, &MainFrame::OnSpin, this);

	linspaceIsp->Bind(wxEVT_SPINCTRL, &MainFrame::OnSpin, this);

	Asp->Bind(wxEVT_SPINCTRL, &MainFrame::OnSpin, this);

	LAMBDAsp->Bind(wxEVT_SPINCTRL, &MainFrame::OnSpin, this);

	vibPerTC->Bind(wxEVT_TEXT, &MainFrame::OnType, this);

	submitButton->Bind(wxEVT_BUTTON, &MainFrame::OnSubmitClick, this);

	clearButton->Bind(wxEVT_BUTTON, &MainFrame::Clear, this);
}

void MainFrame::OnSubmitClick(wxCommandEvent& evt) {
	wxLogStatus("SUBMITTED");
	ActivateFormulas();
	FillXArray();
	PrintXValues();
	FillYArray();
	PrintYValues();
}

void MainFrame::OnSpin(wxSpinEvent& evt) {
	int id = evt.GetId();
	if (id == URspinID) {
		wxSpinCtrl* spinCtrl = wxDynamicCast(evt.GetEventObject(), wxSpinCtrl);
		if (spinCtrl) {
			stringlen = spinCtrl->GetValue();
			}
	}
	if (id == LRspinID) {
		wxSpinCtrl* spinCtrl = wxDynamicCast(evt.GetEventObject(), wxSpinCtrl);
		if (spinCtrl) {
			lowRange = spinCtrl->GetValue();
		}
	}
	if (id == IspinID) {
		wxSpinCtrl* spinCtrl = wxDynamicCast(evt.GetEventObject(), wxSpinCtrl);
		if (spinCtrl) {
			h = spinCtrl->GetValue();
		}
	}
	if (id == AspinID) {
		wxSpinCtrl* spinCtrl = wxDynamicCast(evt.GetEventObject(), wxSpinCtrl);
		if (spinCtrl) {
			a = spinCtrl->GetValue();
		}
	}
	if (id == LAMBDAspinID) {
		wxSpinCtrl* spinCtrl = wxDynamicCast(evt.GetEventObject(), wxSpinCtrl);
		if (spinCtrl) {
			lambda = spinCtrl->GetValue();
		}
	}
}

void MainFrame::OnType(wxCommandEvent& evt) {
	int id = evt.GetId();
	if (id == VIBPERtcID) {
		wxTextCtrl* textCtrl = wxDynamicCast(evt.GetEventObject(), wxTextCtrl);
		if (textCtrl) {
			double value;
			wxString text = textCtrl->GetValue();
			bool result = text.ToDouble(&value);
			if (result) {
				vibPeriod = value;
			}
		}
	}
}

void MainFrame::PrintXValues() {
	XVALUEStext = new wxStaticText(panel, wxID_ANY, "X-VALUES", wxPoint(20, 300));
	wxString str1;
	wxString str2;
	for (int i = 0; i <= (stringlen - lowRange) / h; i++) {
		str2 = wxString::Format("%.2lf  ", x[i]);
		str1.Append(str2);
	}
	XVALUES = new wxTextCtrl(panel, wxID_ANY, str1, wxPoint(20, 330), wxSize(300, 50), wxTE_READONLY | wxTE_MULTILINE);
}

void MainFrame::FillXArray() {
	x[0] = lowRange;
	for (int i = 1; i <= (stringlen - lowRange) / h; i++)
		x[i] = x[i - 1] + h;
}

void MainFrame::FillYArray() {
	for (int j = 0; j <= (stringlen - lowRange) / h; j++)
		y[0][j] = 0.01 * x[j] * (10 - x[j]);
	for (int i = tRange; i >= 1; i--) {
		y[i][lowRange] = 0;
		y[i][(stringlen - lowRange) / h] = 0;
	}
	for (int j = 1; j <= (stringlen - lowRange) / h - 1; j++)
		y[1][j] = 0.5 * (y[0][j - 1] + y[0][j + 1]);
	for (int i = 2; i <= tRange; i++)
		for (int j = 1; j <= (stringlen - lowRange) / h - 1; j++)
			y[i][j] = y[i - 1][j - 1] + y[i - 1][j + 1] +
			((2 - (2 * lambda)) * y[i][j - 1]) - y[i - 2][j];
}

void MainFrame::PrintYValues() {
	YVALUEStext = new wxStaticText(panel, wxID_ANY, "Y-VALUES", wxPoint(350, 300));
	wxString str1;
	wxString str2;
	int point1 = 350, point2 = 350;
	for (int i = tRange; i >= 0; i--) {
		for (int j = 0; j <= (stringlen - lowRange) / h; j++) {
			str2 = wxString::Format("%.2lf\t", y[i][j]);
			str1.Append(str2);
		}
		str1.Append("\n");
	}
	YVALUES = new wxTextCtrl(panel, wxID_ANY, str1, wxPoint(350, 330), wxSize(400, 230), wxTE_READONLY|wxTE_MULTILINE);
}

void MainFrame::ActivateFormulas() {
	outputActivated = 1;
	tRange = (2 * stringlen / a) * vibPeriod;
	k = lambda * (h * h) / (a * a);
}

void MainFrame::Clear(wxCommandEvent& evt) {
	if (outputActivated == 1) {
		XVALUEStext->Destroy();
		XVALUES->Destroy();
		YVALUEStext->Destroy();
		YVALUES->Destroy();
		outputActivated = 0;
	}

	linspaceLRsp->SetValue(0);
	linspaceURsp->SetValue(0);
	linspaceIsp->SetValue(0);
	LAMBDAsp->SetValue(0);
	Asp->SetValue(0);
	vibPerTC->SetValue("");

	linspaceLRsp->SetFocus();

	wxLogStatus("CLEARED");
}