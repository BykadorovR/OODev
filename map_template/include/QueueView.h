#ifndef QUEUE_VIEW
#define QUEUE_VIEW

#include <wx\wx.h>
#include <wx\frame.h>
#include <wx\stattext.h>

#include "Views.h"
#include "Model.h"
#include "GLCanvas.h"
namespace newmeteo {
	class QueueView : public wxFrame
	{
		friend class QueuePresenter;
	private:
		QueuePresenter *m_presenter;
		Model *m_model;
		MapGLPane *m_pane;
	protected:
		wxButton* m_button_apply;
		wxButton* m_button_reject;
		wxStaticText* m_message;

	public:
		QueueView(wxEvtHandler *handler, Model *model, wxWindow* parent = NULL, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(790, 453), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
		~QueueView();
	};

	class QueuePresenter : public IPresenter, public wxEvtHandler
	{
	public:
		QueuePresenter(Model *model, QueueView *view) : m_view(view), m_model(model)
		{
			m_model->accept(this);
		}
		~QueuePresenter()
		{
			m_model->remove(this);
		}


		virtual void data_was_updated()
		{
			;
		}

	private:
		QueueView *m_view;
		Model *m_model;
	};
}
#endif