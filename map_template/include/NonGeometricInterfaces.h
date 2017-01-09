#ifndef NON_GEOMETRIC_INTERFACES
#define NON_GEOMETRIC_INTERFACES

namespace newmeteo {

    class bezier_path;

    ///@brief Data Base interface
    class IDB
    {
    public:
        //!TODO add fuctionality
        virtual bool connect() = 0;
        virtual bool request() = 0;
        virtual bool login(std::string login, std::string password, int& id, int& permissions, int& path_id) = 0;
        virtual bool get_paths(std::list<const bezier_path*>& paths) = 0;
        virtual bool add_path(const bezier_path* path) = 0;
        virtual bool remove_path(int id) = 0;
        //etc
    private:
    };

    ///@brief interface for all drawable objects
    class IDrawable
    {
    public:
        ///@brief draw function. contains GL functionality
        virtual void drawGL() const = 0;
		///@brief draw function. contains GL functionality
		virtual void draw3DGL() const = 0;
    };
}
#endif