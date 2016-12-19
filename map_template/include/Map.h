#ifndef MY_MAP
#define MY_MAP

#include <Eigen>
#include <stack>
#include "Interfaces.h"

class Reconnaissance
{
public:
	///@brief perfrorm reconnaissance
	///@param x is the point of region that needs to be determinated
	///@return corresponding region
	std::vector<unsigned int> find_region(const bezier_line *x)
	{
		


		return std::vector<unsigned int>();
	}
private:

	bool is_inside(const float det, const Vector2f &point, const Vector2f &x, const Vector2f &y, const Vector2f &z) const
	{
		return true;
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


	unsigned int get_id(const Vector2i &coord) const
	{
		assert(coord[0] < SHRT_MIN || coord[1] > SHRT_MAX);

		return coord[0] / m_block_size - SHRT_MIN + USHRT_MAX * coord[1] / m_block_size;
	}

	const int m_block_size = 10;
	std::vector<bool> m_mask;
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
	}
	///@brief add new path
	///@param path is bezier path
	virtual void add_path(const bezier_path *path)
	{
		m_paths.push_back(path);
	}

	///@brief remove path
	///@param id of the bezier path
	virtual void remove_path(iterator &it)
	{
		m_paths.erase(it);
	}

	///@brief get all bezier paths
	///@return const reference to vector of const bezier paths
	virtual const container &get_paths() const
	{
		return m_paths;
	}

	
	///@brief draw function. contains GL functionality
	virtual void drawGL() = 0;
protected:
	container m_paths;
};


///@brief shared map class
/// each method calls base class and then
/// modifies DB records depending on call result
class SharedMap : public ::Map
{
public:
	SharedMap(const IDB *m_DB)
	{
		;
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

		Map::add_path(path);
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

		Map::remove_path(it);
	}

	///@brief get all bezier paths
	///@return const reference to vector of const bezier paths
	virtual const container &get_paths() const
	{
		return Map::get_paths();
	}

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