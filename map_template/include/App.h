#ifndef MY_WX_APP
#define MY_WX_APP

#include <queue>
#include <wx\wx.h>
#include "Model.h"

#include <mysql.h>
namespace newmeteo {

 class MY_DB : public IDB
    {
        // IDB
        virtual bool request()
        {
            return true;
        }
        virtual bool login(std::string login, std::string password, int& id, int& permissions, int& path_id)
        {
            if (exec("select * from users where login = '" + login + "' and password = '" + password + "'")) 
            {
                if (storeResult()) 
                {
                    if (getRowCount())
                    {
                        id = stoi(getFieldValue(0, "id"));
                        permissions = stoi(getFieldValue(0, "permissions"));
                        path_id = stoi(getFieldValue(0, "pathid"));
                        return true;
                    }

                }
            }
            return false;
        }

        virtual bool connect()
        {
            if (!init())
                return false;
            try
            {
                if (!mysql_real_connect(conn, dbHost, dbUser, dbPassword, dbName, dbPort, 0, 0))
                {
                    return false;
                }
            }
            catch (...) {
                return false;
            }
            return true;
        }

        virtual bool get_paths(std::list<const bezier_path*>& paths)
        {
            if (exec("select * from paths"))
            {
                if (storeResult())
                {
                    int numpaths = getRowCount();
                    std::vector<std::string> ids;
                    for (int i = 0; i < numpaths; i++)
                    {
                        ids.push_back(getFieldValue(i, "pathid"));
                    }
                    paths.clear();
                    for (int pathid = 0; pathid < ids.size(); pathid++)
                    {
                        if (exec("select * from paths where pathid = '" + ids[pathid] + "'"))
                        {
                            if (storeResult())
                            {

                                int t_depth = stoi(getFieldValue(0, "depth"));
                                std::string t_comment = getFieldValue(0, "comment");
                                std::vector<bezier_line*> t_lines;

                                if (exec("select * from lines where pathid = '" + ids[pathid] + "'"))
                                {
                                    if (storeResult())
                                    {
                                        int numlines = getRowCount();
                                        for (int lineid = 0; lineid < numlines; lineid++)
                                        {
                                            Vector2f t_dots[4];
                                            t_dots[0] = Vector2f(stof(getFieldValue(lineid, "spx")), stof(getFieldValue(lineid, "spy")));
                                            t_dots[1] = Vector2f(stof(getFieldValue(lineid, "d1x")), stof(getFieldValue(lineid, "d1y")));
                                            t_dots[2] = Vector2f(stof(getFieldValue(lineid, "d2x")), stof(getFieldValue(lineid, "d2y")));
                                            t_dots[3] = Vector2f(stof(getFieldValue(lineid, "epx")), stof(getFieldValue(lineid, "epy")));

                                            t_lines.push_back(new bezier_line(t_dots));
                                        }
                                    }
                                }
                                paths.push_back(new bezier_path(t_lines, stoi(ids[pathid]), t_depth, t_comment));
                            }
                        }
                    }
                }
            }
            return true;
        }

    public:
        MY_DB()
        {
            conn = NULL;
            res = NULL;
            fields = NULL;
        }

        ~MY_DB()
        {
            if (res != NULL) mysql_free_result(res);
            if (conn != NULL) mysql_close(conn);
        }

        virtual bool init()
        {
            conn = mysql_init(NULL);
            if (conn == NULL)
            {
                return false;
            }
            return true;
        }

    private:
        virtual bool query(std::string sql)
        {
            if (res != NULL) mysql_free_result(res);
            if (mysql_query(conn, sql.c_str()) != 0)
            {
                return false;
            }
            return true;
        }

        virtual bool exec(std::string sql)
        {
            if (mysql_query(conn, sql.c_str()) != 0)
            {
                return false;
            }
            return true;
        }

        virtual int active(std::string sql)
        {
            if (exec(sql)) {
                if (storeResult()) {
                    int count = getRowCount();
                    return count;
                }
            }
            return 0;
        }

        virtual bool storeResult()

        {
            if (res != NULL) mysql_free_result(res);
            res = mysql_store_result(conn);
            fields = mysql_fetch_fields(res);
            if (res) return true;
            return false;
        }
        virtual void freeResult()
        {
            if (res != NULL) mysql_free_result(res);
            res = NULL;
            fields = NULL;
        }

        virtual int getRowCount()
        {
            return mysql_num_rows(res);
        }

        virtual int getFieldCount()
        {
            int count = mysql_num_fields(res);
            return count;
        }

        virtual std::string getFieldValue(int rowIndex, int fieldIndex)
        {
            mysql_data_seek(res, rowIndex);
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row[fieldIndex] == NULL) return "";
            return (std::string)row[fieldIndex];
        }

        virtual std::string getFieldValue(int rowIndex, std::string fieldName)
        {
            //	MYSQL_ROW row = mysql_fetch_row(res);
            int count = mysql_num_fields(res);
            for (int i = 0; i < count; i++) {
                if (fieldName == (std::string)fields[i].name) {
                    return getFieldValue(rowIndex, i);
                }
            }
            return "";
        }

    private:
        MYSQL *conn;
        MYSQL_RES *res;
        MYSQL_FIELD *fields;

        const char* dbHost = "localhost";
        const char* dbUser = "root";
        const char* dbPassword = "admin";
        const char* dbName = "my_db";
        const int dbPort = 3306;

    };

    class MyApp : public wxApp
    {
    public:

        virtual ~MyApp()
        {
            delete m_DB;
			if(m_model)
				delete m_model;
        }
        virtual bool OnInit();

        void MyApp::CloseView(wxCloseEvent& event);
    private:
        IDB *m_DB;
        Model *m_model;
        std::queue<wxFrame*> m_pipeline;
    };
  
}
#endif