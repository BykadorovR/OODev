#ifndef MY_MODEL
#define MY_MODEL

#include "Interfaces.h"
#include "Map.h"
#include "Auth.h"
#include "Queue.h"
namespace newmeteo {
	class Model : public IModel, public SharedMap, public Auth, private Queue
	{
	public:
		///@brief constructor
		/// should create connection to DB, initialize Shared map and Auth
		/// check login and password
		Model(IDB *DB) : SharedMap(DB), Auth(DB), Queue(DB)
		{
			;
		}

		virtual ~Model()
		{
			;
		}

		///@brief accept given presenter
		///@param presenter Presenter to be accepted
		virtual void accept(IPresenter* presenter)
		{
			m_presenters.push_back(presenter);
		}

		virtual void remove(IPresenter* presenter)
		{
			for (std::list<IPresenter*>::iterator it = m_presenters.begin(); it != m_presenters.end(); ++it)
			{
				if (*it == presenter)
				{
					m_presenters.erase(it);
					return;
				}
			}
		}

		///@brief add new path
		///@param path is bezier path
		virtual void add_path(const bezier_path *path)
		{
			SharedMap::add_path(path);
		}

		///@brief remove path
		///@param id of the bezier path
		virtual void remove_path(iterator it)
		{
			;
		}

		///@brief get all bezier paths
		///@return const reference to vector of const bezier paths
		virtual const container &get_paths() const
		{
			return SharedMap::get_paths();
		}


		///@brief get all bezier paths
		///@return const reference to vector of const bezier paths
		virtual const IMap::container &get_added_paths() const
		{

			return m_added_paths;
		}
		
		///@brief get all bezier paths
		///@return const reference to vector of const bezier paths
		virtual const IMap::container &get_removed_paths() const
		{
			return m_removed_paths;
		}

		virtual void drawGL()
		{
			;
		}

	private:
		virtual void data_was_updated()
		{
			for (std::list<IPresenter*>::iterator it = m_presenters.begin(); it != m_presenters.end(); ++it)
			{
				(*it)->data_was_updated();
			}
		}
		std::list<IPresenter*> m_presenters;

	};

}
#endif