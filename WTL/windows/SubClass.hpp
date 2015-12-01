//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\SubClass.hpp
//! \brief Represents a subclassed window
//! \date 17 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_SUBCLASS_HPP
#define WTL_SUBCLASS_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/List.hpp>                                     //!< List
#include <wtl/platform/WindowMessage.hpp>                         //!< WindowMesssage

//! \namespace wtl - Windows template library
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct SubClass - Represents an element in the chain of subclassed windows.
  //!
  //! \remarks Encapsulates the window procedure of either a system or WTL window
  /////////////////////////////////////////////////////////////////////////////////////////
  struct SubClass
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
      
    //! \alias wndproc_t - Class window procedure signature
    using wndproc_t = LRESULT (__stdcall*)(::HWND, uint32_t, ::WPARAM, ::LPARAM);

    //! \alias wtlproc_t - Instance window procedure signature
    using wtlproc_t = LResult (__thiscall*)(WindowMessage, ::WPARAM, ::LPARAM);  

    //! \union WindowProc - Window procedure
    union WindowProc
    {
      WindowProc(wndproc_t n) : Native(n)  {}
      WindowProc(wtlproc_t l) : Library(l) {}

      wndproc_t  Native;     //!< Win32
      wtlproc_t  Library;    //!< WTL
    };
      
    //! \enum WindowType - Define window types
    enum class WindowType
    {
      Library,   //!< Wtl window 
      Native,    //!< Native window
    };

    // ----------------------------------- REPRESENTATION -----------------------------------

    WindowProc  WndProc;    //!< Window procedure
    WindowType  Type;       //!< Window type

    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // SubClass::SubClass
    //! Create SubClass from a WTL or native window
    //! 
    //! \param[in] t - Window type (Whether WTL or native)
    //! \param[in] p - Window procedure
    /////////////////////////////////////////////////////////////////////////////////////////
    SubClass(WindowType t, WindowProc p) : WndProc(p), Type(t)
    {}
      
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };
    
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias SubClassCollection - Define subclassed windows collection
  /////////////////////////////////////////////////////////////////////////////////////////
  using SubClassCollection = List<SubClass>;
    
  
} // namespace wtl


#endif // WTL_SUBCLASS_HPP
