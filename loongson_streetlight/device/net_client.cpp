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
    connect_ = false;

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
        nt_active_.action( delta_time );

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

    if ( !connect_ )
    {
        return;
    }

    int duty_cycle = 0;
    if ( pwm_read_duty_cycle( g_pwm, duty_cycle ) == 0 )
    {
        if ( duty_cycle_ != duty_cycle )
        {
            duty_cycle_ = duty_cycle;

            std::unordered_map<std::string, std::string> content = { { "data", std::to_string( duty_cycle_ ) } };
            _send( net_code::pwm_set_duty_cycle, content );

            //fmt::print( "duty_cycle: {}\n", duty_cycle );
        }
    }

    double pm25 = 0;
    if ( read_pm2p5( pm25 ) )
    {
        if ( pm25_ != pm25 )
        {
            pm25_ = pm25;

            std::unordered_map<std::string, std::string> content = { { "data", std::to_string( pm25_ ) } };
            _send( net_code::pm2p5, content );

            //fmt::print( "pm25: {}\n", pm25 );
        }
    }

    double wind_speed = 0;
    if ( read_wind_speed( wind_speed ) )
    {
        if ( wind_speed_ != wind_speed )
        {
            wind_speed_ = wind_speed;

            std::unordered_map<std::string, std::string> content = { { "data", std::to_string( wind_speed_ ) } };
            _send( net_code::wind_speed, content );

            fmt::print( "wind_speed: {}\n", wind_speed );
        }
    }

    std::string direction;
    int angle = 0;
    if ( read_wind_direction( direction, angle ) )
    {
        if ( direction_ != direction || angle_ != angle )
        {
            direction_ = direction;
            angle_ = angle;

            std::unordered_map<std::string, std::string> content = { { "direction", direction_ }, { "angle", std::to_string( angle_ ) } };
            _send( net_code::wind_direction, content );

            //fmt::print( "direction: {} {}\n", direction_,angle_ );
        }
    }

    double humidity = 0;
    double temperature = 0;
    double light = 0;

    if ( read_thl( humidity, temperature, light ) )
    {
        if ( humidity_ != humidity || temperature_ != temperature || light_ != light )
        {
            humidity_ = humidity;
            temperature_ = temperature;
            light_ = light;

            std::unordered_map<std::string, std::string> content = { { "humidity", std::to_string( humidity_ ) }, { "temperature", std::to_string( temperature_ ) }, { "light", std::to_string( light_ ) } };
            _send( net_code::thl, content );
        }
        //fmt::print( "thl: {} {} {}\n", humidity, temperature, light );
    }
}

void net_client::_on_active_timer( void )
{
    if ( !connect_ )
    {
        return;
    }

    _send( net_code::active, nullptr );
}

bool net_client::_init( std::string_view path )
{
    std::string file( path );
    file += "device.ini";

    inifile::IniFile ini;
    if ( ini.Load( file ) != 0 )
    {
        return false;
    }

    ini.GetStringValue( "net", "host", &host_ );
    ini.GetStringValue( "net", "port", &port_ );
    ini.GetStringValue( "system", "id", &id_ );

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

            std::unordered_map<std::string, std::string> content = { { "id", id_ } };
            _send( net_code::login, content );
        } )
        .bind_disconnect( [&]() {
            connect_ = false;
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
                auto command = json_root["command"].asInt();
                if ( command == net_code::active_resp )
                {
                }
                else if ( command == net_code::login_resp )
                {
                    auto result = json_root["result"].asBool();
                    if ( !result )
                    {
                        client_.stop();
                        return;
                    }

                    duty_cycle_ = 0;
                    pm25_ = 0;
                    wind_speed_ = 0;
                    direction_ = {};
                    angle_ = 0;
                    humidity_ = 0;
                    temperature_ = 0;
                    light_ = 0;

                    connect_ = true;
                }
                else if ( command == net_code::pwm_set_duty_cycle )
                {
                    auto data = json_root["data"].asInt();
   
                    pwm_duty_cycle( g_pwm, data );
                }
            }
            catch ( ... )
            {
                client_.stop();
                return;
            }
        } );

    if ( !client_.start( host_, port_, "/websocket" ) )
    {
        return false;
    }

    return true;
}

bool net_client::_send( int command, std::unordered_map<std::string, std::string>& data )
{
    Json::Value jsonContent;
    for ( auto i : data )
    {
        jsonContent[i.first] = i.second;
    }

    std::string content = {};

    try
    {
        Json::StreamWriterBuilder json_builder;
        content = Json::writeString( json_builder, jsonContent );
    }
    catch ( ... )
    {
        return false;
    }

    Json::Value json;
    json["command"] = command;
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

bool net_client::_send( int command, const char* content )
{
    Json::Value json;
    json["command"] = command;

    if ( content != nullptr )
    {
        json["content"] = content;
    }

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
