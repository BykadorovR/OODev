#include "App.h"
#include "MainView.h"
#include "LoginView.h"
#include "QueueView.h"
#include "MainView.h"


const char *db_login = "login";
const char *db_pass = "password";

bool MyApp::OnInit()
{
	m_DB = new MY_DB();
	m_model = new Model(m_DB);

	m_pipeline.push(new LoginView(this, m_model));
	m_pipeline.push(new QueueView(this, m_model));
	m_pipeline.push(new MainView(this, m_model));
	m_pipeline.front()->Show();

	return true;
}

void MyApp::CloseView(wxCloseEvent& event)
{
	m_pipeline.front()->Destroy();
	m_pipeline.pop();

	// check permissions
	if(m_model->permission() == PERMISSION_NONE)
		this->Exit();

	if (!m_pipeline.empty())
		m_pipeline.front()->Show();
}