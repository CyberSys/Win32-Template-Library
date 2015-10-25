//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\WTL.hpp
//! \brief Main header for windows template library
//! \date 7 March 2015
//! \author Nick Crowley
//! \copyright © Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_HPP
#define WTL_HPP

// --------------------------------------------------------------------------------------------------------
// ---------------------------------------------- PLATFORM ------------------------------------------------
// --------------------------------------------------------------------------------------------------------

//! \def STRSAFE_NO_DEPRECATE - Disable the StringSafe library depreciation warnings
#define STRSAFE_NO_DEPRECATE

//! \def _SCL_SECURE_NO_WARNINGS - Disable visual C++ checked iterator warnings, provided by wtl
#define _SCL_SECURE_NO_WARNINGS

//! \def _CRT_SECURE_NO_WARNINGS - Disable C runtime library warnings
#define _CRT_SECURE_NO_WARNINGS

//! \def WIN32_LEAN_AND_MEAN - Exclude rarely used stuff from windows headers
#ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN
#endif

//! \def NOMINMAX - Disable preprocessor macros: min, max
#ifndef NOMINMAX
  #define NOMINMAX
#endif

//! \def _GLIBCXX_USE_INT128 - Remove int128 from clang
#undef _GLIBCXX_USE_INT128

// Target Platform v5.01 (WindowsXP)
//#include <WinSDKVer.h>
#define _WIN32_WINNT    _WIN32_WINNT_WINXP
//#include <SDKDDKVer.h>

#include <windows.h>        // Main windows header
#include <commctrl.h>       // Common controls library
//#include <windowsx.h>
#include <tchar.h>          // Defines narrow/wide char Win32 entry points
//#include <strsafe.h>        // StringSafe library - Secure string handling
#include <shlwapi.h>        // Shell light-weight API - Path handling
//#include <Shellapi.h>     // Shell API - FileSystem-Shell COM interop  (Disabled)

// --------------------------------------------------------------------------------------------------------
// ----------------------------------------------- LIBRARY ------------------------------------------------
// --------------------------------------------------------------------------------------------------------

// Global Macros
#include "Macros.h"

// Types & Constants
#include "Types.h"
#include "Constants.h"

#endif // WTL_HPP

