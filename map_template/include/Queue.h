#ifndef MY_QUEUE
#define MY_QUEUE

#include "Interfaces.h"

///@brief queue with bezier paths
class Queue : public IQueue
{
public:
	Queue(const IDB *m_DB);

	///@brief get all bezier paths
	///@return const reference to vector of const bezier paths
	virtual const std::vector<const bezier_path*> &get_paths() const;
	
	///@brief accept path
	///@param path pointer to bezier path
	void accept(const bezier_path* path);
	
	///@brief accept path
	///@param path index of bezier path
	void accept(const int path);
	
	///@brief reject path
	///@param path pointer to bezier path
	void reject(const bezier_path* path);
	
	///@brief reject path
	///@param path index of bezier path
	void reject(const int path);
private:
	
	///@brief request all path to be accepted/rejected
	void request();
	std::vector<const bezier_path*> m_paths;
};


#endif