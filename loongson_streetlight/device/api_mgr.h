#ifndef api_mgr_h__
#define api_mgr_h__

#include "api_session.h"

class api_mgr
{
public:
    api_mgr( void ) = default;
    ~api_mgr( void ) = default;

public:
    inline static api_mgr& get_instance( void )
    {
        static api_mgr instance;
        return instance;
    }

public:
    bool start( void );
    void stop( void );

public:
    bool get_sync( std::string url, api_session::func_success_type&& fn_success );
    bool post_sync( std::string url, std::string param, api_session::func_success_type&& fn_success );

private:
    static std::size_t _recv_cb( void* content, std::size_t size, std::size_t nmemb, void* userp );

private:
    int time_out_connect = 5;
    int time_out = 20;
};


#endif // api_mgr_h__