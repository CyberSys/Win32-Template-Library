//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\WinAPI.h
//! \brief Provides wide and narrow character implementations of common Windows API functions
//! \date 25 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINAPI_HPP
#define WTL_WINAPI_HPP

#include "WTL.hpp"
#include "wtl/traits/EncodingTraits.hpp"        //!< Encoding

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct WinAPI - Provides wide and narrow character implementations of common Windows API functions
  //!
  //! \tparam ENC - Character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct WinAPI
  {    
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \var encoding - Character encoding
    static constexpr Encoding encoding = ENC;

    //! Functions 'C'
    static constexpr auto callWindowProc = choose<encoding>(::CallWindowProcA,::CallWindowProcW);
    static constexpr auto createFont = choose<encoding>(::CreateFontA,::CreateFontW);
    static constexpr auto createWindowEx = choose<encoding>(::CreateWindowExA,::CreateWindowExW);
    
    //! Functions 'D'
    static constexpr auto defWindowProc = choose<encoding>(::DefWindowProcA,::DefWindowProcW);
    static constexpr auto dispatchMessage = choose<encoding>(::DispatchMessageA,::DispatchMessageW);
    static constexpr auto drawText = choose<encoding>(::DrawTextA,::DrawTextW);
    
    //! Functions 'F'
    static constexpr auto findFirstFile = choose<encoding>(::FindFirstFileA,::FindFirstFileW);
    static constexpr auto findNextFile = choose<encoding>(::FindNextFileA,::FindNextFileW);
    static constexpr auto findResourceEx = choose<encoding>(::FindResourceExA,::FindResourceExW);
    static constexpr auto formatMessage = choose<encoding>(::FormatMessageA,::FormatMessageW);
    
    //! Functions 'G'
    static constexpr auto getClassInfoEx = choose<encoding>(::GetClassInfoExA,::GetClassInfoExW);
    static constexpr auto getDateFormat = choose<encoding>(::GetDateFormatA,::GetDateFormatW);
    static constexpr auto getFileAttributes = choose<encoding>(::GetFileAttributesA,::GetFileAttributesW);
    static constexpr auto getMessage = choose<encoding>(::GetMessageA,::GetMessageW);
    static constexpr auto getModuleFileName = choose<encoding>(::GetModuleFileNameA,::GetModuleFileNameW);
    static constexpr auto getTempPath = choose<encoding>(::GetTempPathA,::GetTempPathW);
    static constexpr auto getTempFileName = choose<encoding>(::GetTempFileNameA,::GetTempFileNameW);
    static constexpr auto getTextExtentPoint32 = choose<encoding>(::GetTextExtentPoint32A,::GetTextExtentPoint32W);
    static constexpr auto getVersion = choose<encoding>(::GetVersionExA,::GetVersionExW);
    static constexpr auto getWindowLongPtr = choose<encoding>(::GetWindowLongPtrA,::GetWindowLongPtrW);

    //! Functions 'I'
    static constexpr auto insertMenuItem = choose<encoding>(::InsertMenuItemA,::InsertMenuItemW);
    static constexpr auto isDialogMessage = choose<encoding>(::IsDialogMessageA,::IsDialogMessageW);
    
    //! Functions 'L'
    static constexpr auto loadAccelerators = choose<encoding>(::LoadAcceleratorsA,::LoadAcceleratorsW);
    static constexpr auto loadCursor = choose<encoding>(::LoadCursorA,::LoadCursorW);
    static constexpr auto loadIcon = choose<encoding>(::LoadIconA,::LoadIconW);
    static constexpr auto loadMenu = choose<encoding>(::LoadMenuA,::LoadMenuW);
    static constexpr auto loadLibrary = choose<encoding>(::LoadLibraryA,::LoadLibraryW);
    
    //! Functions 'M'
    static constexpr auto messageBox = choose<encoding>(::MessageBoxA,::MessageBoxW);

    //! Functions 'P'
    static constexpr auto pathAddBackslash = choose<encoding>(::PathAddBackslashA,::PathAddBackslashW);
    static constexpr auto pathAppend = choose<encoding>(::PathAppendA,::PathAppendW);
    static constexpr auto pathCombine = choose<encoding>(::PathCombineA,::PathCombineW);
    static constexpr auto pathFileExists = choose<encoding>(::PathFileExistsA,::PathFileExistsW);
    static constexpr auto pathFileExtension = choose<encoding>(::PathFindExtensionA,::PathFindExtensionW);
    static constexpr auto pathFindFileName = choose<encoding>(::PathFindFileNameA,::PathFindFileNameW);
    static constexpr auto pathRemoveBackslash = choose<encoding>(::PathRemoveBackslashA,::PathRemoveBackslashW);
    static constexpr auto pathRemoveExtension = choose<encoding>(::PathRemoveExtensionA,::PathRemoveExtensionW);
    static constexpr auto pathRenameExtension = choose<encoding>(::PathRenameExtensionA,::PathRenameExtensionW);
    static constexpr auto pathRemoveFileSpec = choose<encoding>(::PathRemoveFileSpecA,::PathRemoveFileSpecW);
    static constexpr auto postMessage = choose<encoding>(::PostMessageA,::PostMessageW);

    //! Functions 'R'
    static constexpr auto registerClassEx = choose<encoding>(::RegisterClassExA,::RegisterClassExW);

    //! Functions 'S'
    static constexpr auto sendMessage = choose<encoding>(::SendMessageA,::SendMessageW);
    static constexpr auto strCmpI = choose<encoding>(::StrCmpIA,::StrCmpIW);

    //! Functions 'T'
    static constexpr auto translateAccelerator = choose<encoding>(::TranslateAcceleratorA,::TranslateAcceleratorW);

    //! Functions 'U'
    static constexpr auto unregisterClass = choose<encoding>(::UnregisterClassA,::UnregisterClassW);
    
  };
}

#endif // WTL_WINAPI_HPP


