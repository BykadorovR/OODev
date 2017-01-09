#ifndef MY_MAP
#define MY_MAP

#include <Eigen>
#include <stack>
#include "Interfaces.h"

#if TESTING
#define PRIVATE_TESTABLE public
#else
#define PRIVATE_TESTABLE private
#endif

namespace newmeteo {

	class tree
	{
	public:
		void build(const IMap::container &paths, IMap::iterator start, int start_is_used, std::vector<tree*> &map)
		{
			if (start_is_used == -1)
			{
				//head
				m_path = NULL;
				++start_is_used;

				for(IMap::iterator i = start, end = paths.end(); i != end; ++i, ++start_is_used)
				{
					if (map[start_is_used])
						continue;

					tree * new_tree = new tree();
					new_tree->m_parent = this;
					map[start_is_used] = new_tree;
					new_tree->build(paths, i, start_is_used, map);
					m_subtree.push_back(new_tree);
				}
			}
			else
			{
				m_path = (*start);

				++start_is_used;
				++start;
				for (IMap::iterator i = start, end = paths.end(); i != end; ++i, ++start_is_used)
				{
					if (!this->m_path->is_intersects((*i)))
						continue;

					if (this->m_path->get_depth() == (*i)->get_depth())
						continue;

					if (map[start_is_used])
					{
						if (m_subtree.size() == 0)
						{
							//add to subtree
							m_subtree.push_back(map[start_is_used]);
						}
					}
					else
					{
						tree * new_tree = new tree();
						new_tree->m_parent = this;
						map[start_is_used] = new_tree;
						new_tree->build(paths, i, start_is_used, map);
						m_subtree.push_back(new_tree);
					}
				}
			}
		}
		~tree()
		{
			for (int i = 0; i < m_subtree.size(); ++i)
			{
				if(m_subtree[i]->m_parent == this)
					delete m_subtree[i];
			}
		}
	public:
		//NULL if it head of the three
		const bezier_path *m_path;
		tree *m_parent;
		std::vector<tree*> m_subtree;
	};


	class Reconnaissance
	{
	public:
		///@brief perfrorm reconnaissance
		///@param x is bezier line
		///@param out corresponding regions
		void find_region(const bezier_line *x, std::vector<unsigned int> &out) const
		{
			if (x == NULL) return;
			const Vector2f *points = x->get();
			Vector2i blocks[4];

			for (int i = 0; i < 4; ++i)
				blocks[i] = floor(points[i] / m_block_size).cast<int>();

			Vector2f minf = this->min(points[0], points[1]);
			minf = this->min(minf, points[2]);
			minf = this->min(minf, points[3]);

			Vector2f maxf = this->max(points[0], points[1]);
			maxf = this->max(minf, points[2]);
			maxf = this->max(minf, points[3]);

			minf = floor(minf / m_block_size);
			maxf = floor(maxf / m_block_size);

			if (minf == maxf)
			{
				out.push_back(get_id(minf.cast<int>()));
				return;
			}

			for (Vector2f p = minf; p.x() <= maxf.x(); p += Vector2f(1.0f, 0.0f))
			{
				for (p.y() = minf.y(); p.y() <= maxf.y(); p += Vector2f(0.0f, 1.0f))
				{
					if (is_intersects(p, p + Vector2f(1.0f, 0.0f), points[0] / m_block_size,
						points[1] / m_block_size, points[2] / m_block_size, points[3] / m_block_size, true, false) ||

						is_intersects(p, p + Vector2f(0.0f, 1.0f), points[0] / m_block_size,
							points[1] / m_block_size, points[2] / m_block_size, points[3] / m_block_size, true, false) ||

						is_intersects(p + Vector2f(0.0f, 1.0f), p + Vector2f(1.0f, 1.0f), points[0] / m_block_size,
							points[1] / m_block_size, points[2] / m_block_size, points[3] / m_block_size, false, false) ||

						is_intersects(p + Vector2f(1.0f, 0.0f), p + Vector2f(1.0f, 1.0f), points[0] / m_block_size,
							points[1] / m_block_size, points[2] / m_block_size, points[3] / m_block_size, false, false) ||

						is_inside(p, points[0] / m_block_size, points[1] / m_block_size, points[3] / m_block_size) ||

						is_inside(p, points[0] / m_block_size, points[2] / m_block_size, points[3] / m_block_size))
						out.push_back(get_id(round(p).cast<int>()));
				}
			}
		}


		///@brief find regions for path x
		///@param x is bezier path
		///@param out is output regions
		void find_region(const bezier_path *x, std::vector<unsigned int> &out) const
		{
			if (x == NULL) return;
			const std::vector<bezier_line*> &lines = x->get_lines();
			out.clear();
			for (unsigned int i = 0; i < lines.size(); ++i)
				find_region(lines[i], out);

			std::sort(out.begin(), out.end());
			std::vector<unsigned int>::iterator it = std::unique(out.begin(), out.end());
			out.erase(it, out.end());
		}

	PRIVATE_TESTABLE:

		// is point inside triangle [x1,x2,x3]
		bool is_inside(const Vector2f &point, const Vector2f &x1, const Vector2f &x2, const Vector2f &x3) const
		{
			const float det = (x2.y() - x3.y())*(x1.x() - x3.x()) + (x3.x() - x2.x())*(x1.y() - x3.y());
			if (abs(det) < m_det_epsilon)
				return false;
			float a = (x2.y() - x3.y())*(point.x() - x3.x()) + (x3.x() - x2.x())*(point.y() - x3.y());
			float b = (x3.y() - x1.y())*(point.x() - x3.x()) + (x1.x() - x3.x())*(point.y() - x3.y());
			a /= det;
			b /= det;
			float c = 1.0f - a - b;

			if (a < 0.0f || b < 0.0f || c < 0.0f)
				return false;

			return true;
		}
		// is [x1,x2] intersects [x3,x4] include lef, include right
		bool is_intersects(const Vector2f &x1, const Vector2f &x2, const Vector2f &x3, const Vector2f &x4, bool left, bool right) const
		{
			const float det = (x4.y() - x3.y())*(x2.x() - x1.x()) - (x4.x() - x3.x())*(x2.y() - x1.y());

			if (abs(det) < m_det_epsilon)
				return false;
			float a1 = (x4.x() - x3.x())*(x1.y() - x3.y()) - (x4.y() - x3.y())*(x1.x() - x3.x());
			float a2 = (x2.x() - x1.x())*(x1.y() - x3.y()) - (x2.y() - x1.y())*(x1.x() - x3.x());
			a1 /= det;
			a2 /= det;

			if ((left) && (a1 < 0.0f || a2 < 0.0f) || (!left) && (a1 <= 0.0f || a2 < 0.0f) ||
				(right) && (a1 > 1.0f || a2 > 1.0f) || (!right) && (a1 >= 1.0f || a2 >= 1.0f))
				return false;

			return true;
		}

		//intersect [x1,x2] with [rec1,rec2] [rec2,rec4] [rec4,rec3] [rec3,rec1]
		bool is_intersects(const Vector2f &x1, const Vector2f &x2, const Vector2f &rec1, const Vector2f &rec2, const Vector2f &rec3, const Vector2f &rec4, bool left, bool right) const
		{
			return (is_intersects(x1, x2, rec1, rec2, left, right) ||
				is_intersects(x1, x2, rec2, rec4, left, right) ||
				is_intersects(x1, x2, rec4, rec3, left, right) ||
				is_intersects(x1, x2, rec3, rec1, left, right));
		}

		Vector2f min(const Vector2f &a, const Vector2f &b) const
		{
			return Vector2f(a[0] < b[0] ? a[0] : b[0],
				a[1] < b[1] ? a[1] : b[1]);
		}

		Vector2f max(const Vector2f &a, const Vector2f &b) const
		{
			return Vector2f(a[0] > b[0] ? a[0] : b[0],
				a[1] > b[1] ? a[1] : b[1]);
		}

		Vector2f floor(const Vector2f &a) const
		{
			return Vector2f(::floor(a[0]), ::floor(a[1]));
		}

		Vector2f round(const Vector2f &a) const
		{
			return Vector2f(::round(a[0]), ::round(a[1]));
		}

		unsigned int get_id(const Vector2i &block_coord) const
		{
			assert(!(block_coord[0] < SHRT_MIN || block_coord[0] > SHRT_MAX ||
				block_coord[1] < SHRT_MIN || block_coord[1] > SHRT_MAX));

			return block_coord[0] - SHRT_MIN + USHRT_MAX * (block_coord[1] - SHRT_MIN);
		}

		const int m_block_size = 10;
		const float m_det_epsilon = 1e-10f;
	};

	///@brief simple map class
	class Map : public IMap, public IDrawable
	{
	public:
		Map()
		{
			;
		}
		~Map()
		{
			for (iterator it = m_paths.begin(); it != m_paths.end(); ++it)
				delete *it;

			clear_surface();
		}
		///@brief add new path
		///@param path is bezier path
		virtual void add_path(const bezier_path *path)
		{
			m_paths.push_back(path);

			build_surface();
		}

		///@brief remove path
		///@param id of the bezier path
		virtual void remove_path(iterator &it)
		{
			m_paths.erase(it);
		}

		virtual bool remove_path(int index)
		{
			if (index < 0 || index >= m_paths.size())
				return false;
			int l_index = 0;
			for (iterator it = m_paths.begin(); it != m_paths.end(); ++it) {
				if (l_index == index) {
					m_paths.erase(it);
					break;
				}
				l_index++;
			}
			return true;
		}


		///@brief get all bezier paths
		///@return const reference to vector of const bezier paths
		virtual const std::vector<Surface*> &get_surfaces() const
		{
			return m_surfaces;
		}

		///@brief get all bezier paths
		///@return const reference to vector of const bezier paths
		virtual const container &get_paths() const
		{
			return m_paths;
		}


		///@brief draw function. contains GL functionality
		virtual void drawGL() const {
			;
		}

		///@brief draw function. contains GL functionality
		virtual void draw3DGL() const {
			;
		}
	protected:
		void build_surface()
		{
			clear_surface();

			m_paths.sort(&less);
			std::vector<tree*> tree_map;

			tree_map.resize(m_paths.size(), NULL);

			//resize after sort
			for (IMap::iterator it = m_paths.begin(), end = m_paths.end(); it != end; ++it)
				m_nomals_map[(*it)] = std::vector<Vector3f>((*it)->get_polygon().size(), Vector3f(0.0f, 0.0f, 0.0f));

			tree *head_tree = new tree();
			head_tree->build(m_paths, m_paths.begin(), -1, tree_map);

			for(int i =0; i < head_tree->m_subtree.size(); ++i)
				add_surface(head_tree->m_subtree[i]);

			delete head_tree;
		}

		void add_surface(const tree *tree)
		{
			for (int i = 0; i < tree->m_subtree.size(); ++i)
			{
				m_surfaces.push_back(new Surface(tree->m_path->get_polygon(), m_nomals_map[tree->m_path], tree->m_path->get_depth(),
					tree->m_subtree[i]->m_path->get_polygon(), m_nomals_map[tree->m_subtree[i]->m_path], tree->m_subtree[i]->m_path->get_depth()));
				if (tree->m_subtree[i]->m_parent == tree)
					add_surface(tree->m_subtree[i]);
			}

		}

		void clear_surface()
		{
			for (int i = 0; i < m_surfaces.size(); ++i)
				delete m_surfaces[i];
			m_surfaces.resize(0);
		}

	protected:
		container m_paths;
		std::vector<Surface*> m_surfaces;

		std::map<const bezier_path*, std::vector<Vector3f> >m_nomals_map;
	};


	///@brief shared map class
	/// each method calls base class and then
	/// modifies DB records depending on call result
	class SharedMap : public Map
	{
	public:
		SharedMap(IDB *DB) : m_DB(DB)
		{
			update();
		}
		~SharedMap()
		{
			;
		}
		///@brief add new path
		///@param path bezier path
		virtual void add_path(const bezier_path *path)
		{
			//before commiting local change ask DB queue_remove if this path already exists. use path id
			//if so, error somewhere is present.

			//!!!!!!!!
			//если существует путь с таким же айди, то есть логическая ошибка
			//обычный механизм действий - поместить в очередь добавления на сервере
			//поместить в локальную копию (вызов метода ниже)
            if (m_DB->add_path(path))
            {
                Map::add_path(path);
                update();
            }
		}

		///@brief remove path
		///@param id of the bezier path
		virtual void remove_path(iterator &it)
		{
			//before commiting local change ask DB queue_add if this path already exists. use path id
			//if so, remove it from queue, since this client is owner of (iterator &it) path and path wasn't commited

			//!!!!!!!!
			//ключевой принцип - перемещаем на сервере удаляемый путь из серверной карты в очередь удаления
			//при запросе карты кем-либо выдаем саму карту + очередь удаления(т.к. те пути еще не все подтвердили удаление пути)
			//!!!!!!!!
			//если существует путь с таким же айди, в очереде удаления, то кто-то уже нас опередил, на это можно забить и не вносить изменений нигде
			//если существует путь с таким же айди, в очереде добавления, то это означает что только этот самый клиент мог его добавить
			//и является его владельцем, возвращаем этот путь из очереди удаления обратно в карту
			//если в очередях пути нет, то переносим его в очередь удаления
			//удалить из локальной копии по ситуации исходя из вышесказанного (вызов метода ниже)

            if (m_DB->remove_path((*it)->get_id()))
            {
                Map::remove_path(it);
                update();
            }
		}

		virtual bool remove_path(int index)
		{
			if (index < 0) return false;
            int t_index = 0;
            for (iterator it = m_paths.begin(); it != m_paths.end(); ++it)
            {
                if (t_index == index)
                {
                    this->remove_path(it);
                    return true;
                }
                t_index++;
            }
            return false;
		}

		///@brief get all bezier paths
		///@return const reference to vector of const bezier paths
		virtual const container &get_paths() const
		{
			return Map::get_paths();
		}

		///@brief draw function. contains GL functionality
		virtual void drawGL() const {
			;
		}
	private:
		IDB *m_DB;
		const Reconnaissance *m_rec;
	private:
        virtual bool update()
        {
			bool result = m_DB->get_paths(m_paths);
			this->build_surface();
            return result;
        }
        //!TODO: add decomposition methods(from IDB request to normal representation)
		//!TODO: add local changes!!! should be drawn differently

	};
}
#endif