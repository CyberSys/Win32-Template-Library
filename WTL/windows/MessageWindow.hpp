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
#include <wtl/windows/Window.hpp>               //!< Window

//! \namespace wtl - Windows template library
namespace wtl 
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct MessageWindow - Provides a message-only window
  //! 
  //! \tparam ENC - Window charactrer encoding (UTF16 if unspecified)
  ///////////////////////////////////////////////////////////////////////////////
  template <wtl::Encoding ENC = wtl::Encoding::UTF16>
  struct MessageWindow : Window<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = MessageWindow<ENC>;

    //! \alias base - Define base type
    using base = Window<ENC>;

    //! \var encoding - Inherit window character encoding
    static constexpr Encoding  encoding = base::encoding;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------- CONSTRUCTION -----------------------------------

    ///////////////////////////////////////////////////////////////////////////////
    // MessageWindow::MessageWindow
    //! Create a message-only window
    ///////////////////////////////////////////////////////////////////////////////
    MessageWindow() 
    {
      // Compile-time subclass the window
      this->SubClasses.push(getNativeSubClass());
    }
  
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
    // ----------------------------------- STATIC METHODS -----------------------------------
  private:
    /////////////////////////////////////////////////////////////////////////////////////////
    // MessageWindow::getNativeSubClass 
    //! Get the window procedure for the standard Message-only window
    //! 
    //! \return SubClass<encoding> - SubClass representing the window procedure of a standard Message-only window
    /////////////////////////////////////////////////////////////////////////////////////////
    static SubClass<encoding> getNativeSubClass() 
    {
      static const WindowClass<encoding>  std(SystemClass::MessageOnly);    //!< Lookup message-only button window-class
      
      // Return native window proc
      return { std.WndProc };
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // MessageWindow::registerClass 
    //! Registers the window-class 
    //! 
    //! \param[in] instance - Handle to registering module  [Used only during initial call]
    //! \return const WindowClass<encoding>& - Window class 
    //! 
    //! \throw wtl::platform_error - Unable to register window class
    /////////////////////////////////////////////////////////////////////////////////////////
    static const WindowClass<encoding>&  registerClass(::HINSTANCE instance) 
    {
      static const String<encoding> name("WTL.MessageOnly");
      
      // Define WTL message-only window-class
      static const WindowClass<encoding>  std(SystemClass::MessageOnly);  //!< Message-only system class
      static const WindowClass<encoding>  msg(instance,
                                              name.c_str(),
                                              std.Style,
                                              base::WndProc,              //!< Replace the window procedure 'Compile-time subclass'
                                              std.Menu,
                                              std.Cursor,
                                              std.Background,
                                              std.SmallIcon,
                                              std.LargeIcon,
                                              std.ClassStorage,
                                              std.WindowStorage);    
      // Return singleton
      return msg;
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // MessageWindow::create
    //! Creates the window 
    //!
    //! \param[in,out] *owner - [optional] Parent/owner window   (Must also be a message-only window)
    //! 
    //! \throw wtl::logic_error - Window already exists
    //! \throw wtl::platform_error - Unable to create window
    //! 
    //! \remarks The window handle is initialized twice during the construction process. When the ::CreateWindow() call sends a
    //! \remarks WM_CREATE message, the wndproc saves a weak-ref in this->Handle that is later overwritten by the strong-ref returned
    //! \remarks from the ::CreateWindow() call.
    /////////////////////////////////////////////////////////////////////////////////////////
    void create(Window<ENC>* owner = nullptr) override
    {
      // Ensure window does not exist
      if (this->exists())
        throw logic_error(HERE, "Window already exists");

      // Provide message-only sentinel unless owner window
      ::HWND parent = owner ? (::HWND)owner->handle() : HWND_MESSAGE;          //!< Use parent if any

      // Create as message only window
      this->Handle = HWnd(wndclass(), this, parent, this->Style, this->StyleEx, defvalue<::HMENU>(), this->Text(), PointL{}, SizeL{});
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // MessageWindow::wndclass const
    //! Get the window class
    //! 
    //! \return const WindowClass<encoding>& - Shared window class
    /////////////////////////////////////////////////////////////////////////////////////////
    const WindowClass<encoding>& wndclass() const override
    {
      return registerClass(nullptr);
    }
    
  };
  
  
} // namespace wtl


#endif // WTL_MESSAGE_WINDOW_HPP
