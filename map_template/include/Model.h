#ifndef MY_MODEL
#define MY_MODEL

#include "Interfaces.h"
#include "Map.h"
#include "Auth.h"
#include "Queue.h"

class Model : public IModel, public SharedMap, private Auth, private Queue
{
public:
	///@brief constructor
	/// should create connection to DB, initialize Shared map and Auth
	/// check login and password
	Model();

	virtual ~Model();

	///@brief accept given presenter
	///@param presenter Presenter to be accepted
	virtual void accept(IPresenter* presenter);

	///@brief add new path
	///@param path is bezier path
	virtual void add_path(const bezier_path *path);

	///@brief remove path
	///@param id of the bezier path
	virtual void remove_path(const int id);

	///@brief get all bezier paths
	///@return const reference to vector of const bezier paths
	virtual const std::vector<const bezier_path*> &get_paths() const;

private:
	///@brief ask permissions
	///@return mangled permissions
	virtual int permission() const;
	
	///@brief perform login
	///@return true if succeed, false otherwise
	virtual bool login();
};


#endif