#include "stdafx.h"

#include "api_mgr.h"
#include "net_client.h"

int main( [[maybe_unused]] int argc, [[maybe_unused]] char* argv[] )
{
    std::string path = argv[0];

#if defined( WIN32 ) || defined( _WIN32 ) || defined( _WIN64 ) || defined( _WINDOWS_ )
    auto p = path.find_last_of( "\\" );

    path = path.substr( 0, p + 1 );
#else
    auto p = path.find_last_of( "/" );

    path = path.substr( 0, p + 1 );
#endif

    api_mgr::get_instance().start();

    net_client client;
    client.start( path );

    client.stop();

    api_mgr::get_instance().stop();

    return 1;
}