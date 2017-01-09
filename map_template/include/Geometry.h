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

		Vector2f B3(const float t)
		{
			float nt = (1.0f - t);
			return nt*nt*nt*m_dots[0] + 3.0f*nt*nt*t*m_dots[1] +
				3.0f*nt*t*t*m_dots[2] + t*t*t*m_dots[3];
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
			evaluate(20);
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
		virtual void drawGL() const;

		virtual void draw3DGL() const
		{
			;
		}

		const std::vector<bezier_line*> &get_lines() const
		{
			return m_lines;
		}

		const std::vector<Vector2f> &get_polygon() const
		{
			return m_polygon;
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

		bool is_inside(const Vector2f &p) const;
		
		bool is_inside_rectangle(const Vector2f &p) const;

		bool is_intersects_rectangle(const bezier_path *path) const
		{
			return is_inside_rectangle(path->m_North) || is_inside_rectangle(path->m_East) ||
				is_inside_rectangle(path->m_South) || is_inside_rectangle(path->m_West);
		}

		bool is_intersects(const bezier_path *path) const
		{
			// instead of calculating intersection of edges
			return this->is_intersects_rectangle(path) || path->is_intersects_rectangle(this);
		}

	private:
		void evaluate(const int subdivisions)
		{
			m_polygon.resize(subdivisions*m_lines.size());
			m_North = m_South = m_East = m_West = m_lines[0]->B3(0.0f);

			for (int i = 0, size = m_lines.size(); i < size; ++i)
				for (int j = 0; j < subdivisions; ++j)
				{
					const Vector2f vertex = m_lines[i]->B3(float(j) / float(subdivisions));
					m_polygon[i*subdivisions + j] = vertex;
					
					if (vertex.y() < m_North.y())
						m_North = vertex;
					if (vertex.y() > m_South.y())
						m_South = vertex;
					if (vertex.x() < m_West.x())
						m_West = vertex;
					if (vertex.x() > m_East.x())
						m_East = vertex;
				}
		}


	private:

		float m_depth;
		std::vector<bezier_line*> m_lines;
		std::string m_comment;
		int m_id;

		Vector2f m_North, m_South, m_East, m_West;
		std::vector<Vector2f> m_polygon;
	};

	bool less(const bezier_path *lhs, const bezier_path *rhs);

	class Surface : public IDrawable
	{
	public:
		virtual void drawGL() const
		{
			;
		}

		virtual void draw3DGL() const;

		Surface(const std::vector<Vector2f> &polygon_x, std::vector<Vector3f> &normals_x, const float depth_x, const std::vector<Vector2f> &polygon_y, std::vector<Vector3f> &normals_y, const float depth_y);

	private:
		std::vector<Vector3f> m_triangles;
		std::vector<Vector3f*> m_normals;
	};
}
#endif