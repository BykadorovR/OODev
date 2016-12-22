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
		wxEvtHandler *m_main_app;
		float m_level;
	protected:
		wxTextCtrl* m_textLevel;
		wxStaticText* m_staticText1;
		wxButton* m_buttonAdd;
	public:
		MainView(wxEvtHandler *handler, Model *model, wxWindow* parent = NULL, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(744, 528), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
		virtual ~MainView();
	};

	class MainPresenter : public IPresenter, public MapGLPanePresenter
	{
	public:
		MainPresenter(Model *model, MainView *view) : MapGLPanePresenter(view->m_pane)
		{
			m_view = view;
			m_model = model;
			m_model->accept(this);
		}
		virtual ~MainPresenter()
		{
			m_model->remove(this);
		}


		virtual void data_was_updated()
		{
			m_pane->Refresh();
		}

		void add_point(wxMouseEvent& event);

		void add_point(wxCommandEvent& event);

		virtual void custom_render_2d();

		virtual void custom_render_3d();

	private:
		std::list<Vector2f> m_tmp_points;
		MainView *m_view;
		Model *m_model;
	};
}
#endif
