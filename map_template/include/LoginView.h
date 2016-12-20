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
		IAuth *m_auth;
	protected:
		wxTextCtrl* m_text_login;
		wxTextCtrl* m_text_password;
		wxButton* m_button_login;

	public:

		LoginView(wxEvtHandler *handler, IAuth *model, wxWindow* parent = NULL, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(1020, 562), long style = wxCAPTION | wxTAB_TRAVERSAL);

		~LoginView();

	};

	class LoginPresenter : public IPresenter, public wxEvtHandler
	{
	public:
		LoginPresenter(IAuth *auth, LoginView *view) : m_view(view), m_auth(auth)
		{
			;
		}
		~LoginPresenter()
		{
			;
		}


		///@brief register given view
		///@param view to be registered
		virtual void reg(IView *view)
		{
			;
		}
		///@brief register given model
		///@param model to be registered
		virtual void reg(IModel *model)
		{
			;
		}
		///@brief register given map
		///@param map to be registered
		virtual void reg(IMap *map)
		{
			;
		}
		///@brief register given Auth module
		///@param auth module to be registered
		virtual void reg(IAuth *auth)
		{
			;
		}

		virtual void button_login_click(wxCommandEvent& event)
		{
			std::string login = m_view->m_text_login->GetValue();
			std::string password = m_view->m_text_password->GetValue();
			if (m_auth->login(login, password))
				wxMessageBox(wxString(wxT("Welcome ")) + m_auth->get_login(), wxT("Welcome"), wxICON_INFORMATION);
			m_view->Close();
		}

	private:
		LoginView *m_view;
		IAuth *m_auth;
	};
}
#endif