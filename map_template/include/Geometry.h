#ifndef MY_GEOMETRY
#define MY_GEOMETRY

#include <eigen>
#include <vector>
#include <string>
#include "Interfaces.h"

///@brief bezier line 
class bezier_line
{
public:
	bezier_line(const Vector2f dots[4])
	{
		;
	}
	const Vector2f* get() const
	{
		return dots;
	}
private:
	Vector2f dots[4];
};

///@brief bezier path
class bezier_path : public IDrawable
{
public:
	~bezier_path();
	///@brief perform basic test
	///@return true if path is valid
	bool test();
	///@brief draw function. contains GL functionality
	virtual void drawGL();
private:
	int depth;
	std::vector<bezier_line*> m_lines;
	std::string m_comment;
	int m_id;
};

#endif