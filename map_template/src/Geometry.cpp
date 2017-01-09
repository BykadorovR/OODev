#include <vector>
#include <Eigen>
#include <deque>
#include <algorithm>
#include <iterator>

#include <windows.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "Geometry.h"

namespace newmeteo {

	inline int index(const int i, const int modulo)
	{
		return (modulo + i) % modulo;
	}

	///i in local coordinates
	inline int index(const int i, const int region_start, const int region_distance, const int global_distance)
	{
		return index(region_start + index(i, region_distance), global_distance);
	}

	inline int distance(const int i, const int j, const int modulo, const bool ccw)
	{
		return ccw ? (j - i + modulo) % modulo : (i - j + modulo) % modulo;
	}

	inline bool test_ccw(const int i, const int j, const int j_next, const int modulo)
	{
		int sum1 = 0, sum2 = 0;

		sum1 += distance(i, j, modulo, true);
		sum1 += distance(j, j_next, modulo, true);
		sum1 += distance(j_next, i, modulo, true);

		sum2 += distance(i, j, modulo, false);
		sum2 += distance(j, j_next, modulo, false);
		sum2 += distance(j_next, i, modulo, false);

		if (i == j || j == j_next)
			return distance(i, j_next, modulo, true) < distance(i, j_next, modulo, false);
		else if(i == j_next)
			return distance(i, j, modulo, true) < distance(i, j, modulo, false);

		assert(sum1 == modulo || sum2 == modulo || (i == j && j == j_next));
		return (sum1 == modulo);
	}

	inline float cross_norm(const Vector2f &a, const Vector2f &b)
	{
		float x = (a.y()*0.0f - 1.0f*b.y());
		float y = (1.0f*b.x() - a.x()*0.0f);
		float z = (a.x()*b.y() - a.y()*b.x());
		return x*x + y*y + z*z;
	}

	inline float signed_left(const Vector2f &a, const Vector2f &b, const Vector2f &c)
	{
		Vector2f dir1 = b - a, dir2 = c - b;

		return dir1.x()*dir2.y() - dir1.y()*dir2.x();
	}

	inline bool left(const Vector2f &a, const Vector2f &b, const Vector2f &c)
	{
		return signed_left(a, b, c) > 0.0f;
	}


	//start, end cw order
	void build_hull(const std::vector<Vector2f> &polygon, const int start, const int distance, std::vector<int> &hull)
	{
		std::deque<int> deq;
		int size = polygon.size();

		if (distance < 4)
		{
			for (int i = 0; i < distance; ++i)
				hull.push_back(index(start + i, size));

			return;
		}

		//melkman
		int idx = start;
		int idx_next = index(start + 1, size);
		int idx_next_next = index(start + 2, size);

		if (left(polygon[idx], polygon[idx_next], polygon[idx_next_next]))
		{
			deq.push_back(idx_next_next);
			deq.push_back(idx);
			deq.push_back(idx_next);
			deq.push_back(idx_next_next);
		}
		else
		{
			deq.push_back(idx_next_next);
			deq.push_back(idx_next);
			deq.push_back(idx);
			deq.push_back(idx_next_next);
		}

		for (int i = 3; i < distance; ++i)
		{
			idx = index(start + i, size);
			if (left(polygon[deq[deq.size() - 2]], polygon[deq[deq.size() - 1]], polygon[idx]) &&
				left(polygon[idx], polygon[deq[0]], polygon[deq[1]]))
				continue;
			while (!left(polygon[deq[deq.size() - 2]], polygon[deq[deq.size() - 1]], polygon[idx]))
				deq.pop_back();

			deq.push_back(idx);

			while (!left(polygon[idx], polygon[deq[0]], polygon[deq[1]]))
				deq.pop_front();

			deq.push_front(idx);
		}

		deq.pop_front();
		std::copy(deq.begin(), deq.end(), std::back_inserter(hull));
	}

	//find average of given hull over polygon
	Vector2f find_average(const std::vector<Vector2f> &polygon, const std::vector<int> &hull)
	{
		Vector2f average = Vector2f(0.0f, 0.0f);
		for (int i = 0, x_size = hull.size(); i < x_size; ++i)
			average += polygon[hull[i]];

		return average / hull.size();
	}

	int find_max_projection(const Vector2f &direction, const std::vector<Vector2f> &polygon_y, const std::vector<int> &hull_y, const Vector2f &hull_y_avg)
	{
		float max_proj = direction.dot(polygon_y[hull_y[0]] - hull_y_avg);
		int max_idx = hull_y[0];

		for (int j = 0, y_size = hull_y.size(); j < y_size; ++j)
		{
			float proj = direction.dot((polygon_y[hull_y[j]] - hull_y_avg).normalized());
			if (proj > max_proj)
			{
				max_proj = proj;
				max_idx = hull_y[j];
			}
		}

		return max_idx;
	}

	void find_projections(const std::vector<Vector2f> &polygon_x, const std::vector<Vector2f> &polygon_y, std::vector<int> &polygon_x_map, int start_x, int distance_x, int start_y, int distance_y)
	{
		int size_x = polygon_x.size();
		int size_y = polygon_y.size();

		//build convex hull
		std::vector<int> hull_x;
		std::vector<int> hull_y;

		build_hull(polygon_x, start_x, distance_x, hull_x);
		build_hull(polygon_y, start_y, distance_y, hull_y);

		//find projections
		Vector2f x_average = find_average(polygon_x, hull_x),
			y_average = find_average(polygon_y, hull_y);


		for (int i = 0, x_size = hull_x.size(); i < x_size; ++i)
		{
			Vector2f dir = polygon_x[hull_x[i]] - x_average;
			polygon_x_map[hull_x[i]] = find_max_projection(dir, polygon_y, hull_y, y_average);
		}

		// find regions
		for (int i = 0; i < distance_x; ++i)
		{
			int idx = index(start_x + i, size_x);
			if (polygon_x_map[idx] == -1)
			{
				int region_start_x = idx;
				int region_distance_x = 1;
				++i;
				while (i < distance_x && polygon_x_map[index(start_x + i, size_x)] == -1)
				{
					++region_distance_x;
					++i;
				}

				int region_start_y_idx = index(idx - 1, size_x);
				//only first region could start with -1
				int region_start_y = polygon_x_map[region_start_y_idx] == -1? start_y : polygon_x_map[region_start_y_idx];
				bool ccw = test_ccw(region_start_y, polygon_x_map[index(idx + region_distance_x, size_x)], polygon_x_map[index(idx + region_distance_x + 1, size_x)], size_y);
				int region_distance_y = distance(region_start_y, polygon_x_map[index(idx + region_distance_x, size_x)], size_y, ccw) + 1;

				//make ccw
				if (!ccw)
					region_start_y = index(region_start_y - region_distance_y + 1, size_y);


				//call for subregion
				find_projections(polygon_x, polygon_y, polygon_x_map, region_start_x, region_distance_x, region_start_y, region_distance_y);
			}
		}
	}

	void expand_map(const std::vector<Vector2f> &polygon_x, const std::vector<Vector2f> &polygon_y, std::vector<int> &polygon_x_map)
	{
		int size_x = polygon_x.size();
		int size_y = polygon_y.size();

		std::vector<int> old_map(polygon_x_map);
		//could be parallel
		for (int i = 0, old_map_size = old_map.size(); i < old_map_size; ++i)
		{
			int region_start_y = polygon_x_map[i];
			bool ccw = test_ccw(region_start_y, polygon_x_map[index(i + 1, size_x)], polygon_x_map[index(i + 2, size_x)], size_y);
			int region_distance_y = distance(region_start_y, polygon_x_map[index(i + 1, size_x)], size_y, ccw) + 1;

			if (!ccw)
				region_start_y = index(region_start_y - region_distance_y + 1, size_y);


			float sum = 0.0f;
			int min = region_start_y;
			float sum_min = FLT_MAX;

			const Vector2f &x1 = polygon_x[i];
			const Vector2f &x2 = polygon_x[index(i + 1, size_x)];

			for (int j = 0; j < region_distance_y; ++j)
			{
				sum = 0.0f;
				//calculate formula
				for (int k = 0; k < j; ++k)
				{
					int local_idx = index(region_start_y + k, size_y);
					int local_idx_next = index(region_start_y + k + 1, size_y);
					sum += cross_norm(x1 - polygon_y[local_idx], polygon_y[local_idx_next] - polygon_y[local_idx]);
				}

				for (int k = j; k < region_distance_y; ++k)
				{
					int local_idx = index(region_start_y + k, size_y);
					int local_idx_next = index(region_start_y + k + 1, size_y);
					sum += cross_norm(x2 - polygon_y[local_idx], polygon_y[local_idx_next] - polygon_y[local_idx]);
				}

				int local_idx = index(region_start_y + j, size_y);
				sum += cross_norm(x2 - x1, polygon_y[local_idx] - x1);

				if (sum_min > sum)
				{
					min = local_idx;
					sum_min = sum;
				}
			}

			polygon_x_map[i] = min;
		}
	}

	inline Vector3f get_normal(const Vector2f &x, const Vector2f &y1, const Vector2f &y2, const float x_depth, const float y_depth)
	{
		const Vector2f dir1 = (x - y1);
		const Vector2f dir2 = (y2 - y1);
		const Vector3f edge1 = Vector3f(dir1.x(), dir1.y(), x_depth - y_depth);
		const Vector3f edge2 = Vector3f(dir2.x(), dir2.y(), 0.0f);
		return edge1.cross(edge2).normalized();
	}

	inline Vector3f get_normal(const Vector3f &x, const Vector3f &y, const Vector3f &z)
	{
		const Vector3f dir1 = (y - x);
		const Vector3f dir2 = (z - x);
		return dir1.cross(dir2).normalized();
	}

	Surface::Surface(const std::vector<Vector2f> &polygon_x, std::vector<Vector3f> &normals_x, const float depth_x, const std::vector<Vector2f> &polygon_y, std::vector<Vector3f> &normals_y, const float depth_y)
	{

		// map triangle to polygon
		int size_x = polygon_x.size();
		int size_y = polygon_y.size();

		std::vector<int> polygon_x_map;
		//all elements should be -1
		polygon_x_map.resize(polygon_x.size(), -1);


		find_projections(polygon_x, polygon_y, polygon_x_map, 0, polygon_x.size(), 0, polygon_y.size());
		//expand_map(polygon_x, polygon_y, polygon_x_map);


		for (int i = 0, polygon_x_map_size = polygon_x_map.size(); i < polygon_x_map_size; ++i)
		{
			int region_start_y = polygon_x_map[index(i-1, size_x)];
			bool ccw = test_ccw(region_start_y, polygon_x_map[i], polygon_x_map[index(i + 1, size_x)], size_y);
			int region_distance_y = distance(region_start_y, polygon_x_map[i], size_y, ccw) + 1;

			int step = ccw ? 1 : -1;

			const Vector2f &x = polygon_x[i];

			for (int j = 0; j < region_distance_y - 1; ++j)
			{
				const int y_idx = index(region_start_y + step*j, size_y);
				const int y_idx_next = index(region_start_y + step*(j + 1), size_y);
				const Vector2f &y = polygon_y[y_idx];
				const Vector2f &y_next = polygon_y[y_idx_next];

				Vector3f triangle_normal = get_normal(x, y, y_next, depth_x, depth_y);
				normals_x[i] += triangle_normal;
				normals_y[y_idx] += triangle_normal;
				normals_y[y_idx_next] += triangle_normal;


				m_triangles.push_back(Vector3f(x.x(), x.y(), depth_x));
				m_normals.push_back(&(normals_x[i]));
				m_triangles.push_back(Vector3f(y.x(), y.y(), depth_y));
				m_normals.push_back(&(normals_y[y_idx]));
				m_triangles.push_back(Vector3f(y_next.x(), y_next.y(), depth_y));
				m_normals.push_back(&(normals_y[y_idx_next]));
			}


			const Vector2f &y_last = polygon_y[polygon_x_map[i]];
			const Vector2f &x_next = polygon_x[index(i + 1, size_x)];

			Vector3f triangle_normal = -get_normal(y_last, x, x_next, depth_y, depth_x);
			normals_x[i] += triangle_normal;
			normals_y[polygon_x_map[i]] += triangle_normal;
			normals_x[index(i + 1, size_x)] += triangle_normal;

			m_triangles.push_back(Vector3f(x.x(), x.y(), depth_x));
			m_normals.push_back(&(normals_x[i]));
			m_triangles.push_back(Vector3f(polygon_y[polygon_x_map[i]].x(), polygon_y[polygon_x_map[i]].y(), depth_y));
			m_normals.push_back(&(normals_y[polygon_x_map[i]]));
			m_triangles.push_back(Vector3f(polygon_x[index(i+1,size_x)].x(), polygon_x[index(i + 1, size_x)].y(), depth_x));
			m_normals.push_back(&(normals_x[index(i + 1, size_x)]));
		}

	}

	void bezier_path::drawGL() const
	{
		glBegin(GL_LINE_STRIP);

		for (int i = 0, size = m_polygon.size(); i < size; ++i)
			glVertex3f(m_polygon[i].x(), m_polygon[i].y(), m_depth);

		glVertex3f(m_polygon[0].x(), m_polygon[0].y(), m_depth);
		glEnd();
	}

	void Surface::draw3DGL() const
	{
		glBegin(GL_TRIANGLES);
		int triangles_size = m_triangles.size();

		for (int i = 0, size = triangles_size / 3; i < size; ++i)
		{
			m_normals[3 * i]->normalize();
			m_normals[3 * i + 1]->normalize();
			m_normals[3 * i + 2]->normalize();

			Vector3f tr_normal = get_normal(m_triangles[3 * i], m_triangles[3 * i + 1], m_triangles[3 * i + 2]);

			//normals from triangles
			//glNormal3f(tr_normal.x(), tr_normal.y(), tr_normal.z());
			glNormal3f(m_normals[3 * i]->x(), m_normals[3 * i]->y(), m_normals[3 * i]->z());
			glVertex3f(m_triangles[3 * i].x(), m_triangles[3 * i].y(), m_triangles[3 * i].z());
			//glNormal3f(tr_normal.x(), tr_normal.y(), tr_normal.z());;
			glNormal3f(m_normals[3 * i + 1]->x(), m_normals[3 * i + 1]->y(), m_normals[3 * i + 1]->z());
			glVertex3f(m_triangles[3 * i + 1].x(), m_triangles[3 * i + 1].y(), m_triangles[3 * i + 1].z());
			//glNormal3f(tr_normal.x(), tr_normal.y(), tr_normal.z());
			glNormal3f(m_normals[3 * i + 2]->x(), m_normals[3 * i + 2]->y(), m_normals[3 * i + 2]->z());
			glVertex3f(m_triangles[3 * i + 2].x(), m_triangles[3 * i + 2].y(), m_triangles[3 * i + 2].z());
		}
		glEnd();

		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_LINES);
		for (int i = 0, size = triangles_size / 3; i < size; ++i)
		{
			//draw grid
			//glVertex3f(m_triangles[3 * i].x(), m_triangles[3 * i].y(), m_triangles[3 * i].z());
			//glVertex3f(m_triangles[3 * i + 1].x(), m_triangles[3 * i + 1].y(), m_triangles[3 * i + 1].z());

			//glVertex3f(m_triangles[3 * i].x(), m_triangles[3 * i].y(), m_triangles[3 * i].z());
			//glVertex3f(m_triangles[3 * i + 2].x(), m_triangles[3 * i + 2].y(), m_triangles[3 * i + 2].z());

			//glVertex3f(m_triangles[3 * i + 1].x(), m_triangles[3 * i + 1].y(), m_triangles[3 * i + 1].z());
			//glVertex3f(m_triangles[3 * i + 2].x(), m_triangles[3 * i + 2].y(), m_triangles[3 * i + 2].z());

			//draw triangles
			glVertex3f(m_triangles[3 * i].x(), m_triangles[3 * i].y(), m_triangles[3 * i].z());
			glVertex3f((m_triangles[3 * i] + 10.0f* *m_normals[3 * i]).x(), (m_triangles[3 * i] + 10.0f* *m_normals[3 * i]).y(), (m_triangles[3 * i] + 10.0f* *m_normals[3 * i]).z());

			glVertex3f(m_triangles[3 * i + 1].x(), m_triangles[3 * i + 1].y(), m_triangles[3 * i + 1].z());
			glVertex3f((m_triangles[3 * i + 1] + 10.0f* *m_normals[3 * i + 1]).x(), (m_triangles[3 * i + 1] + 10.0f* *m_normals[3 * i + 1]).y(), (m_triangles[3 * i + 1] + 10.0f* *m_normals[3 * i + 1]).z());

			glVertex3f(m_triangles[3 * i + 2].x(), m_triangles[3 * i + 2].y(), m_triangles[3 * i + 2].z());
			glVertex3f((m_triangles[3 * i + 2] + 10.0f* *m_normals[3 * i + 2]).x(), (m_triangles[3 * i + 2] + 10.0f* *m_normals[3 * i + 2]).y(), (m_triangles[3 * i + 2] + 10.0f* *m_normals[3 * i + 2]).z());
		}
		glEnd();
	}


	bool bezier_path::is_inside(const Vector2f &p) const
	{
		Vector2f outside(FLT_MAX, 0.0f);

		int intersects = 0;

		for (int i = 0, size = m_polygon.size(); i < size; ++i)
		{
			int idx = i;
			int idx_next = index(i + 1, size);

			float l3 = signed_left(m_polygon[idx], m_polygon[idx_next], outside);
			float l4 = signed_left(m_polygon[idx], m_polygon[idx_next], p);
			float l1 = signed_left(p, outside, m_polygon[idx]);
			float l2 = signed_left(p, outside, m_polygon[idx_next]);

			if (((l3>0.0f) ^ (l4>0.0f) || (l4>=0.0f))&& ((l1>0.0f) ^ (l2>0.0f) || (l1>=0.0f)))
				++intersects;
		}

		return intersects % 2;
	}


	bool  bezier_path::is_inside_rectangle(const Vector2f &p) const
	{
		return left(m_North, m_East, p) && left(m_East, m_South, p) &&
			left(m_South, m_West, p) && left(m_West, m_North, p);
	}

	bool less(const bezier_path *lhs, const bezier_path *rhs)
	{
		return lhs->get_depth() < rhs->get_depth();
	}
}