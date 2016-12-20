#ifndef LOGIN_VIEW
#define LOGIN_VIEW

#include <wx\wx.h>
#include <wx\frame.h>

#include "Views.h"
#include "Model.h"
namespace newmeteo {
	class LoginView : public wxFrame
	{
		friend class LoginPresenter;
	private:
		LoginPresenter *m_presenter;
		Model *m_model;
	protected:
		wxTextCtrl* m_text_login;
		wxTextCtrl* m_text_password;
		wxButton* m_button_login;

	public:

		LoginView(wxEvtHandler *handler, Model *model, wxWindow* parent = NULL, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(1020, 562), long style = wxCAPTION | wxTAB_TRAVERSAL);

		~LoginView();

	};

	class LoginPresenter : public IPresenter, public wxEvtHandler
	{
	public:
		LoginPresenter(Model *model, LoginView *view) : m_view(view), m_model(model)
		{
			m_model->accept(this);
		}
		~LoginPresenter()
		{
			m_model->remove(this);
		}

		virtual void data_was_updated()
		{
			;
		}

		virtual void button_login_click(wxCommandEvent& event)
		{
			std::string login = m_view->m_text_login->GetValue();
			std::string password = m_view->m_text_password->GetValue();
			if (m_model->login(login, password))
				wxMessageBox(wxString(wxT("Welcome ")) + m_model->get_login(), wxT("Welcome"), wxICON_INFORMATION);
			m_view->Close();
		}

	private:
		LoginView *m_view;
		Model *m_model;
	};
}
#endif