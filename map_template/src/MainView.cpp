#include "MainView.h"
#include "App.h"

MainView::MainView(wxEvtHandler *handler, Model *model, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	m_presenter = new MainPresenter(model, this);
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer(wxHORIZONTAL);

	m_textLevel = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer14->Add(m_textLevel, 0, wxALL, 5);

	m_staticText1 = new wxStaticText(this, wxID_ANY, wxT("Sea Level"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText1->Wrap(-1);
	bSizer14->Add(m_staticText1, 0, wxALL, 5);


	bSizer12->Add(bSizer14, 0, wxFIXED_MINSIZE, 5);

	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer(wxVERTICAL);

	m_buttonAdd = new wxButton(this, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer17->Add(m_buttonAdd, 0, wxALL, 5);


	bSizer12->Add(bSizer17, 0, wxFIXED_MINSIZE, 5);


	bSizer11->Add(bSizer12, 0, wxFIXED_MINSIZE, 5);

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer(wxVERTICAL);

	int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };
	m_pane = new MapGLPane(this, args);


	bSizer13->Add(m_pane, 1, wxEXPAND, 5);

	bSizer11->Add(bSizer13, 1, wxEXPAND, 5);


	this->SetSizer(bSizer11);
	this->Layout();

	this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MyApp::CloseView), NULL, handler);

	this->Centre(wxBOTH);
}

MainView::~MainView()
{
	delete m_presenter;
}

