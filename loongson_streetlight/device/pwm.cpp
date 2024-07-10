#include "stdafx.h"

#include "pwm.h"

#if !defined( WIN32 ) && !defined( _WIN32 ) && !defined( _WIN64 ) && !defined( _WINDOWS_ )
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

unsigned int g_pwm = 0; //pwm0

//打开PWM接口
int pwm_export( [[maybe_unused]] unsigned int pwm )
{
    [[maybe_unused]] int fd = 0;

#if !defined( WIN32 ) && !defined( _WIN32 ) && !defined( _WIN64 ) && !defined( _WINDOWS_ )
    if ( pwm == 0 )
    {
        fd = open( "/sys/class/pwm/pwmchip0/export", O_WRONLY );
    }
    else if ( pwm == 1 )
    {
        fd = open( "/sys/class/pwm/pwmchip1/export", O_WRONLY );
    }
    else if ( pwm == 2 )
    {
        fd = open( "/sys/class/pwm/pwmchip2/export", O_WRONLY );
    }
    else if ( pwm == 3 )
    {
        fd = open( "/sys/class/pwm/pwmchip3/export", O_WRONLY );
    }

    if ( fd < 0 )
    {
        printf( "\nFailed expport PWM%c\n", pwm );
        return -1;
    }

    auto w = write( fd, "0", 2 );
    std::ignore = w;

    close( fd );

#endif
    return 0;
}

//关闭PWM接口
int pwm_unexport( [[maybe_unused]] unsigned int pwm )
{
    [[maybe_unused]] int fd = 0;

#if !defined( WIN32 ) && !defined( _WIN32 ) && !defined( _WIN64 ) && !defined( _WINDOWS_ )
    if ( pwm == 0 )
    {
        fd = open( "/sys/class/pwm/pwmchip0/unexport", O_WRONLY );
    }
    else if ( pwm == 1 )
    {
        fd = open( "/sys/class/pwm/pwmchip1/unexport", O_WRONLY );
    }
    else if ( pwm == 2 )
    {
        fd = open( "/sys/class/pwm/pwmchip2/unexport", O_WRONLY );
    }
    else if ( pwm == 3 )
    {
        fd = open( "/sys/class/pwm/pwmchip3/unexport", O_WRONLY );
    }
    if ( fd < 0 )
    {
        printf( "\nFailed unexpport PWM%d\n", pwm );
        return -1;
    }

    auto w = write( fd, "0", 2 );
    std::ignore = w;

    close( fd );

#endif
    return 0;
}
//使能pwm
int pwm_enable( [[maybe_unused]] unsigned int pwm )
{
    [[maybe_unused]] int fd = 0;

#if !defined( WIN32 ) && !defined( _WIN32 ) && !defined( _WIN64 ) && !defined( _WINDOWS_ )
    if ( pwm == 0 )
    {
        fd = open( "/sys/class/pwm/pwmchip0/pwm0/enable", O_WRONLY );
    }
    else if ( pwm == 1 )
    {
        fd = open( "/sys/class/pwm/pwmchip1/pwm0/enable", O_WRONLY );
    }
    else if ( pwm == 2 )
    {
        fd = open( "/sys/class/pwm/pwmchip2/pwm0/enable", O_WRONLY );
    }
    else if ( pwm == 3 )
    {
        fd = open( "/sys/class/pwm/pwmchip3/pwm0/enable", O_WRONLY );
    }

    if ( fd < 0 )
    {
        printf( "\nFailed enable PWM%d\n", pwm );
        return -1;
    }

    auto w = write( fd, "1", 2 );
    std::ignore = w;

    close( fd );
#endif

    return 0;
}
//禁止使能pwm
int pwm_disable( [[maybe_unused]] unsigned int pwm )
{
    [[maybe_unused]] int fd = 0;

#if !defined( WIN32 ) && !defined( _WIN32 ) && !defined( _WIN64 ) && !defined( _WINDOWS_ )
    if ( pwm == 0 )
    {
        fd = open( "/sys/class/pwm/pwmchip0/pwm0/enable", O_WRONLY );
    }
    else if ( pwm == 1 )
    {
        fd = open( "/sys/class/pwm/pwmchip1/pwm0/enable", O_WRONLY );
    }
    else if ( pwm == 2 )
    {
        fd = open( "/sys/class/pwm/pwmchip2/pwm0/enable", O_WRONLY );
    }
    else if ( pwm == 3 )
    {
        fd = open( "/sys/class/pwm/pwmchip3/pwm0/enable", O_WRONLY );
    }
    if ( fd < 0 )
    {
        printf( "\nFailed disable PWM%d\n", pwm );
        return -1;
    }

    auto w = write( fd, "0", 2 );
    std::ignore = w;

    close( fd );
#endif

    return 0;
}
//设置占空比
int pwm_period( [[maybe_unused]] unsigned int pwm, [[maybe_unused]] unsigned int period )
{
    [[maybe_unused]] int fd = 0;

    char buf[32] = "";
    memset( buf, 0, sizeof( buf ) );

    [[maybe_unused]] int len = snprintf( buf, sizeof( buf ) - 1, "%d", period );

#if !defined( WIN32 ) && !defined( _WIN32 ) && !defined( _WIN64 ) && !defined( _WINDOWS_ )
    if ( pwm == 0 )
    {
        fd = open( "/sys/class/pwm/pwmchip0/pwm0/period", O_WRONLY );
    }
    else if ( pwm == 1 )
    {
        fd = open( "/sys/class/pwm/pwmchip1/pwm0/period", O_WRONLY );
    }
    else if ( pwm == 2 )
    {
        fd = open( "/sys/class/pwm/pwmchip2/pwm0/period", O_WRONLY );
    }
    else if ( pwm == 3 )
    {
        fd = open( "/sys/class/pwm/pwmchip3/pwm0/period", O_WRONLY );
    }

    if ( fd < 0 )
    {
        printf( "\nFailed set PWM period%d\n", pwm );
        return -1;
    }

    auto w = write( fd, buf, len );
    std::ignore = w;

    close( fd );

#endif

    return 0;
}

//设置占空比
int pwm_duty_cycle( [[maybe_unused]] unsigned int pwm, [[maybe_unused]] unsigned int duty_cycle )
{
    [[maybe_unused]] int fd = 0;

    char buf[32] = "";
    memset( buf, 0, sizeof( buf ) );

    [[maybe_unused]] int len = snprintf( buf, sizeof( buf ) - 1, "%d", duty_cycle );

#if !defined( WIN32 ) && !defined( _WIN32 ) && !defined( _WIN64 ) && !defined( _WINDOWS_ )
    if ( pwm == 0 )
    {
        fd = open( "/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_WRONLY );
    }
    else if ( pwm == 1 )
    {
        fd = open( "/sys/class/pwm/pwmchip1/pwm0/duty_cycle", O_WRONLY );
    }
    else if ( pwm == 2 )
    {
        fd = open( "/sys/class/pwm/pwmchip2/pwm0/duty_cycle", O_WRONLY );
    }
    else if ( pwm == 3 )
    {
        fd = open( "/sys/class/pwm/pwmchip3/pwm0/duty_cycle", O_WRONLY );
    }

    if ( fd < 0 )
    {
        printf( "\nFailed set PWM duty_cycle%d\n", pwm );
        return -1;
    }

    auto w = write( fd, buf, len );
    std::ignore = w;

    close( fd );
#endif

    return 0;
}

//配置
int pwm_config( [[maybe_unused]] unsigned int pwm, [[maybe_unused]] unsigned int period, [[maybe_unused]] unsigned int duty_cycle )
{
    [[maybe_unused]] int fd = 0;

    char buf_p[32] = "";
    char buf_d[32] = "";

    memset( buf_p, 0, sizeof( buf_p ) );
    memset( buf_d, 0, sizeof( buf_d ) );

    [[maybe_unused]] int len_p = snprintf( buf_p, sizeof( buf_p ) - 1, "%d", period );
    [[maybe_unused]] int len_d = snprintf( buf_d, sizeof( buf_d ) - 1, "%d", duty_cycle );

#if !defined( WIN32 ) && !defined( _WIN32 ) && !defined( _WIN64 ) && !defined( _WINDOWS_ )
    if ( pwm == 0 )
    {
        fd = open( "/sys/class/pwm/pwmchip0/pwm0/period", O_WRONLY );
    }
    else if ( pwm == 1 )
    {
        fd = open( "/sys/class/pwm/pwmchip1/pwm0/period", O_WRONLY );
    }
    else if ( pwm == 2 )
    {
        fd = open( "/sys/class/pwm/pwmchip2/pwm0/period", O_WRONLY );
    }
    else if ( pwm == 3 )
    {
        fd = open( "/sys/class/pwm/pwmchip3/pwm0/period", O_WRONLY );
    }

    if ( fd < 0 )
    {
        printf( "\nFailed set PWM period%d\n", pwm );
        return -1;
    }

    auto w = write( fd, buf_p, len_p );
    std::ignore = w;

    close( fd );

    if ( pwm == 0 )
    {
        fd = open( "/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_WRONLY );
    }
    else if ( pwm == 1 )
    {
        fd = open( "/sys/class/pwm/pwmchip1/pwm0/duty_cycle", O_WRONLY );
    }
    else if ( pwm == 2 )
    {
        fd = open( "/sys/class/pwm/pwmchip2/pwm0/duty_cycle", O_WRONLY );
    }
    else if ( pwm == 3 )
    {
        fd = open( "/sys/class/pwm/pwmchip3/pwm0/duty_cycle", O_WRONLY );
    }

    if ( fd < 0 )
    {
        printf( "\nFailed set PWM duty_cycle%d\n", pwm );
        return -1;
    }

    w = write( fd, buf_d, len_d );
    std::ignore = w;


    close( fd );
#endif

    return 0;
}
//设置极性
int pwm_polarity( [[maybe_unused]] unsigned int pwm, [[maybe_unused]] int polarity )
{
    [[maybe_unused]] int fd = 0;

#if !defined( WIN32 ) && !defined( _WIN32 ) && !defined( _WIN64 ) && !defined( _WINDOWS_ )
    if ( pwm == 0 )
    {
        fd = open( "/sys/class/pwm/pwmchip0/pwm0/polarity", O_WRONLY );
    }
    else if ( pwm == 1 )
    {
        fd = open( "/sys/class/pwm/pwmchip1/pwm0/polarity", O_WRONLY );
    }
    else if ( pwm == 2 )
    {
        fd = open( "/sys/class/pwm/pwmchip2/pwm0/polarity", O_WRONLY );
    }
    else if ( pwm == 3 )
    {
        fd = open( "/sys/class/pwm/pwmchip3/pwm0/polarity", O_WRONLY );
    }

    if ( fd < 0 )
    {
        printf( "\nFailed set PWM polarity%d\n", pwm );
        return -1;
    }

    if ( polarity == 1 )
    {
        auto w = write( fd, "normal", 6 );
        std::ignore = w;
    }
    else if ( polarity == 0 )
    {
        auto w = write( fd, "inversed", 8 );
        std::ignore = w;
    }

    close( fd );
#endif

    return 0;
}
//设置极性
int pwm_read_duty_cycle( [[maybe_unused]] unsigned int pwm, [[maybe_unused]] int& polarity )
{
    [[maybe_unused]] int fd = 0;

    char buf[32] = "";
    memset( buf, 0, sizeof( buf ) );

#if !defined( WIN32 ) && !defined( _WIN32 ) && !defined( _WIN64 ) && !defined( _WINDOWS_ )
    if ( pwm == 0 )
    {
        fd = open( "/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_RDONLY );
    }
    else if ( pwm == 1 )
    {
        fd = open( "/sys/class/pwm/pwmchip1/pwm0/duty_cycle", O_RDONLY );
    }
    else if ( pwm == 2 )
    {
        fd = open( "/sys/class/pwm/pwmchip2/pwm0/duty_cycle", O_RDONLY );
    }
    else if ( pwm == 3 )
    {
        fd = open( "/sys/class/pwm/pwmchip3/pwm0/duty_cycle", O_RDONLY );
    }

    if ( fd < 0 )
    {
        printf( "\nFailed set PWM duty_cycle%d\n", pwm );
        return -1;
    }

    lseek( fd, 0, SEEK_SET );

    int r = read( fd, buf, sizeof( buf ) );
    if ( r > 0 )
    {
        try
        {
            polarity = std::stoi( buf );
        }
        catch ( ... )
        {
            // 处理转换失败的情况
        }
    }

    close( fd );
#endif

    return 0;
}