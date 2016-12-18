#include "LoginView.h"
#include "App.h"

LoginView::LoginView(wxEvtHandler *handler, IAuth *auth, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	m_auth = auth;
	m_presenter = new LoginPresenter(auth, this);

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxVERTICAL);

	m_text_login = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(m_text_login, 0, wxALL, 5);

	m_text_password = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(m_text_password, 0, wxALL, 5);

	m_button_login = new wxButton(this, wxID_ANY, wxT("login"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(m_button_login, 0, wxALL, 5);

	m_button_login->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LoginPresenter::button_login_click), NULL, m_presenter);
	this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MyApp::CloseView), NULL, handler);

	this->SetSizer(bSizer3);
	this->Layout();
	this->Fit();
	this->Centre(wxBOTH);
}

LoginView::~LoginView()
{
	m_button_login->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LoginPresenter::button_login_click), NULL, m_presenter);
	delete m_presenter;
}