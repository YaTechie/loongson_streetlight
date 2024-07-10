#include "stdafx.h"

#include "net_timer.h"


net_timer::net_timer( bool active, int64_t delay )
    : active_( active )
    , delay_( delay )
    , tick_( 0 )
    , count_( 0 )
    , active_count_( 0 )
    , param_( 0 )
    , fun_( nullptr )
{
}

net_timer::net_timer( bool active, int64_t delay, int count )
    : active_( active )
    , delay_( delay )
    , tick_( 0 )
    , count_( count )
    , active_count_( 0 )
    , param_( 0 )
    , fun_( nullptr )
{
}

net_timer::net_timer( bool active, int64_t delay, std::function<void( void )> fun )
    : active_( active )
    , delay_( delay )
    , tick_( 0 )
    , count_( 0 )
    , active_count_( 0 )
    , param_( 0 )
    , fun_( fun )
{
}

net_timer::net_timer( bool active, int64_t delay, int count, std::function<void( void )> fun )
    : active_( active )
    , delay_( delay )
    , tick_( 0 )
    , count_( count )
    , active_count_( 0 )
    , param_( 0 )
    , fun_( fun )
{
}

// net_timer 消息处理程序

void net_timer::start( void )
{
    active_ = true;
    active_count_ = 0;
}

void net_timer::start( int64_t tick )
{
    tick_ = tick;
    active_ = true;
    active_count_ = 0;
}

void net_timer::stop( void )
{
    active_ = false;
}

bool net_timer::action( int64_t time )
{
    if ( active_ )
    {
        if ( delay_ == 0 )
        {
            tick_ = 0;

            if ( count_ > 0 )
            {
                ++active_count_;

                if ( active_count_ >= count_ )
                {
                    active_ = false;
                }
            }

            if ( fun_ != nullptr )
            {
                fun_();
            }

            return true;
        }
        else
        {
            tick_ += time;

            if ( tick_ >= delay_ )
            {
                tick_ %= delay_;

                if ( count_ > 0 )
                {
                    ++active_count_;

                    if ( active_count_ >= count_ )
                    {
                        active_ = false;
                    }
                }

                if ( fun_ != nullptr )
                {
                    fun_();
                }

                return true;
            }
        }
    }

    return false;
}
