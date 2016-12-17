#ifndef MY_MAIN_VIEW

#include <wx\wx.h>
#include <wx\frame.h>

#include "Interfaces.h"

///@brief main view, window by itself
class MainView : public IView, public wxFrame//wxWindow
{
public:
	///@brief construct interface, do OGL init
	///! create own presenter, bind events to presenter
	///@param map is object to perform data binding
	MainView(const IMap *map);

	MainView(const wxString& title, const wxPoint& pos, const wxSize& size);

	virtual ~MainView()
	{;}

	///@brief accept given presenter
	///@param presenter to be accepted
	virtual void accept(IPresenter *presenter)
	{;}

	///@brief redraw all
	virtual void redraw()
	{;}
private:
	// !NOTE no event handling here
	// only private fields
};

#endif
