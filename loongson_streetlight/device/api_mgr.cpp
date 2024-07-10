#include "stdafx.h"

#include "api_mgr.h"


bool api_mgr::start( void )
{
    curl_global_init( CURL_GLOBAL_ALL );

    return true;
}

void api_mgr::stop( void )
{
    curl_global_cleanup();
}

bool api_mgr::get_sync( std::string url, api_session::func_success_type&& fn_success )
{
    api_session::ptr api_session_ptr = std::make_shared<api_session>( std::move( fn_success ), nullptr );
    if ( api_session_ptr == nullptr || !api_session_ptr->start() )
    {
        return false;
    }

    CURL* curl = curl_easy_init();
    if ( curl == nullptr )
    {
        api_session_ptr->stop();

        return false;
    }

    curl_easy_setopt( curl, CURLOPT_URL, url.c_str() );
    curl_easy_setopt( curl, CURLOPT_FOLLOWLOCATION, 1L );
    curl_easy_setopt( curl, CURLOPT_SSL_VERIFYHOST, 0 );
    curl_easy_setopt( curl, CURLOPT_SSL_VERIFYPEER, 0 );
    curl_easy_setopt( curl, CURLOPT_TIMEOUT, time_out );
    curl_easy_setopt( curl, CURLOPT_CONNECTTIMEOUT, time_out_connect );
    curl_easy_setopt( curl, CURLOPT_USERAGENT, "Chrome/107.0.0.0" );

    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, _recv_cb );
    curl_easy_setopt( curl, CURLOPT_WRITEDATA, reinterpret_cast<api_session*>( api_session_ptr.get() ) );

    curl_share_setopt( curl, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS );

    CURLcode res = CURLE_OK;
    try
    {
        res = curl_easy_perform( curl );
    }
    catch ( ... )
    {
        res = CURL_LAST;
    }

    if ( res != CURLE_OK )
    {
        if ( curl != nullptr )
        {
            curl_easy_cleanup( curl );
            curl = nullptr;
        }

        api_session_ptr->stop();

        return false;
    }

    api_session_ptr->success();

    if ( curl != nullptr )
    {
        curl_easy_cleanup( curl );
        curl = nullptr;
    }

    api_session_ptr->stop();

    return true;
}

bool api_mgr::post_sync( std::string url, std::string param, api_session::func_success_type&& fn_success )
{

    api_session::ptr api_session_ptr = std::make_shared<api_session>( std::move( fn_success ), nullptr );
    if ( api_session_ptr == nullptr || !api_session_ptr->start() )
    {
        return false;
    }

    CURL* curl = curl_easy_init();
    if ( curl == nullptr )
    {
        api_session_ptr->stop();

        return false;
    }

    api_session_ptr->set_param( param );
    api_session_ptr->add_headers( "Content-Type:application/x-www-form-urlencoded;charset=UTF-8" );

    curl_easy_setopt( curl, CURLOPT_CUSTOMREQUEST, "POST" );
    curl_easy_setopt( curl, CURLOPT_URL, url.c_str() );
    curl_easy_setopt( curl, CURLOPT_FOLLOWLOCATION, 1L );
    curl_easy_setopt( curl, CURLOPT_SSL_VERIFYHOST, 0 );
    curl_easy_setopt( curl, CURLOPT_SSL_VERIFYPEER, 0 );
    curl_easy_setopt( curl, CURLOPT_TIMEOUT, time_out );
    curl_easy_setopt( curl, CURLOPT_CONNECTTIMEOUT, time_out_connect );
    curl_easy_setopt( curl, CURLOPT_USERAGENT, "Chrome/107.0.0.0" );
    curl_easy_setopt( curl, CURLOPT_HTTPHEADER, api_session_ptr->get_headers() );
    curl_easy_setopt( curl, CURLOPT_POSTFIELDS, api_session_ptr->get_param().c_str() );

    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, _recv_cb );
    curl_easy_setopt( curl, CURLOPT_WRITEDATA, reinterpret_cast<api_session*>( api_session_ptr.get() ) );

    curl_share_setopt( curl, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS );

    CURLcode res = CURLE_OK;
    try
    {
        res = curl_easy_perform( curl );
    }
    catch ( ... )
    {
        res = CURL_LAST;
    }

    if ( res != CURLE_OK )
    {
        if ( curl != nullptr )
        {
            curl_easy_cleanup( curl );
            curl = nullptr;
        }

        api_session_ptr->stop();

        return false;
    }

    api_session_ptr->success();

    if ( curl != nullptr )
    {
        curl_easy_cleanup( curl );
        curl = nullptr;
    }

    api_session_ptr->stop();

    return true;
}

std::size_t api_mgr::_recv_cb( void* content, std::size_t size, std::size_t nmemb, void* userp )
{
    api_session* ps = reinterpret_cast<api_session*>( userp );

    std::size_t len = size * nmemb;

    ps->add_content( content, len );

    return len;
}
