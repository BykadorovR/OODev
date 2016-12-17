#ifndef MY_GEOMETRY
#define MY_GEOMETRY

#include <eigen>
#include <vector>
#include <string>
#include "NonGeometricInterfaces.h"

///@brief bezier line 
class bezier_line
{
public:
	bezier_line(const Vector2f dots[4])
	{
		for (int i = 0; i < 4; i++) {
			this->dots[i] = dots[i];
		}
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
	~bezier_path() {

	}
	///@brief perform basic test
	///@return true if path is valid
	bool test() {
		return true;
	}

	void setLines(std::vector<bezier_line*> lines) {
		m_lines = lines;
	}
	///@brief draw function. contains GL functionality
	virtual void drawGL() {

	}

	int depth;
	std::vector<bezier_line*> m_lines;
	std::string m_comment;
	int m_id;
};

#endif