#include "stdafx.h"

#include "api_mgr.h"
#include "net_client.h"

bool pwm_init( void )
{
    //打开PWM接口
    if ( pwm_export( g_pwm ) != 0 )
    {
        return false;
    }

    // 设置PWM周期

    int period = 1000; //周期 ns

    if ( pwm_period( g_pwm, period ) != 0 )
    {
        return false;
    }

    // 设置PWM占空比

    int duty_cycle = 0;
    if ( pwm_duty_cycle( g_pwm, duty_cycle ) != 0 )
    {
        return false;
    }

    //使能pwm
    if ( pwm_enable( g_pwm ) != 0 )
    {
        return false;
    }

    return true;
}

void pwm_uninit( void )
{
    pwm_unexport( g_pwm );
}

bool rs485_init( void )
{
    if ( rs485_init( "/dev/ttyS3" ) != 0 )
    {
        return false;
    }

    return true;
}

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

    if ( !pwm_init() )
    {
        api_mgr::get_instance().stop();
        return -1;
    }

    if ( !rs485_init() )
    {
        pwm_uninit();

        api_mgr::get_instance().stop();
        return -1;
    }

    net_client client;
    client.start( path );

    client.stop();

    pwm_uninit();

    api_mgr::get_instance().stop();

    return 1;
}