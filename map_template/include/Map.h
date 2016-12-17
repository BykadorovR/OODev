#ifndef MY_MAP
#define MY_MAP

#include <Eigen>
#include "Interfaces.h"


	class Reconnaissance
	{
	public:
		///@brief perfrorm reconnaissance
		///@param x is the point of region that needs to be determinated
		///@return corresponding region
		int find_region(const Vector2f &x) const {
			return 0;
		}
	private:
	};

	namespace our {
		///@brief simple map class
		class Map : public IMap, public IDrawable
		{
		public:
			Map() {

			}
			~Map();
			///@brief add new path
			///@param path is bezier path
			virtual void add_path(const bezier_path *path) {
				m_paths.push_back(path);
			}

			///@brief remove path
			///@param id of the bezier path
			virtual bool remove_path(const int id) {
				if (id < 0 || id >= m_paths.size())
					return false;
				m_paths.erase(m_paths.begin() + id);
				return true;
			}

			///@brief get all bezier paths
			///@return const reference to vector of const bezier paths
			virtual const std::vector<const bezier_path*> &get_paths() const {
				return m_paths;
			}


			///@brief draw function. contains GL functionality
			virtual void drawGL() {

			}
		
			std::vector<const bezier_path*> m_paths;
		};
	}

	///@brief shared map class
	/// each method calls base class and then
	/// modifies DB records depending on call result
	class SharedMap : public our::Map
	{
	public:
		SharedMap(const IDB *m_DB) {

		}
		~SharedMap() {

		}
		///@brief add new path
		///@param path bezier path
		virtual void add_path(const bezier_path *path) {
			//sync with DB and add path
		}

		///@brief remove path
		///@param id of the bezier path
		virtual bool remove_path(const int id) {
			if (id < 0 || id >= get_paths().size()) return false;
			//sync with db and add path
			return true;
		}

		///@brief get all bezier paths
		///@return const reference to vector of const bezier paths
		virtual const std::vector<const bezier_path*> &get_paths() const {
			//past real m_paths from DB
			return m_paths;
		}

		///@brief get all local bezier paths
		///@return const reference to vector of const local bezier paths
		virtual const std::vector<const bezier_path*> &get_local_paths() const {
			return m_paths;
		}

		///@brief draw function. contains GL functionality
		virtual void drawGL() {

		}
	private:
		const IDB *m_DB;
		const Reconnaissance *m_rec;
	private:
		//!TODO: add decomposition methods(from IDB request to normal representation)
		//!TODO: add local changes!!! should be drawn differently
	};


#endif