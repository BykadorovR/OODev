#include "MainView.h"
#include "App.h"

#include <wx/valnum.h>

using namespace newmeteo;

MainView::MainView(wxEvtHandler *handler, Model *model, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	m_main_app = handler;
	m_level = 0.0f;
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer(wxHORIZONTAL);

	wxFloatingPointValidator<float>
		val(2, &m_level, wxNUM_VAL_ZERO_AS_BLANK);
	m_textLevel = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, val);
	bSizer14->Add(m_textLevel, 0, wxALL, 5);

	m_staticText1 = new wxStaticText(this, wxID_ANY, wxT("Sea Level"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText1->Wrap(-1);
	bSizer14->Add(m_staticText1, 0, wxALL, 5);


	bSizer12->Add(bSizer14, 0, wxFIXED_MINSIZE, 5);

	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer(wxVERTICAL);

	m_buttonAdd = new wxButton(this, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer17->Add(m_buttonAdd, 0, wxALL, 5);


	bSizer12->Add(bSizer17, 0, wxFIXED_MINSIZE, 5);


	bSizer11->Add(bSizer12, 0, wxFIXED_MINSIZE, 5);

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer(wxVERTICAL);

	int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };
	m_pane = new MapGLPane(this, args);


	bSizer13->Add(m_pane, 1, wxEXPAND, 5);

	bSizer11->Add(bSizer13, 1, wxEXPAND, 5);


	this->SetSizer(bSizer11);
	this->Layout();

	this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MyApp::CloseView), NULL, handler);

	this->Centre(wxBOTH);

	//establish links
	m_presenter = new MainPresenter(model, this);
	m_pane->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(MainPresenter::add_point), NULL, m_presenter);
	m_buttonAdd->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainPresenter::add_point), NULL, m_presenter);
}

MainView::~MainView()
{
	delete m_presenter;
}

void MainPresenter::custom_render_2d()
{
	const IMap::container &c =  m_model->get_paths();


	glColor3f(0.0f, 1.0f, 0.0f);


	for (IMap::iterator it = c.begin(), end = c.end(); it != end; ++it)
	{
		(*it)->drawGL();
	}

	glPointSize(5.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
	for (std::list<Vector2f>::iterator it = m_tmp_points.begin(); it != m_tmp_points.end(); ++it)
		glVertex3f((*it).x(), (*it).y(), 0.0f);
	glEnd();
}

void MainPresenter::custom_render_3d()
{
	const IMap::container &c = m_model->get_paths();


	glColor3f(0.0f, 1.0f, 0.0f);


	for (IMap::iterator it = c.begin(), end = c.end(); it != end; ++it)
	{
		(*it)->drawGL();
	}

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1000.0f, 0.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);
	glEnd();
}

void MainPresenter::add_point(wxCommandEvent& event)
{
	m_view->m_textLevel->GetValidator()->TransferFromWindow();
	if (m_tmp_points.size() % 3 || m_tmp_points.size() < 6)
		return;

	int lines_num = m_tmp_points.size() / 3;
	std::vector<bezier_line*> lines(lines_num);
	std::list<Vector2f>::iterator it = m_tmp_points.begin();
	for (int i = 0; i < lines_num; ++i)
	{
		Vector2f *dots = new Vector2f[4];
		dots[0] = *it;
		dots[1] = *(++it);
		dots[2] = *(++it);
		if ((++it) == m_tmp_points.end())
			dots[3] = *m_tmp_points.begin();
		else
			dots[3] = *it;
		bezier_line *line = new bezier_line(dots);
		lines[i] = line;
	}
	m_model->add_path(new bezier_path(lines, 0, m_view->m_level));
	m_tmp_points.clear();
	m_pane->Refresh();
}

void MainPresenter::add_point(wxMouseEvent& event)
{
	wxPoint pos = event.GetPosition();

	if (pos.x > m_pane->getWidth() / 2)
		return;

	m_tmp_points.push_back(
		Vector2f((pos.x - m_projective_position.x()) * m_scale[0],
			(pos.y - m_projective_position.y()) * m_scale[0] ));
	
	m_pane->Refresh();
}