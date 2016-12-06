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
	int find_region(const Vector2f &x) const;
private:
};

///@brief simple map class
class Map : public IMap, public IDrawable
{
public:
	Map();
	~Map();
	///@brief add new path
	///@param path is bezier path
	virtual void add_path(const bezier_path *path);

	///@brief remove path
	///@param id of the bezier path
	virtual void remove_path(const int id);

	///@brief get all bezier paths
	///@return const reference to vector of const bezier paths
	virtual const std::vector<const bezier_path*> &get_paths() const;

	
	///@brief draw function. contains GL functionality
	virtual void drawGL() = 0;
protected:
	std::vector<const bezier_path*> m_paths;
};


///@brief shared map class
/// each method calls base class and then
/// modifies DB records depending on call result
class SharedMap : public ::Map
{
public:
	SharedMap(const IDB *m_DB);
	~SharedMap();
	///@brief add new path
	///@param path bezier path
	virtual void add_path(const bezier_path *path);

	///@brief remove path
	///@param id of the bezier path
	virtual void remove_path(const int id);

	///@brief get all bezier paths
	///@return const reference to vector of const bezier paths
	virtual const std::vector<const bezier_path*> &get_paths() const;

	///@brief get all local bezier paths
	///@return const reference to vector of const local bezier paths
	virtual const std::vector<const bezier_path*> &get_local_paths() const;

	///@brief draw function. contains GL functionality
	virtual void drawGL() = 0;
private:
	const IDB *m_DB;
	const Reconnaissance *m_rec;
private:
	//!TODO: add decomposition methods(from IDB request to normal representation)
	//!TODO: add local changes!!! should be drawn differently
};

#endif