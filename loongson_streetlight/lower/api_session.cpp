#include "stdafx.h"

#include "api_session.h"


bool api_session::start( void )
{
    content_ = reinterpret_cast<uint8_t*>( malloc( 1 ) );
    if ( content_ == nullptr )
    {
        return false;
    }

    content_[0] = 0;

    len_ = 0;

    return true;
}

void api_session::stop( void )
{
    if ( headers_ != nullptr )
    {
        curl_slist_free_all( headers_ );
        headers_ = nullptr;
    }

    if ( content_ != nullptr )
    {
        free( content_ );
        content_ = nullptr;
    }

    len_ = 0;
}

void api_session::trace( void )
{
}

void api_session::add_headers( const char* header )
{
    headers_ = curl_slist_append( headers_, header );
}

bool api_session::add_content( void* content, std::size_t len )
{
    if ( len == 0 )
    {
        return true;
    }

    uint8_t* p = reinterpret_cast<uint8_t*>( realloc( content_, len_ + len + 1 ) );
    if ( p == nullptr )
    {
        return false;
    }

    content_ = p;

    memcpy( &content_[len_], content, len );

    len_ += len;
    content_[len_] = 0;

    return true;
}

void api_session::success( void )
{
    if ( fn_success_ != nullptr )
    {
        fn_success_( reinterpret_cast<char*>( content_ ) );
    }
}

void api_session::error( int result, int res_code )
{
    if ( fn_error_ != nullptr )
    {
        //curl_easy_strerror(result)

        fn_error_( result, res_code );
    }
}
