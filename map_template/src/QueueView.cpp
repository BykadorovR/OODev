#include "QueueView.h"
#include "App.h"

using namespace newmeteo;

QueueView::QueueView(wxEvtHandler *handler, Model *model, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	m_main_app = handler;
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxVERTICAL);

	m_button_apply = new wxButton(this, wxID_ANY, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer4->Add(m_button_apply, 0, wxALL, 5);

	m_button_reject = new wxButton(this, wxID_ANY, wxT("Reject"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer4->Add(m_button_reject, 0, wxALL, 5);

	bSizer3->Add(bSizer4, 0, wxFIXED_MINSIZE, 5);

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer(wxVERTICAL);

	int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };
	m_pane = new MapGLPane(this, args);

	bSizer5->Add(m_pane, 1, wxEXPAND, 5);

	bSizer3->Add(bSizer5, 1, wxEXPAND, 5);

	this->SetSizer(bSizer3);
	this->Layout();

	this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MyApp::CloseView), NULL, m_main_app);

	this->Centre(wxBOTH);

	//establish links
	m_presenter = new QueuePresenter(model, this);
}

QueueView::~QueueView()
{
	delete m_presenter;
	this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MyApp::CloseView), NULL, m_main_app);
}