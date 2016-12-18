#ifndef MY_PRESENTERS
#define MY_PRESENTERS

#include <wx\wx.h>

#include "Interfaces.h"
#include "LoginView.h"
#include "events.h"

class MainPresenter : public IPresenter
{
public:
	MainPresenter(IView *view);
	~MainPresenter();


	///@brief register given view
	///@param view to be registered
	virtual void reg(IView *view);
	///@brief register given model
	///@param model to be registered
	virtual void reg(IModel *model);
	///@brief register given map
	///@param map to be registered
	virtual void reg(IMap *map);
	///@brief register given Auth module
	///@param auth module to be registered
	virtual void reg(IAuth *auth);

	void buttion1_click(/*wxEvent *ev*/);
	void buttion2_click(/*wxEvent *ev*/);
	void buttion3_click(/*wxEvent *ev*/);
private:
};




#endif