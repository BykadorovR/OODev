#include "GLCanvas.h"

#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/glcanvas.h"

#include <GL/glu.h>
#include <GL/gl.h>

using namespace newmeteo;

//BEGIN_EVENT_TABLE(BasicGLPane, wxGLCanvas)
//EVT_MOTION(BasicGLPane::mouseMoved)
//EVT_LEFT_DOWN(BasicGLPane::mouseDown)
//EVT_LEFT_UP(BasicGLPane::mouseReleased)
//EVT_RIGHT_DOWN(BasicGLPane::rightClick)
//EVT_LEAVE_WINDOW(BasicGLPane::mouseLeftWindow)
//EVT_SIZE(BasicGLPane::resized)
//EVT_KEY_DOWN(BasicGLPane::keyPressed)
//EVT_KEY_UP(BasicGLPane::keyReleased)
//EVT_MOUSEWHEEL(BasicGLPane::mouseWheelMoved)
//EVT_PAINT(BasicGLPane::render)
//END_EVENT_TABLE()


void MapGLPanePresenter::mouseMoved(wxMouseEvent& event)
{
	const float speed_persp= 0.1f;
	const float speed_proj = 1.0f;
	if (!event.LeftIsDown())
		return;
	wxPoint pos = event.GetPosition();
	if (m_active_3dpane)
	{
		m_angles.x() += speed_persp * (pos.y - m_prev_position.y);
		m_angles.y() += speed_persp * (pos.x - m_prev_position.x);
	}
	else
	{
		m_projective_position.x() += speed_proj * (pos.x - m_prev_position.x);
		m_projective_position.y() += speed_proj * (pos.y - m_prev_position.y);
	}

	m_prev_position = pos;
	m_pane->Refresh();
}
void MapGLPanePresenter::mouseDown(wxMouseEvent& event)
{
	m_pane->SetFocus();
	m_prev_position = event.GetPosition();

	m_active_3dpane = m_prev_position.x > m_pane->getWidth() / 2;

}
void MapGLPanePresenter::mouseWheelMoved(wxMouseEvent& event)
{
	const float scale_step_proj = 0.005f;
	const float scale_step_persp = 1.0f;
	wxPoint pos = event.GetPosition();
	if (pos.x > m_pane->getWidth() / 2)
	{
		m_scale[1] += scale_step_persp * event.GetWheelRotation() / abs(event.GetWheelRotation());
	}
	else
	{
		m_scale[0] += scale_step_proj * event.GetWheelRotation() / abs(event.GetWheelRotation());
	}
	m_pane->Refresh();
}

void MapGLPanePresenter::mouseReleased(wxMouseEvent& event) { ; }
void MapGLPanePresenter::rightClick(wxMouseEvent& event) { ; }
void MapGLPanePresenter::mouseLeftWindow(wxMouseEvent& event) { ; }
void MapGLPanePresenter::keyPressed(wxKeyEvent& event) { ; }
void MapGLPanePresenter::keyReleased(wxKeyEvent& event) { ; }

// Vertices and faces of a simple cube to demonstrate 3D render
// source: http://www.opengl.org/resources/code/samples/glut_examples/examples/cube.c
GLfloat v[8][3];
GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
	{ 0, 1, 2, 3 },{ 3, 2, 6, 7 },{ 7, 6, 5, 4 },
	{ 4, 5, 1, 0 },{ 5, 6, 2, 1 },{ 7, 4, 0, 3 } };



MapGLPane::MapGLPane(wxFrame* parent, int* args) :
	wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
	m_context = new wxGLContext(this);

	// prepare a simple cube to demonstrate 3D render
	// source: http://www.opengl.org/resources/code/samples/glut_examples/examples/cube.c
	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;

	// To avoid flashing on MSW
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

MapGLPane::~MapGLPane()
{
	delete m_context;
}

void MapGLPanePresenter::resized(wxSizeEvent& evt)
{
	//	wxGLCanvas::OnSize(evt);

	m_pane->Refresh();
}

/** Inits the OpenGL viewport for drawing in 3D. */
void MapGLPanePresenter::prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
	//glClearDepth(1.0f);	// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST); // Enables Depth Testing
	glEnable(GL_MAP1_VERTEX_3);
	glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_COLOR_MATERIAL);

	glViewport(topleft_x, topleft_y, bottomrigth_x - topleft_x, bottomrigth_y - topleft_y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float ratio_w_h = (float)(bottomrigth_x - topleft_x) / (float)(bottomrigth_y - topleft_y);
	gluPerspective(45 /*view angle*/, ratio_w_h, 0.1 /*clip close*/, 200 /*clip far*/);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

/** Inits the OpenGL viewport for drawing in 2D. */
void MapGLPanePresenter::prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
	glEnable(GL_TEXTURE_2D);   // textures
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_MAP1_VERTEX_3);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(topleft_x, topleft_y, bottomrigth_x - topleft_x, bottomrigth_y - topleft_y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(topleft_x, bottomrigth_x,
		bottomrigth_y, topleft_y, 10000.f, -10000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int MapGLPane::getWidth()
{
	return GetSize().x;
}

int MapGLPane::getHeight()
{
	return GetSize().y;
}


void MapGLPanePresenter::render(wxPaintEvent& evt)
{
	if (!m_pane->IsShown()) return;

	m_pane->SetCurrent(*m_pane->m_context);
	wxPaintDC(this->m_pane); // only to be used in paint events. use wxClientDC to paint outside the paint event

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ------------- draw some 2D ----------------
	prepare2DViewport(0, 0, m_pane->getWidth() / 2, m_pane->getHeight());
	glLoadIdentity();

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex2f(m_pane->getWidth() / 2, 0);
	glVertex2f(m_pane->getWidth() / 2, m_pane->getHeight());
	glEnd();

	glTranslatef(m_projective_position.x(), m_projective_position.y(), 0.0f);
	glScalef(1.0f / m_scale[0], 1.0f / m_scale[0], 1.0f / m_scale[0]);


	custom_render_2d();

	// ------------- draw some 3D ----------------
	prepare3DViewport(m_pane->getWidth() / 2, 0, m_pane->getWidth(), m_pane->getHeight());
	glLoadIdentity();

	GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 0.5 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 1000.0, 1000.0, 1000.0, 1.0 };



	glTranslatef(m_perspective_position.x(), m_perspective_position.y(), m_perspective_position.z());
	glRotatef(m_angles.x(), 1.0f, 0.0f, 0.0f);
	glRotatef(m_angles.y(), 0.0f, 1.0f, 0.0f);
	glRotatef(m_angles.z(), 0.0f, 0.0f, 1.0f);
	glScalef(1.0f / m_scale[1], 1.0f / m_scale[1], 1.0f / m_scale[1]);

	glColor4f(0, 0, 1, 1);
	glDisable(GL_LIGHTING);
	glPointSize(15.0f);
	glBegin(GL_POINTS);
	glVertex4f(light_position[0], light_position[1], light_position[2], light_position[3]);
	glEnd();
	glEnable(GL_LIGHTING);

	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	//glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	custom_render_3d();

	glFlush();
	m_pane->SwapBuffers();
}

void MapGLPanePresenter::custom_render_2d()
{
	;
}
void MapGLPanePresenter::custom_render_3d()
{
	;
}