#include "App.h"
#include "MainView.h"
#include "LoginView.h"
#include "QueueView.h"
#include "MainView.h"

using namespace newmeteo;

const char *db_login = "login";
const char *db_pass = "password";

std::vector<bezier_line *> fill_path(int count, int x, int y) {
    std::vector<bezier_line *> lines;
    for (int i = 0; i < count; i++) {
        Vector2f* dots;
        dots = new Vector2f[4];
        for (int j = 0; j < 4; j++) {
            dots[j] = Vector2f(x + j + i, y + j + i);
        }
        lines.push_back(new bezier_line(dots));
    }
    return lines;
}
bool MyApp::OnInit()
{
    IDB* db = new MY_DB();
    db->connect();
 

    
    m_DB = new MY_DB();
	m_model = NULL;
    if (!m_DB->connect())
        return false;
	m_model = new Model(m_DB);

	m_pipeline.push(new LoginView(this, m_model));
	m_pipeline.push(new QueueView(this, m_model));
	m_pipeline.push(new MainView(this, m_model));
	m_pipeline.front()->Show();

	return true;
}

void MyApp::CloseView(wxCloseEvent& event)
{
	m_pipeline.front()->Hide();
	m_pipeline.front()->Destroy();
	m_pipeline.pop();

	// check permissions
	if(m_model->permission() == PERMISSION_NONE)
		this->Exit();

	if (!m_pipeline.empty())
		m_pipeline.front()->Show();
}