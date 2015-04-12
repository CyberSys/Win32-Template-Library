////////////////////////////////////////////////////////////////////////////////
//! \file wtl\WTL.hpp
//! \brief Main header for windows template library
//! \date 7 March 2015
//! \author Nick Crowley
//! \copyright � Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_HEADER_HPP
#define WTL_HEADER_HPP

// --------------------------------------------------
// --------------- HEADER MANAGEMENT ----------------
// --------------------------------------------------

#include "Macros.h"

// --------------------------------------------------
// ----------- STANDARD TEMPLATE LIBRARY ------------
// --------------------------------------------------

#include <stdexcept>
#include <cassert>
#include <cstdio>
#include <algorithm>
#include <functional>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <map>
#include <deque>
#include <list>
#include <forward_list>
#include <set>
#include <vector>
#include <string>
#include <tuple>
#include <regex>

// --------------------------------------------------
// -------------------- PLATFORM --------------------
// --------------------------------------------------

#ifdef WIN32
  //! \def WIN32_LEAN_AND_MEAN - Exclude rarely used stuff from windows headers
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif

  //! \def NOMINMAX - Disable preprocessor macros: min, max
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif

  // Target Platform v5.01 (WindowsXP)
  #include <WinSDKVer.h>
  #define _WIN32_WINNT    _WIN32_WINNT_WINXP
  #include <SDKDDKVer.h>

  #include <windows.h>        // Main windows header
  #include <commctrl.h>       // Common controls library
  //#include <windowsx.h> 
  #include <tchar.h>          // Defines narrow/wide char Win32 entry points 
  //#include <strsafe.h>        // StringSafe library - Secure string handling
  #include <shlwapi.h>        // Shell light-weight API - Path handling
  //#include <Shellapi.h>     // Shell API - FileSystem-Shell COM interop  (Disabled)
#endif

// --------------------------------------------------
// ---------- WINDOWS TEMPLATE LIBRARY --------------
// --------------------------------------------------

// Types
#include "Types.h"

// Function object adapters + static allocator + utilities
#include "utils/Adapters.hpp" 
#include "utils/Policies.hpp"
#include "utils/Helpers.hpp"

// Enumerations + Encoding
#include "traits/EnumTraits.hpp"
#include "traits/EncodingTraits.hpp"

// Casts: 
#include "casts/StaticPtrCast.hpp"
#include "casts/OpaqueCast.hpp"
#include "casts/EnumCast.hpp"
#include "casts/BooleanCast.hpp"

// Utilities: 
#include "utils/FormatSpec.hpp"
#include "utils/Zero.hpp"
#include "utils/Exception.hpp"
#include "utils/Range.hpp"
#include "utils/DynamicArray.hpp"
#include "utils/FixedArray.hpp"
#include "utils/CharArray.hpp"
#include "utils/Lazy.hpp"
#include "utils/Bitset.hpp"
#include "utils/Handle.hpp"
#include "utils/Random.hpp"

// Platform Constants:
#include "platform/Colours.hpp"
#include "platform/DrawingFlags.hpp"
#include "platform/WindowFlags.hpp"
#include "platform/ControlStyles.hpp"

// Platform Utilities:
#include "platform/Path.hpp"
#include "platform/FileSearch.hpp"
#include "platform/Shapes.hpp"

// Platform Traits:
#include "traits/LocaleTraits.hpp"
#include "traits/ResourceTraits.hpp"
#include "traits/WindowClassTraits.hpp"
#include "traits/WindowTraits.hpp"
#include "traits/GlobalTraits.hpp"
#include "traits/PenTraits.hpp"
#include "traits/BrushTraits.hpp"
#include "traits/CursorTraits.hpp"
#include "traits/MenuTraits.hpp"
#include "traits/IconTraits.hpp"
#include "traits/FontTraits.hpp"
#include "traits/AcceleratorTraits.hpp"
#include "traits/WindowMessageTraits.hpp"
#include "traits/DeviceContextTraits.hpp"

// IO: Streams
#include "io/MemoryStream.hpp"
//#include "io/FileStream.hpp"
#include "io/LocalFileStream.hpp"

// Console/Widget
#include "windows/MessageBox.hpp"
#include "utils/Console.hpp"
//#include "utils/Widget.hpp"

// Constants
//#include "Constants.hpp"

// IO: Readers,Writers
#include "io/TextReader.hpp"
#include "io/TextWriter.hpp"
#include "io/BinaryReader.hpp"
#include "io/BinaryWriter.hpp"
#include "io/XmlReader.hpp"
#include "io/XmlWriter.hpp"
#include "io/RichTextWriter.hpp"

// Resources
#include "resources/Resources.hpp"
#include "resources/StringResource.hpp"
#include "resources/Menu.hpp"

// GDI
#include "gdi/StockObjects.hpp"
#include "gdi/DeviceContext.hpp"

// Events
#include "windows/EventArgs.hpp"
#include "windows/EventHandler.hpp"
#include "windows/EventDelegate.hpp"
#include "events/CreateWindowEvent.hpp"
#include "events/DestroyWindowEvent.hpp"
#include "events/PaintWindowEvent.hpp"
#include "events/ShowWindowEvent.hpp"

// Windows
#include "windows/WindowClass.hpp"
#include "windows/WindowBase.hpp"

// Events
#include "events/CommandEvent.hpp"
#include "events/NotifyEventData.hpp"
#include "events/NotifyEvent.hpp"
#include "events/ControlEvents.hpp"

// Threads
#include "threads/WorkerThread.hpp"
#include "threads/MessageThread.hpp"

// Controls
#include "windows/Window.hpp"
#include "controls/Button.hpp"

// Windows & Dialogs

// Convenience
#include "windows/MessageHandler.hpp"

#endif // WTL_HEADER_HPP
