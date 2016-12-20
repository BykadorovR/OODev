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

	Reconnaissance rec;
	Vector2f dots[] = { Vector2f(0.0f, 0.0f), Vector2f(1.5f, 2.5f), Vector2f(2.5f, 0.5f), Vector2f(3.0f, 4.0f) };
	bezier_line *line = new bezier_line(dots);
	std::vector<bezier_line*> lines; lines.push_back(line);
	bezier_path path(lines);
	std::vector<unsigned int> out;
	rec.find_region(&path, out);

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