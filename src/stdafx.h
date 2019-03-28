// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <ostream>
#include <fstream>
#include <typeinfo>
#include <functional>
#include <algorithm>
#include <memory>
#include <chrono>
#include <mutex> 
#include <array>
#include <map>

// using namespace std::string_literals;
using namespace std::placeholders; 

#include <csignal>


#include <inttypes.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#ifdef _MSC_VER
    #define _CRT_SECURE_NO_WARNINGS

    #include "targetver.h"

    #include <tchar.h>
    #include <direct.h>

    #include <crtdefs.h>

    #include <windows.h>

#endif


#define FMT_HEADER_ONLY 
#include "cppformat/format.h"

