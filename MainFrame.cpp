#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/dynarray.h>
#include <wx/textentry.h>

double y[100][100], x[100];
int stringlen, lowRange, h;
double a, k, tRange,lambda, outputActivated = 0, vibPeriod;

enum IDs {
	LRtcID = 2,
	URtcID = 3,
	ItcID = 4,
	AtcID = 5,
	LAMBDAtcID = 6,
	VIBPERtcID = 7
};

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	CreateControls();
	BindControls();
}

void MainFrame::CreateControls() {
	panel = new wxPanel(this);

	wxTextValidator validator(wxFILTER_NUMERIC);

	wxFont font(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Arial Unicode MS");

	wxStaticText* outputText = new wxStaticText(panel, wxID_ANY, "OUTPUT", wxPoint(15, 250), wxSize(765, -1), wxALIGN_CENTRE);
	outputText->SetBackgroundColour(*wxRED);

	linspaceText = new wxStaticText(panel, wxID_ANY, "Lower-range(LR), upper-range(UR), and increment(I) value for x:", wxPoint(15, 20), wxSize(400, 100));
	linspaceText->SetBackgroundColour(*wxLIGHT_GREY);

	linspaceLRtc = new wxTextCtrl(panel, LRtcID, "LR", wxPoint(25, 70), wxSize(50, 40), 0L, validator);
	linspaceLRtc->SetFont(font);

	linspaceURtc = new wxTextCtrl(panel, URtcID, "UR", wxPoint(125, 70), wxSize(50, 40), 0L, validator);
	linspaceURtc->SetFont(font);

	linspaceItc = new wxTextCtrl(panel, ItcID, "I", wxPoint(225, 70), wxSize(50, 40), 0L, validator);
	linspaceItc->SetFont(font);

	lambdaText = new wxStaticText(panel, wxID_ANY, "Enter LAMBDA:", wxPoint(500, 20), wxSize(100, 100));
	lambdaText->SetBackgroundColour(*wxLIGHT_GREY);

	LAMBDAtc = new wxTextCtrl(panel, LAMBDAtcID, "Lambda", wxPoint(510, 75), wxSize(75, 30), 0L, validator);

	aText = new wxStaticText(panel, wxID_ANY, "Enter a:", wxPoint(685, 20), wxSize(100, 100));
	aText->SetBackgroundColour(*wxLIGHT_GREY);

	Atc = new wxTextCtrl(panel, AtcID, "a", wxPoint(700, 70), wxSize(50, 40), 0L, validator);
	Atc->SetFont(font);

	vibPerText = new wxStaticText(panel, wxID_ANY, "Enter the vibration period:", wxPoint(600, 140), wxSize(100, 100));
	vibPerText->SetBackgroundColour(*wxLIGHT_GREY);

	vibPerTC = new wxTextCtrl(panel, VIBPERtcID, "Period", wxPoint(610, 200), wxSize(75, 30), 0L, validator);

	submitButton = new wxButton(panel, wxID_ANY, "SUBMIT", wxPoint(60, 170), wxSize(300, 50));

	clearButton = new wxButton(panel, wxID_ANY, "CLEAR", wxPoint(680, 575), wxSize(80, 25));

	CreateStatusBar();
}


void MainFrame::BindControls() {
	linspaceLRtc->Bind(wxEVT_TEXT, &MainFrame::OnType, this);

	linspaceURtc->Bind(wxEVT_TEXT, &MainFrame::OnType, this);

	linspaceItc->Bind(wxEVT_TEXT, &MainFrame::OnType, this);

	Atc->Bind(wxEVT_TEXT, &MainFrame::OnType, this);

	LAMBDAtc->Bind(wxEVT_TEXT, &MainFrame::OnType, this);

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
	if (id == LRtcID) {
		wxTextCtrl* textCtrl = wxDynamicCast(evt.GetEventObject(), wxTextCtrl);
		if (textCtrl) {
			int value;
			wxString text = textCtrl->GetValue();
			bool result = text.ToInt(&value);
			if (result) {
				lowRange = value;
			}
		}
	}
	if (id == URtcID) {
		wxTextCtrl* textCtrl = wxDynamicCast(evt.GetEventObject(), wxTextCtrl);
		if (textCtrl) {
			int value;
			wxString text = textCtrl->GetValue();
			bool result = text.ToInt(&value);
			if (result) {
				stringlen = value;
			}
		}
	}
	if (id == ItcID) {
		wxTextCtrl* textCtrl = wxDynamicCast(evt.GetEventObject(), wxTextCtrl);
		if (textCtrl) {
			int value;
			wxString text = textCtrl->GetValue();
			bool result = text.ToInt(&value);
			if (result) {
				h = value;
			}
		}
	}
	if (id == AtcID) {
		wxTextCtrl* textCtrl = wxDynamicCast(evt.GetEventObject(), wxTextCtrl);
		if (textCtrl) {
			double value;
			wxString text = textCtrl->GetValue();
			bool result = text.ToDouble(&value);
			if (result) {
				a = value;
			}
		}
	}
	if (id == LAMBDAtcID) {
		wxTextCtrl* textCtrl = wxDynamicCast(evt.GetEventObject(), wxTextCtrl);
		if (textCtrl) {
			double value;
			wxString text = textCtrl->GetValue();
			bool result = text.ToDouble(&value);
			if (result) {
				lambda = value;
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

	linspaceLRtc->SetValue("LR");
	linspaceURtc->SetValue("UR");
	linspaceItc->SetValue("I");
	LAMBDAtc->SetValue("Lambda");
	Atc->SetValue("a");
	vibPerTC->SetValue("Period");

	linspaceLRtc->SetFocus();

	wxLogStatus("CLEARED");
}