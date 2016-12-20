#ifndef MY_AUTH
#define MY_AUTH

#include "Interfaces.h"
#include <string>

namespace newmeteo {
    enum
    {
        PERMISSION_NONE,
        PERMISSION_READER
    };

    class Auth : public IAuth
    {

    public:
        Auth(IDB* DB) : m_DB(DB)
        {
            ;
        }
        ~Auth()
        {
            ;
        }

        ///@brief ask permissions
        ///@return mangled permissions
        virtual int permission() const
        {
            return m_permissions;
        }

        ///@brief perform login
        ///@return true if succeed, false otherwise
        virtual bool login(const std::string &login, const std::string &password)
        {
            if (m_DB->login(login, password, m_id, m_permissions, m_pathid))
            {
                m_login = login;
                m_password = password;
                return true;
            }
            else
                return false;

        }

        virtual const std::string &get_login() const
        {
            return m_login;
        }

        ///@brief get user id
        virtual short id() const
        {
            return m_id;
        }

        ///@brief id of the first path user can create and commit
        virtual short allowed_path_id() const
        {
            return m_pathid;
        }

    private:
        IDB *m_DB;
    private:
        std::string m_login;
        std::string m_password;
        int m_permissions;
        int m_pathid;
        int m_id;
    };
}
#endif