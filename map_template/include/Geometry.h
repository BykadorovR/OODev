#ifndef MY_GEOMETRY
#define MY_GEOMETRY

#include <eigen>
#include <vector>
#include <string>

#include <GL/glu.h>
#include <GL/gl.h>

#include "NonGeometricInterfaces.h"
namespace newmeteo {
	///@brief bezier line 
	class bezier_line
	{
	public:
		bezier_line(Vector2f * const dots)
		{
			m_dots = dots;
		}
		~bezier_line()
		{
			delete[] m_dots;
		}
		const Vector2f* get() const
		{
			return m_dots;
		}
	private:
		Vector2f *m_dots;
	};

	///@brief bezier path
	class bezier_path : public IDrawable
	{
	public:
		bezier_path(const std::vector<bezier_line*> &lines, int id = 0, float depth = 0, std::string comment = "") : m_lines(lines),
            m_id(id),
            m_depth(depth),
            m_comment(comment)
		{
			;
		}
		~bezier_path()
		{
			for (unsigned int i = 0; i < m_lines.size(); ++i)
				delete m_lines[i];
		}
		///@brief perform basic test
		///@return true if path is valid
		bool test()
		{
			return true;
		}
		///@brief draw function. contains GL functionality
		virtual void drawGL() const
		{
			GLfloat ctrlpoints[12];
			for (std::vector<bezier_line*>::const_iterator it = m_lines.begin(), end = m_lines.end();
				it != end; ++it)
			{
				//configure ctrl points

				for (int i = 0; i < 4; ++i)
				{
					ctrlpoints[3*i] = (*it)->get()[i].x();
					ctrlpoints[3*i + 1] = (*it)->get()[i].y();
					ctrlpoints[3*i + 2] = m_depth;
				}

				glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, ctrlpoints);

				glBegin(GL_LINE_STRIP);
				for (int i = 0; i <= 30; i++)
					glEvalCoord1f((GLfloat)i / 30.0f);
				glEnd();
			}
		}
		const std::vector<bezier_line*> &get_lines() const
		{
			return m_lines;
		}

        const int get_id() const
        {
            return m_id;
        }

		const float get_level() const
		{
			return m_depth;
		}

		const float get_depth() const
		{
			return m_depth;
		}

		const std::string &get_comment() const
		{
			return m_comment;
		}

	private:
		float m_depth;
		std::vector<bezier_line*> m_lines;
		std::string m_comment;
		int m_id;
	};
}
#endif