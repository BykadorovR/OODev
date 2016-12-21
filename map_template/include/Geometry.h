#ifndef MY_GEOMETRY
#define MY_GEOMETRY

#include <eigen>
#include <vector>
#include <string>
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
		bezier_path(const std::vector<bezier_line*> &lines, int id = 0, int depth = 0, std::string comment = "") : m_lines(lines),
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
		virtual void drawGL()
		{
			;
		}
		const std::vector<bezier_line*> &get_lines() const
		{
			return m_lines;
		}

        const int get_id() const
        {
            return m_id;
        }

	private:
		int m_depth;
		std::vector<bezier_line*> m_lines;
		std::string m_comment;
		int m_id;
	};
}
#endif