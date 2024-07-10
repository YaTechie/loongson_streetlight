#include "stdafx.h"

#include "net_client.h"


bool net_client::start( std::string_view path )
{
    fmt::print( "net_client starting...\n" );

    if ( !iopool_.start() )
    {
        fmt::print( "net_client start error.\n" );
        return false;
    }

    if ( !_init( path ) )
    {
        fmt::print( "net_client start error.\n" );
        return false;
    }

    run_ = true;

    threads_.create_thread( std::bind( &net_client::_loop, this ) );

    _connect();

    nt_loop_.start();

    fmt::print( "net_client started.\n" );

    return true;
}

void net_client::stop( void )
{
    if ( run_ )
    {
        iopool_.wait_signal( SIGINT, SIGTERM );
    }

    fmt::print( "net_client stopping...\n" );

    run_ = false;

    nt_loop_.stop();

    threads_.join_all();

    client_.stop();
    iopool_.stop();

    fmt::print( "net_client stopped.\n" );
}

void net_client::_loop( void )
{
    auto last_time = sys_timer::get_instance().get_elapsed();
    auto cur_time = sys_timer::get_instance().get_elapsed();
    [[maybe_unused]] auto delta_time = cur_time - last_time;

    auto loop_time = 10;

    std::chrono::milliseconds sleep = std::chrono::milliseconds( loop_time );

    while ( run_ )
    {
        cur_time = sys_timer::get_instance().get_elapsed();

        delta_time = cur_time - last_time;
        last_time = cur_time;

        nt_loop_.action( delta_time );
        nt_t_.action( delta_time );

        std::this_thread::sleep_for( sleep );
    }
}

void net_client::_on_loop_timer( void )
{
    //if ( !api_mgr::get_instance().get_sync( "http://www.baidu.com", [&]( [[maybe_unused]] const char* content ) {
    //     } ) )
    //{
    //    // fmt::print( "get_sync error.\n" );
    //}
}

void net_client::_on_t_timer( void )
{
    _send( 2, "39.0" );
}

bool net_client::_init( std::string_view path )
{
    std::string file( path );
    file += "loongson_demo.ini";

    inifile::IniFile ini;
    if ( ini.Load( file ) != 0 )
    {
        return false;
    }

    ini.GetStringValue( "net", "host", &host_ );
    ini.GetStringValue( "net", "port", &port_ );

    client_.set_connect_timeout( std::chrono::seconds( 5 ) );

    return true;
}

bool net_client::_connect( void )
{
    client_.bind_init( [&]() {
               client_.ws_stream().text( true );
           } )
        .bind_connect( [&]() {
            if ( asio2::get_last_error() )
            {
                fmt::print( "connect failure.\n" );
                return;
            }

            fmt::print( "connected.\n" );

            _send( 1, "hello" );
        } )
        .bind_upgrade( [&]() {
            if ( asio2::get_last_error() )
            {
                fmt::print( "upgrade failure\n" );
                return;
            }
        } )
        .bind_recv( [&]( std::string_view recv ) {
            Json::Value json_root;
            JSONCPP_STRING json_error;

            Json::CharReaderBuilder json_builder;

            std::unique_ptr<Json::CharReader> json_reader( json_builder.newCharReader() );
            if ( !json_reader->parse( recv.data(), recv.data() + recv.length(), &json_root, &json_error ) )
            {
                return;
            }

            try
            {
                bool result = json_root["result"].asBool();
                if ( !result )
                {
                    return;
                }
            }
            catch ( ... )
            {
                return;
            }
        } );

    if ( !client_.start( host_, port_, "/websocket" ) )
    {
        return false;
    }

    return true;
}

bool net_client::_send( int commond, const char* content )
{
    Json::Value json;
    json["commond"] = commond;
    json["content"] = content;

    std::string s = {};

    try
    {
        Json::StreamWriterBuilder json_builder;
        s = Json::writeString( json_builder, json );
    }
    catch ( ... )
    {
        return false;
    }

    client_.async_send( std::move( s ) );

    return true;
}
