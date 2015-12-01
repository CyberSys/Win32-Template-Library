//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\MessageWindow.hpp
//! \brief Message window class
//! \date 19 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MESSAGE_WINDOW_HPP
#define WTL_MESSAGE_WINDOW_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/Window.hpp>

//! \namespace wtl - Windows template library
namespace wtl 
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct MessageWindow - Provides a message-only window
  //! 
  //! \tparam ENC - Window charactrer encoding (UTF16 if unspecified)
  ///////////////////////////////////////////////////////////////////////////////
  template <wtl::Encoding ENC = wtl::Encoding::UTF16>
  struct MessageWindow : wtl::Window<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias base - Define base type
    using base = wtl::Window<ENC>;

    //! \var encoding - Inherit window character encoding
    static constexpr wtl::Encoding  encoding = base::encoding;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------- CONSTRUCTION -----------------------------------

    ///////////////////////////////////////////////////////////////////////////////
    // MessageWindow::MessageWindow
    //! Create a message-only window
    ///////////////////////////////////////////////////////////////////////////////
    MessageWindow() 
    {}
  
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
    // ----------------------------------- STATIC METHODS -----------------------------------
  
    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
  
    ///////////////////////////////////////////////////////////////////////////////
    // MessageWindow::getClass 
    //! Get the window class
    //!
    //! \param[in] instance - Module owning the window class
    //! \return class_t& - Reference to window class 
    ///////////////////////////////////////////////////////////////////////////////
    const WindowClass<encoding>& wndclass() const override
    {
      static WindowClass<encoding>  std(SystemClass::MessageOnly);  //!< Message-only system class

      // Return singleton
      return std;
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };

  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct NativeWindow - Provides a (non-owning) wrapper for a native window handle
  //! 
  //! \tparam ENC - Window charactrer encoding (UTF16 if unspecified)
  ///////////////////////////////////////////////////////////////////////////////
  template <wtl::Encoding ENC = wtl::Encoding::UTF16>
  struct NativeWindow : wtl::Window<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias base - Define base type
    using base = wtl::Window<ENC>;

    //! \alias class_t - Inherit window class type
    using class_t = typename base::class_t;
    
    //! \alias resource_t - Inherit identifier type
    using resource_t = typename base::resource_t;
    
    //! \var encoding - Inherit window character encoding
    static constexpr wtl::Encoding  encoding = base::encoding;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  private:
    class_t   NativeClass;     //!< Provides storage for the window class

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // NativeWindow::NativeWindow
    //! Create a native window wrapper
    //! 
    //! \param[in] wnd - Native window handle
    //!
    //! \throw wtl::invalid_argument - Missing window handle
    ///////////////////////////////////////////////////////////////////////////////
    NativeWindow(::HWND wnd) : base(wnd, NativeClass),
                               NativeClass(getClassAtom(wnd))
    {}
  
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
		DISABLE_COPY(NativeWindow);			//!< Move-only type
		ENABLE_MOVE(NativeWindow);			//!< Can be moved
		
    // ----------------------------------- STATIC METHODS -----------------------------------
  private:
    ///////////////////////////////////////////////////////////////////////////////
    // MessageWindow::getClass 
    //! Get the window class
    //!
    //! \param[in] instance - Module owning the window class
    //! \return class_t& - Reference to window class 
    ///////////////////////////////////////////////////////////////////////////////
    class_t& getClass(::HINSTANCE instance) override
    {
      static WindowClass<encoding>  std(SystemClass::MessageOnly);  //!< Message-only system class

      // Return singleton
      return std;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // NativeWindow::getClassAtom
    //! Get the window class atom from a native handle
    //! 
    //! \param[in] wnd - Native window handle
    //! \return resource_t - Resource identifier
    //! 
    //! \throw wtl::invalid_argument - Missing window handle
    ///////////////////////////////////////////////////////////////////////////////
    resource_t getClassAtom(::HWND wnd) 
    {
      REQUIRED_PARAM(wnd);

      // Query class atom
      return { static_cast<uint16_t>(::GetClassLongPtr(wnd, GCW_ATOM)) };
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  
  };

} // namespace wtl


#endif // WTL_MESSAGE_WINDOW_HPP
