#ifndef QUEUE_VIEW
#define QUEUE_VIEW

#include <wx\wx.h>
#include <wx\frame.h>

#include "Views.h"
#include "Presenters.h"
#include "Model.h"
#include "GLCanvas.h"

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

public:
	QueueView(wxEvtHandler *handler, Model *model, wxWindow* parent = NULL, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(790, 453), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
	~QueueView();
};

class QueuePresenter : public IPresenter, public wxEvtHandler
{
public:
	QueuePresenter(IAuth *auth, QueueView *view) : m_view(view), m_auth(auth)
	{
		;
	}
	~QueuePresenter()
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
	QueueView *m_view;
	IAuth *m_auth;
};
#endif