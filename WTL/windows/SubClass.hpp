//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\SubClass.hpp
//! \brief Represents a subclassed window
//! \date 24 February 2016
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_SUBCLASS_HPP
#define WTL_SUBCLASS_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/Stack.hpp>                                    //!< Stack
#include <wtl/platform/WindowMessage.hpp>                         //!< WindowMesssage

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct SubClass - Encapsulates the window procedure of a native or library window
  //! 
  //! \tparam ENC - Character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct SubClass
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
      
    //! \alias type - Define own type
    using type = SubClass<ENC>;
  
    //! \alias wndproc_t - Class window procedure signature
    using wndproc_t = LRESULT (__stdcall*)(::HWND, uint32_t, ::WPARAM, ::LPARAM);

    //! \alias wtlproc_t - Window routing method signature
    using wtlproc_t = LResult (Window<ENC>::*)(WindowMessage, ::WPARAM, ::LPARAM);  
    
    //! \var encoding - Inherit character encoding
    static constexpr Encoding  encoding = ENC;
    
  private:
    //! \enum WindowType - Define window types
    enum class WindowType
    {
      Library,   //!< Wtl window 
      Native,    //!< Native window
    };

    // ----------------------------------- REPRESENTATION -----------------------------------
  private:
    //! \union - Window procedure
    union 
    {
      wtlproc_t  Library;    //!< WTL window procedure
      wndproc_t  Native;     //!< Win32 window procedure
    };    
    
    WindowType  Type;        //!< Window procedure type

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // SubClass::SubClass
    //! Create SubClass from native window procedure
    //! 
    //! \param[in] fn - Window procedure
    /////////////////////////////////////////////////////////////////////////////////////////
    SubClass(wndproc_t fn) : Native(fn), Type(WindowType::Native)
    {}
      
    /////////////////////////////////////////////////////////////////////////////////////////
    // SubClass::SubClass
    //! Create SubClass from routing method of WTL window
    //! 
    //! \param[in] fn - Routing method of WTL window
    /////////////////////////////////////////////////////////////////////////////////////////
    SubClass(wtlproc_t fn) : Library(fn), Type(WindowType::Library)
    {}
      
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // SubClass::route const
    //! Route message to the subclassed window procedure
    //!
    //! \param[in] message - Window message identifier
    //! \param[in] w - [optional] First message parameter
    //! \param[in] l - [optional] Second message parameter
    //! \return LResult - Message routing and result 
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult route(Window<encoding>& wnd, WindowMessage message, ::WPARAM w, ::LPARAM l) const
    {
      // [LIBRARY] Delegate to instance procedure 
      if (Type == WindowType::Library)
        return (wnd.*Library)(message, w, l);

      // [NATIVE] Delegate to native window procedure 
      return WinAPI<encoding>::callWindowProc(Native, wnd.handle(), enum_cast(message), w, l);
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };
    
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias SubClassCollection - Define stack-based collection of sub-classed window procedures
  //! 
  //! \tparam ENC - Character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using SubClassCollection = Stack<SubClass<ENC>>;
    
  
} // namespace wtl


#endif // WTL_SUBCLASS_HPP
