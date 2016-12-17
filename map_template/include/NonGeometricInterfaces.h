#ifndef NON_GEOMETRIC_INTERFACES
#define NON_GEOMETRIC_INTERFACES
///@brief Data Base interface
class IDB
{
public:
	//!TODO add fuctionality
	virtual bool connect() = 0;
	virtual bool request() = 0;
	//etc
private:
};

///@brief interface for all drawable objects
class IDrawable
{
public:
	///@brief draw function. contains GL functionality
	virtual void drawGL() = 0;
};

#endif