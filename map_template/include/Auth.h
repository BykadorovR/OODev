#ifndef MY_AUTH
#define MY_AUTH

#include "Interfaces.h"
#include <string>

class Auth : public IAuth
{
public:
	Auth(const IDB* DB, std::string login, std::string password) {

	}
	~Auth() {
	};

	///@brief ask permissions
	///@return mangled permissions
	virtual int permission() {
		return -1;
	};
	
	///@brief perform login
	///@return true if succeed, false otherwise
	virtual bool login() {
		return true;
	};
private:
	const IDB *m_DB;
private:
	std::string m_login;
	std::string m_password;
	int m_permissions;
	int m_cache;
};

#endif