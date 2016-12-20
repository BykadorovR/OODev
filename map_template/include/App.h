#ifndef MY_WX_APP
#define MY_WX_APP

#include <queue>
#include <wx\wx.h>
#include "Model.h"


class MY_DB : public IDB
{
	virtual void connect()
	{
		;
	}
	virtual void request()
	{
		;
	}
};

class MyApp : public wxApp
{
public:

	virtual ~MyApp()
	{
		delete m_DB;
		delete m_model;
	}
	virtual bool OnInit();

	void MyApp::CloseView(wxCloseEvent& event);
private:
	IDB *m_DB;
	Model *m_model;
	std::queue<wxFrame*> m_pipeline;
};


#endif