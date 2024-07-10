#include "stdafx.h"

#include "gpio.h"

#if !defined( WIN32 ) && !defined( _WIN32 ) && !defined( _WIN64 ) && !defined( _WINDOWS_ )
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#endif

unsigned char* map_base = NULL;
int dev_fd;

int gpio_init( void )
{
#if defined( WIN32 ) || defined( _WIN32 ) || defined( _WIN64 ) || defined( _WINDOWS_ )
    return -1;
#else
    dev_fd = open( "/dev/mem", O_RDWR | O_SYNC );
    if ( dev_fd < 0 )
    {
        printf( "\nopen(/dev/mem) failed.\n" );
        return -1;
    }
    map_base = (unsigned char*)mmap( 0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, dev_fd, REG_BASE );
    return 0;
#endif
}

int gpio_enable( int gpio_num, int val )
{
    int offset, gpio_move;
    if ( gpio_num > 31 )
    {
        offset = 4;
        gpio_move = gpio_num - 32;
    }
    else
    {
        offset = 0;
        gpio_move = gpio_num;
    }
    if ( val == 0 )
    {
        *(volatile unsigned int*)( map_base + GPIO_EN + offset ) |= ( 1 << gpio_move ); //GPIO使能in
                                                                                        //printf("Enable GPIO%d in\n",gpio_num);
    }
    else
    {
        *(volatile unsigned int*)( map_base + GPIO_EN + offset ) &= ~( 1 << gpio_move ); //GPIO使能out
                                                                                         //printf("Enable GPIO%d out\n",gpio_num);
    }

    return 0;
}

int gpio_close( void )
{
    if ( dev_fd < 0 )
    {
        printf( "\nopen(/dev/mem) failed.\n" );
        return -1;
    }

#if defined( WIN32 ) || defined( _WIN32 ) || defined( _WIN64 ) || defined( _WINDOWS_ )
    return -1;
#else
    munmap( map_base, MAP_SIZE ); //解除映射关系
    if ( dev_fd )
    {
        close( dev_fd );
    }

    return 0;
#endif
}

int gpio_write( int gpio_num, int val )
{
    int offset, gpio_move;

    if ( gpio_num > 31 )
    {
        offset = 4;
        gpio_move = gpio_num - 32;
    }
    else
    {
        offset = 0;
        gpio_move = gpio_num;
    }
    if ( val == 1 )
    {
        *(volatile unsigned int*)( map_base + GPIO_OUT + offset ) |= ( 1 << gpio_move ); //输出高
    }
    else
    {
        *(volatile unsigned int*)( map_base + GPIO_OUT + offset ) &= ~( 1 << gpio_move ); //输出低
    }

    return 1;
}

int gpio_read( int gpio_num )
{
    int offset, gpio_move;

    if ( gpio_num > 31 )
    {
        offset = 4;
        gpio_move = gpio_num - 32;
    }
    else
    {
        offset = 0;
        gpio_move = gpio_num;
    }

    return ( *(volatile unsigned int*)( map_base + GPIO_IN + offset ) >> gpio_move ) & 0x01; //读取
}
