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
			m_model->accept(this);
		}
		~MainPresenter()
		{
			m_model->remove(this);
		}


		virtual void data_was_updated()
		{
			;
		}

	private:
		MainView *m_view;
		Model *m_model;
	};
}
#endif
