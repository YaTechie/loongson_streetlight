#ifndef stdafx_h__
#define stdafx_h__

// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#ifdef WIN32

#include "targetver.h"

#endif

#include <stdio.h>

// TODO: 在此处引用程序需要的其他头文件
#include <openssl/des.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/opensslv.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>

#include <atomic>
#include <bitset>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <regex>
#include <set>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>

#define FMT_STRING_ALIAS 1

#include <curl/curl.h>
#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/printf.h>
#include <json/json.h>

#include <asio2/asio2.hpp>

#include "global.h"

#endif // stdafx_h__
