#ifndef MY_MAIN_VIEW
#define MY_MAIN_VIEW

#include <wx\wx.h>
#include <wx\frame.h>
#include <wx\stattext.h>

#include "Interfaces.h"
#include "Model.h"
#include "GLCanvas.h"
namespace newmeteo {
	class MainView : public wxFrame
	{
		friend class MainPresenter;
	private:
		MainPresenter *m_presenter;
		Model *m_model;
		MapGLPane *m_pane;
	protected:
		wxTextCtrl* m_textLevel;
		wxStaticText* m_staticText1;
		wxButton* m_buttonAdd;
	public:
		MainView(wxEvtHandler *handler, Model *model, wxWindow* parent = NULL, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(744, 528), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
		~MainView();
	};

	class MainPresenter : public IPresenter, public wxEvtHandler
	{
	public:
		MainPresenter(Model *model, MainView *view) : m_view(view), m_model(model)
		{
			;
		}
		~MainPresenter()
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

	private:
		MainView *m_view;
		Model *m_model;
	};
}
#endif
