#ifndef MY_QUEUE
#define MY_QUEUE

#include "Interfaces.h"

///@brief queue with bezier paths
class Queue : public IQueue
{
public:
	Queue(const IDB *m_DB) {

	}

	///@brief get all bezier paths
	///@return const reference to vector of const bezier paths
	virtual const std::vector<const bezier_path*> &get_paths() const {
		return m_paths;
	}
	
	///@brief accept path
	///@param path pointer to bezier path
	bool accept(const bezier_path* path) {
		if (path == NULL) return false;
		return true;
	}
	
	///@brief accept path
	///@param path index of bezier path
	bool accept(const int path) {
		if (path < 0 || path >= m_paths.size()) return false;
		return true;
	}
	///@brief reject path
	///@param path pointer to bezier path
	bool reject(const bezier_path* path) {
		if (path == NULL) return false;
		return true;
	}
	
	///@brief reject path
	///@param path index of bezier path
	bool reject(const int path) {
		if (path < 0 || path >= m_paths.size()) return false;
		return true;
	}
	
	///@brief request all path to be accepted/rejected
	void request() {

	}
	std::vector<const bezier_path*> m_paths;
};


#endif