#ifndef sys_timer_h__
#define sys_timer_h__

#include <chrono>


class sys_timer
{
public:
    sys_timer() = default;
    ~sys_timer() = default;

public:
    inline static sys_timer& get_instance( void )
    {
        static sys_timer instance;
        return instance;
    }

public:
    //纳秒
    int64_t get_elapsed_nano( void )
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>( std::chrono::high_resolution_clock::now().time_since_epoch() ).count();
    }

    //微秒
    int64_t get_elapsed_micro( void )
    {
        return std::chrono::duration_cast<std::chrono::microseconds>( std::chrono::high_resolution_clock::now().time_since_epoch() ).count();
    }

    //毫秒
    int64_t get_elapsed( void )
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::high_resolution_clock::now().time_since_epoch() ).count();
    }

    //秒
    int64_t get_elapsed_seconds( void )
    {
        return std::chrono::duration_cast<std::chrono::seconds>( std::chrono::high_resolution_clock::now().time_since_epoch() ).count();
    }

    //分
    int get_elapsed_minutes( void )
    {
        return std::chrono::duration_cast<std::chrono::minutes>( std::chrono::high_resolution_clock::now().time_since_epoch() ).count();
    }

    //时
    int get_elapsed_hours( void )
    {
        return std::chrono::duration_cast<std::chrono::hours>( std::chrono::high_resolution_clock::now().time_since_epoch() ).count();
    }

    int64_t get_time( void )
    {
        return get_elapsed();
    }

public:
    template <class Duration>
    int64_t get_elapsed_duration( Duration& duration )
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>( duration + std::chrono::high_resolution_clock::now().time_since_epoch() ).count();
    }
};

#endif // sys_timer_h__
