#ifndef MY_GL_CANVAS
#define MY_GL_CANVAS

#include <eigen>

#include <wx/wx.h>
#include <wx/glcanvas.h>

namespace newmeteo {
	class MapGLPane : public wxGLCanvas
	{
		friend class MapGLPanePresenter;
	private:
		wxGLContext* m_context;
	public:
		MapGLPane(wxFrame* parent, int* args);
		virtual ~MapGLPane();
		int getWidth();
		int getHeight();
	};

	class MapGLPanePresenter : public wxEvtHandler
	{
	public:
		MapGLPanePresenter(MapGLPane *pane) : m_pane(pane)
		{
			m_scale = Vector2f(1.0f, 1.0f);
			//angles for 3d
			m_angles = Vector3f(0.0f, 0.0f, 0.0f);
			m_projective_position = Vector2f(0.0f, 0.0f);
			m_perspective_position = Vector3f(0.0f, 0.0f, -100.0f);


			m_pane->Connect(wxEVT_PAINT, wxPaintEventHandler(MapGLPanePresenter::render), NULL, this);
			m_pane->Connect(wxEVT_SIZE, wxSizeEventHandler(MapGLPanePresenter::resized), NULL, this);
			m_pane->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(MapGLPanePresenter::mouseDown), NULL, this);
			m_pane->Connect(wxEVT_MOTION, wxMouseEventHandler(MapGLPanePresenter::mouseMoved), NULL, this);
			m_pane->Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(MapGLPanePresenter::mouseWheelMoved), NULL, this);
		}
		virtual ~MapGLPanePresenter()
		{
			m_pane->Disconnect(wxEVT_PAINT, wxPaintEventHandler(MapGLPanePresenter::render), NULL, this);
			m_pane->Disconnect(wxEVT_SIZE, wxSizeEventHandler(MapGLPanePresenter::resized), NULL, this);
			m_pane->Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(MapGLPanePresenter::mouseDown), NULL, this);
			m_pane->Disconnect(wxEVT_MOTION, wxMouseEventHandler(MapGLPanePresenter::mouseMoved), NULL, this);
			m_pane->Disconnect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(MapGLPanePresenter::mouseWheelMoved), NULL, this);
		}

		void mouseReleased(wxMouseEvent& event);
		void rightClick(wxMouseEvent& event);
		void mouseLeftWindow(wxMouseEvent& event);
		void keyReleased(wxKeyEvent& event);
		void keyPressed(wxKeyEvent& event);

		void mouseWheelMoved(wxMouseEvent& event);
		void mouseMoved(wxMouseEvent& event);
		void mouseDown(wxMouseEvent& event);
		void render(wxPaintEvent& evt);
		void resized(wxSizeEvent& evt);

		void prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
		void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);

		virtual void custom_render_2d();

		virtual void custom_render_3d();

	protected:
		//scale for 2 windows
		Vector2f m_scale;
		//angles for 3d
		Vector3f m_angles;
		Vector2f m_projective_position;
		Vector3f m_perspective_position;
		
		// false - 2d, true 3d
		bool m_active_3dpane;
		wxPoint m_prev_position;

		MapGLPane *m_pane;
	};
}
#endif