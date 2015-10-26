//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\Window.hpp
//! \brief Main window class
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_HPP
#define WTL_WINDOW_HPP

#include "wtl/WTL.hpp"
#include "wtl/windows/WindowBase.hpp"

//! \namespace wtl - Windows template library
namespace wtl 
{
  //! \struct Window - Window class
  template <Encoding ENC>
  struct Window : WindowBase<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = Window<ENC>;
  
    //! \alias base - Define base type
    using base = WindowBase<ENC>;

    //! \alias wndclass_t - Define window class type
    using wndclass_t = typename base::wndclass_t;
    
    //! \alias encoding - Inherit window character encoding 
    static constexpr Encoding encoding = base::encoding;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------- CONSTRUCTION -----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::Window
    //! Creates the window object (but not window handle) for an instance of a registered window class
    //! 
    //! \param[in] &cls - Registered window class 
    /////////////////////////////////////////////////////////////////////////////////////////
    Window(wndclass_t& cls) : base(cls)
    {
    }

    // -------------------------------- COPY, MOVE & DESTROY  -------------------------------
    
    DISABLE_COPY(Window);     //!< Cannot be copied
    ENABLE_MOVE(Window);      //!< Can be moved
    ENABLE_POLY(Window);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------
  
    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
  
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  
    
  
  };

  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct MessageWindow - Provides a message-only window
  //! 
  //! \tparam ENC - Window charactrer encoding (UTF16 if unspecified)
  ///////////////////////////////////////////////////////////////////////////////
  template <wtl::Encoding ENC = wtl::Encoding::UTF16>
  struct MessageWindow : wtl::WindowBase<ENC>
  {
    // ------------------------ TYPES --------------------------
  
    //! \alias base - Define base type
    using base = wtl::WindowBase<ENC>;

    //! \alias wndclass_t - Inherit window class type
    using wndclass_t = typename base::wndclass_t;

    //! \var encoding - Inherit window character encoding
    static constexpr wtl::Encoding  encoding = base::encoding;

    // -------------------- REPRESENTATION ---------------------
  
    // --------------------- CONSTRUCTION ----------------------
  
    ///////////////////////////////////////////////////////////////////////////////
    // MessageWindow::MessageWindow
    //! Create a message-only window
    ///////////////////////////////////////////////////////////////////////////////
    MessageWindow() : base(getClass())
    {}
  
    // ------------------------ STATIC -------------------------
  
    ///////////////////////////////////////////////////////////////////////////////
    // MessageWindow::getClass 
    //! Get the window class
    //! 
    //! \return wndclass_t& - Window class 
    ///////////////////////////////////////////////////////////////////////////////
    static wndclass_t& getClass() 
    {
      static WindowClass<encoding>  std(SystemClass::MessageOnly);  //!< Message-only system class

      // Return singleton
      return std;
    }
  };

} // namespace wtl


#endif // WTL_WINDOW_HPP
