#ifndef MY_INTERFACE
#define MY_INTERFACE

#include <vector>
#include "NonGeometricInterfaces.h"
#include "Geometry.h"

class IView;
class IPresenter;
class IModel;
class IMap;
class IAuth;

///@brief View interface
class IView
{
public:
	///@brief accept given presenter
	///@param presenter to be accepted
	virtual void accept(IPresenter *presenter) = 0;

	///@brief redraw all
	virtual void redraw() = 0;
};

///@brief Presenter interface
class IPresenter
{
public:
	///@brief register given view
	///@param view to be registered
	virtual void reg(IView *view) = 0;
	///@brief register given model
	///@param model to be registered
	virtual void reg(IModel *model) = 0;
	///@brief register given map
	///@param map to be registered
	virtual void reg(IMap *map) = 0;
	///@brief register given Auth module
	///@param auth module to be registered
	virtual void reg(IAuth *auth) = 0;
};

///@brief Model interface
class IModel
{
public:
	///@brief accept given presenter
	///@param presenter to be accepted
	virtual void accept(IPresenter *presenter) = 0;
};


///@brief Map interface
class IMap
{
public:
	///@brief add new path
	///@param path is bezier path
	virtual void add_path(const bezier_path *path) = 0;

	///@brief remove path
	///@param id of the bezier path
	virtual void remove_path(const int id) = 0;
	
	///@brief get all bezier paths
	///@return const reference to vector of const bezier paths
	virtual const std::vector<const bezier_path*> &get_paths() const = 0;

	// + some sort of iterator
};


///@brief Auth interface
class IAuth
{
public:
	
	///@brief ask permissions
	///@return mangled permissions
	virtual int permission() = 0;
	
	///@brief perform login
	///@return true if succeed, false otherwise
	virtual bool login();

	
	//!TODO add permission checks(can read, can write, etc)
};

///@brief interface for queue with bezier paths
class IQueue
{
public:
	///@brief get all bezier paths
	///@return const reference to vector of const bezier paths
	virtual const std::vector<const bezier_path*> &get_paths() const = 0;
	
	///@brief accept path
	///@param path pointer to bezier path
	virtual void accept(const bezier_path* path) = 0;
	
	///@brief accept path
	///@param path index of bezier path
	virtual void accept(const int path) = 0;
	
	///@brief reject path
	///@param path pointer to bezier path
	virtual void reject(const bezier_path* path) = 0;
	
	///@brief reject path
	///@param path index of bezier path
	virtual void reject(const int path) = 0;
};

#endif