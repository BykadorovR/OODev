#ifndef MY_INTERFACE
#define MY_INTERFACE

#include <list>
#include "NonGeometricInterfaces.h"
#include "Geometry.h"

namespace newmeteo {
	class IView;
	class IPresenter;
	class IModel;
	class IMap;
	class IAuth;
	class IDB;
	class IDrawable;

	///@brief View interface
	class IView
	{
	public:
		///@brief redraw all
		virtual void redraw() = 0;
	};

	///@brief Presenter interface
	class IPresenter
	{
	public:
		virtual void data_was_updated() = 0;
	};

	///@brief Model interface
	class IModel
	{
	public:
		///@brief accept given presenter
		///@param presenter to be accepted
		virtual void accept(IPresenter *presenter) = 0;

		virtual void remove(IPresenter* presenter) = 0;

		virtual void data_was_updated() = 0;
	};


	///@brief Map interface
	class IMap
	{
	public:
		typedef std::list<const bezier_path*> container;
		typedef std::list<const bezier_path*>::const_iterator iterator;

		///@brief add new path
		///@param path is bezier path
		virtual void add_path(const bezier_path *path) = 0;

		///@brief remove path
		///@param id of the bezier path
		virtual void remove_path(iterator &it) = 0;

		virtual bool remove_path(int index) = 0;

		///@brief get all bezier paths
		///@return const reference to vector of const bezier paths
		virtual const container &get_paths() const = 0;

		// + some sort of iterator
	};


	///@brief Auth interface
	class IAuth
	{
	public:

		///@brief ask permissions
		///@return mangled permissions
		virtual int permission() const = 0;

		///@brief perform login
		///@return true if succeed, false otherwise
		virtual bool login(const std::string &login, const std::string &password) = 0;

		virtual const std::string &get_login() const = 0;
		//!TODO add permission checks(can read, can write, etc)

		///@brief get user id
		virtual short id() const = 0;

		///@brief id of the first path user can create and commit
		virtual short allowed_path_id() const = 0;
	};

	///@brief interface for queue with bezier paths
	class IQueue
	{
	public:
		///@brief get all bezier paths
		///@return const reference to vector of const bezier paths
		virtual const IMap::container &get_added_paths() const = 0;

		///@brief get all bezier paths
		///@return const reference to vector of const bezier paths
		virtual const IMap::container &get_removed_paths() const = 0;

		///@brief accept path
		///@param path pointer to bezier path
		virtual void accept(const IMap::iterator &path) = 0;
		virtual bool accept(int index) = 0;

		///@brief reject path
		///@param path pointer to bezier path
		virtual void reject(const IMap::iterator &path) = 0;
		virtual bool reject(int index) = 0;
	};
}
#endif