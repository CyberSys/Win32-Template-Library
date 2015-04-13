////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\WindowBase.hpp
//! \brief Base class for windows
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_BASE_HPP
#define WTL_WINDOW_BASE_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //! \struct WindowBase - Base window class
  template <Encoding ENC>
  struct WindowBase 
  {
    struct SubClass;

    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias char_t - Define window character type
    using char_t = encoding_char_t<ENC>;

    //! \alias resource_t - Resource identifier type
    using resource_t = ResourceId<ENC>;
    
    //! \alias CreateStruct - Define WM_CREATE/WM_NCCREATE creation data
    using CreateStruct = getType<char_t,::CREATESTRUCTA,::CREATESTRUCTW>;

    //! \var encoding - Define window character encoding
    static constexpr Encoding encoding = ENC;

    //! \alias HandlerCollection - Define event handler collection type
    using HandlerCollection = EventHanderCollection<ENC>;

    //! \alias WindowCollection - Window collection type
    using WindowCollection = std::list<WindowBase*>;
    
    //! \alias WindowHandleCollection - Provides an association between window handles and WindowBase objects
    using WindowHandleCollection = std::map<HWND,WindowBase*>;

    //! \alias WindowIdCollection - Provides an association between window Ids and WindowBase objects
    using WindowIdCollection = std::map<WindowId,WindowBase*>;
    
    //! \alias ActiveWindowCollection - Define 'Active Windows' collection type
    using ActiveWindowCollection = WindowHandleCollection;

    //! \alias ChildWindowCollection - Define child window collection type
    using ChildWindowCollection = WindowIdCollection;
    
    //! \alias SubClassCollection - Define subclassed windows collection
    using SubClassCollection = std::list<SubClass>;

    //! \alias wndclass_t - Window class type
    using wndclass_t = WindowClass<encoding>;

    //! \alias wndproc_t - Win32 Window procedure type
    using wndproc_t = LRESULT (__stdcall*)(HWND, uint32, WPARAM, LPARAM);

    //! \alias wtlproc_t - WTL Window procedure type
    using wtlproc_t = LResult (__thiscall*)(WindowMessage, WPARAM, LPARAM);  // = decltype(onMessage);

    //! \alias window_t - Define own type
    using window_t = WindowBase;

    //! \enum WindowType - Define window types
    enum class WindowType
    {
      Library,   //!< Wtl window 
      Native,    //!< Native window
    };

    //! \struct SubClass - Represents a subclassed window
    struct SubClass
    {
      //! \union WindowProc - Window procedure
      union WindowProc
      {
        WindowProc(wndproc_t n) : Native(n)  {}
        WindowProc(wtlproc_t l) : Library(l) {}

        wndproc_t  Native;     //!< Win32
        wtlproc_t  Library;    //!< WTL
      };

      ///////////////////////////////////////////////////////////////////////////////
      // SubClass::SubClass
      //! Create SubClass from a WTL or native window
      //! 
      //! \param[in] t - Window type (Whether WTL or native)
      //! \param[in] p - Window procedure
      ///////////////////////////////////////////////////////////////////////////////
      SubClass(WindowType t, WindowProc p) : WndProc(p), Type(t)
      {}

      WindowProc  WndProc;    //!< Window procedure
      WindowType  Type;       //!< Window type
    };

    //! \var ActiveWindows - Static collection of all Windows Template Library windows that currently exist for the current process
    static WindowHandleCollection  ActiveWindows;
    
    // --------------------- CONSTRUCTION ----------------------
  protected:
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::WindowBase
    //! Creates the window object (but not window handle) for an instance of a registered window class
    //! 
    //! \param[in] &cls - Registered window class 
    ///////////////////////////////////////////////////////////////////////////////
    WindowBase(wndclass_t& cls) : Class(cls)
    {
      // Ensure we always have a WM_PAINT handler
      *this += new PaintWindowEventHandler<encoding>( this, &WindowBase::onPaint );
    }
    
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::~WindowBase
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~WindowBase()
    {
    }

    // ------------------------ STATIC -------------------------
  protected:
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::isUnhandled
    //! Query whether a result is the 'Unhandled' return code
    //!
    //! \param[in] message - Window message identifier
    //! \param[in] res - Message result 
    //! \return bool - True iff result indicates message was unhandled
    ///////////////////////////////////////////////////////////////////////////////
    static bool  isUnhandled(WindowMessage msg, LRESULT res)
    {
      switch (msg)
      {
      case WindowMessage::CREATE:         return res == unhandled_result<WindowMessage::CREATE>::value;
      case WindowMessage::DESTROY:        return res == unhandled_result<WindowMessage::DESTROY>::value;
      case WindowMessage::SHOWWINDOW:     return res == unhandled_result<WindowMessage::SHOWWINDOW>::value;
      case WindowMessage::SIZE:           return res == unhandled_result<WindowMessage::SIZE>::value;
      case WindowMessage::SETFOCUS:       return res == unhandled_result<WindowMessage::SETFOCUS>::value;
      case WindowMessage::KILLFOCUS:      return res == unhandled_result<WindowMessage::KILLFOCUS>::value;
      case WindowMessage::GETMINMAXINFO:  return res == unhandled_result<WindowMessage::GETMINMAXINFO>::value;
      default:                            return res != 0;
      }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::onMessage
    //! Instance window procedure 
    //!
    //! \param[in] message - Window message identifier
    //! \param[in] wParam - [optional] First message parameter
    //! \param[in] lParam - [optional] Second message parameter
    //! \return LResult - Message routing and result 
    ///////////////////////////////////////////////////////////////////////////////
    LResult onMessage(WindowMessage message, WPARAM wParam, LPARAM lParam)
    {
      try
      {
        LResult ret;       //!< Message result, defaults to unhandled

        // [INSTANCE] Offer message to the each of the instance's event handlers
        for (auto& ev : Handlers)
          // [ACCEPT/INVOKE] Pass to handle if it accepts the message 
          if (ev->accept(*Handle, message, wParam, lParam))
            if ((ret = ev->invoke(*Handle, message, wParam, lParam)).Route == MsgRoute::Handled)
              // [HANDLED] Return message result & routing
              return ret;

        // [SUB-CLASS] Offer message to each subclass in turn (if any)
        for (auto& wnd : SubClasses)
          switch (wnd.Type)
          {
          // [WTL WINDOW] Delegate to window object 
          case WindowType::Library:
            // Delegate to instance window procedure
            ret = wnd.WndProc.Library(message, wParam, lParam);

            // [HANDLED/REFLECTED] Return result & routing
            if (ret.Route == MsgRoute::Handled || ret.Route == MsgRoute::Reflected)
              return ret;
            break;

          // [NATIVE WINDOW] Call window procedure via Win32 API and determine routing from result
          case WindowType::Native:
            // Delegate to native class window procedure and infer routing
            ret.Result = getFunc<char_t>(::CallWindowProcA,::CallWindowProcW)(wnd.WndProc.Native, *Handle, enum_cast(message), wParam, lParam);
            ret.Route = (isUnhandled(message, ret.Result) ? MsgRoute::Unhandled : MsgRoute::Handled);
          
            // [HANDLED] Return result & routing
            if (ret.Route == MsgRoute::Handled)
              return ret;
          }

        // [UNHANDLED] Return result & routing
        return ret;
      }
      catch (wtl::exception& e)
      {
        cdebug.log(HERE, e);
        
        // [ERROR] Unhandled
        return MsgRoute::Unhandled;
      }
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::WndProc
    //! Class window procedure 
    //!
    //! \param[in] hWnd - Window handle
    //! \param[in] message - Message ident
    //! \param[in] wParam - [optional] Parameter1
    //! \param[in] lParam - [optional] Parameter2
    //! \return LRESULT - Typically zero if handled, non-zero if not    (but not always)
    ///////////////////////////////////////////////////////////////////////////////
    static LRESULT WINAPI  WndProc(HWND hWnd, uint32 message, WPARAM wParam, LPARAM lParam)
    {
      WindowBase* wnd(nullptr);   //!< Window object associated with message, if any
       
      try
      {
        // Attempt to lookup window object 
        switch (static_cast<WindowMessage>(message))
        {
        // [CREATE/NCCREATE] Extract instance pointer from ::CreateWindow(..) call parameter data
        case WindowMessage::CREATE:
        case WindowMessage::NCCREATE:
          // Extract instance pointer
          wnd = reinterpret_cast<WindowBase*>( opaque_cast<CreateStruct>(lParam)->lpCreateParams );

          // Temporarily assign a weak handle reference for the duration of creation process
          *wnd->Handle = HWnd(hWnd, AllocType::WeakRef);    // Overwritten by strong reference returned from ::CreateWindow over message is processed

          // Add to 'Active Windows' collection
          ActiveWindows[hWnd] = wnd;
          break;

        // [WINDOW EXTENT] Unable to handle on first call
        case WindowMessage::GETMINMAXINFO:
          return getFunc<char_t>(::DefWindowProcA,::DefWindowProcW)(hWnd, message, wParam, lParam);

        // [REMAINDER] Lookup native handle from the 'Active Windows' collection
        default:
          // Lookup window handle
          if (ActiveWindows.find(hWnd) != ActiveWindows.end())
            wnd = ActiveWindows[hWnd];
          break;
        }
      
        // Delegate to instance procedure
        LResult msg = wnd->onMessage(static_cast<WindowMessage>(message), wParam, lParam);

        // [HANDLED/REFLECTED] Return result
        if (msg.Route != MsgRoute::Unhandled)
          return msg.Result;
      }
      catch (wtl::exception& e)
      {
        cdebug.log(HERE, e);
      }

      // [UNHANDLED/ERROR] Pass back to OS
      LRESULT result = getFunc<char_t>(::DefWindowProcA,::DefWindowProcW)(hWnd, message, wParam, lParam);
      
      // [CREATE/NCCREATE] Cleanup
      switch (auto msg = static_cast<WindowMessage>(message))
      {
      case WindowMessage::CREATE:
      case WindowMessage::NCCREATE:
        // [FAILED] Remove from 'Active Windows' collection
        if (result == -1)
          ActiveWindows.erase(hWnd);
        break;
      }

      // [UNHANDLED/ERROR] Return result
      return result;
    }

    // ---------------------- ACCESSORS ------------------------			
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::clientRect const
    //! Get the client rectangle
    //! 
    //! \return RectL - Client rectangle
    ///////////////////////////////////////////////////////////////////////////////
    RectL clientRect() const
    {
      RectL rc;
      ::GetClientRect(Handle, rc);
      return rc;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::getData
    //! Get window data
    //! 
    //! \param[in] atom - Registered class atom
    //! \param[in] instance - Associated instance handle
    //! 
    //! \throw wtl::platform_error - Unrecognised window class name
    ///////////////////////////////////////////////////////////////////////////////
    template <typename DATA>
    DATA* getData() const
    {
      return getFunc<char_t>(::GetWindowLongPtrA,::GetWindowLongPtrW)(Handle.get(), GWLP_USERDATA);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::exists const
    //! Query whether the window exists
    //! 
    //! \return bool - True iff window exists
    ///////////////////////////////////////////////////////////////////////////////
    bool exists() const
    {
      return handle() != nullptr;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::handle const
    //! Get the native window handle 
    //! 
    //! \return HWND - Native handle
    ///////////////////////////////////////////////////////////////////////////////
    HWND handle() const
    {
      return this && Handle.exists() ? Handle->get() : nullptr;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::windowRect const
    //! Get the window rectangle
    //! 
    //! \return RectL - Window rectangle
    ///////////////////////////////////////////////////////////////////////////////
    RectL windowRect() const
    {
      RectL rc;
      ::GetWindowRect(Handle, rc);
      return rc;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::operator HWND const
    //! Implicit user conversion to native window handle 
    //! 
    //! \return HWND - Native handle
    ///////////////////////////////////////////////////////////////////////////////
    operator HWND() const
    {
      return handle();
    }
    
    // ----------------------- MUTATORS ------------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::create
    //! Creates a parent window
    //! 
    //! \tparam ENC - Title string encoding
    //! \tparam LEN - Title buffer capacity
    //!
    //! \param[in] const* parent - Parent window
    //! \param[in] const& title - Window title
    //! \param[in] const& rc - Initial position
    //! \param[in] style - Window styles
    //! \param[in] exStyle - [optional] Extended styles (default is no extended styles)
    //! \param[in] const* menu - [optional] Window Menu (default is no window menu)
    //! 
    //! \throw wtl::logic_error - Window already exists
    //! \throw wtl::platform_error - Unable to create window
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC, unsigned LEN>
    void createEx(const WindowBase* parent, const CharArray<ENC,LEN>& title, const Rect<int32>& rc, WindowStyle style, WindowStyleEx exStyle = WindowStyleEx::None, const WindowMenu* menu = nullptr)
    {
      // Ensure doesn't already exist
      if (Handle)
        throw logic_error(HERE, "Window already exists");

      // Create handle object, insert dummy
      Handle.create(HWnd::npos);

      try
      {
        // Create window handle (assign weak-ref during onCreate(), overwrite with strong-ref HWnd from ::CreateWindow)
        *Handle = HWnd(Class.Instance, Class.Name, this, style, exStyle, CharArray<encoding,LEN>(title), rc, parent->handle(), menu->handle());
      }
      catch (platform_error& e)
      {
        // Log & rethrow
        cdebug.log(HERE, e);
        throw;
      }
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::create
    //! Create as a child window
    //! 
    //! \tparam ENC - Text string encoding
    //! \tparam LEN - Text buffer capacity
    //! \tparam IDENT - Child window identifier type
    //! \tparam STYLE - Window style type
    //!
    //! \param[in,out] &parent - Parent window
    //! \param[in] const& text - Window text
    //! \param[in] const& rc - Initial position
    //! \param[in] id - Window id
    //! \param[in] style - Window styles
    //! \param[in] exStyle - [optional] Extended styles (default is no extended styles)
    //! \param[in] const* menu - [optional] Window Menu (default is no window menu)
    //! 
    //! \throw wtl::logic_error - Window already exists
    //! \throw wtl::platform_error - Unable to create window
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC, unsigned LEN, typename IDENT = WindowId, typename STYLE = WindowStyle>
    void create(WindowBase& parent, const CharArray<ENC,LEN>& text, const Rect<int32>& rc, IDENT id, STYLE style = (STYLE)WindowStyle::Child, WindowStyleEx exStyle = WindowStyleEx::None)
    {
      // Ensure doesn't already exist
      if (Handle)
        throw logic_error(HERE, "Window already exists");

      // Create handle object, insert dummy
      Handle.create(HWnd::npos);

      try
      {
        // Create handle, assign weak-ref during onCreate(), overwrite with strong-ref HWnd from ::CreateWindow
        *Handle = HWnd(Class.Instance, Class.Name, this, static_cast<WindowId>(id), static_cast<WindowStyle>(style), exStyle, CharArray<encoding,LEN>(text), rc, parent.handle());

        // Add to parent's child windows collection
        parent.Children[static_cast<WindowId>(id)] = this;
      }
      catch (platform_error& e)
      {
        // Remove parent's child windows collection
        parent.Children.erase(static_cast<WindowId>(id));

        // Log & rethrow
        cdebug.log(HERE, e);
        throw;
      }
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::destroy
    //! Destroys the window
    //!
    //! \throw wtl::platform_error - Unable to destroy window
    ///////////////////////////////////////////////////////////////////////////////
    void destroy()
    {
      // Ensure doesn't already exist
      if (Handle)
        Handle.destroy();
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::find
    //! Find a child window
    //! 
    //! \tparam WINDOW - Child window type
    //! \tparam IDENT - Window id type
    //! 
    //! \param[in] child - Child window Id
    //! \return WINDOW& - Reference to child window
    //! 
    //! \throw wtl::domain_error - Mismatched child window type
    //! \throw wtl::logic_error - Missing child window
    ///////////////////////////////////////////////////////////////////////////////
    template <typename WINDOW, typename IDENT>
    WINDOW& find(IDENT child) 
    {
      // Lookup child window
      auto pos = Children.find(static_cast<WindowId>(child));
      if (pos != Children.end())
      {
        // [FOUND] Convert & return
        if (WINDOW* wnd = dynamic_cast<WINDOW*>(pos->second))
          return *wnd;

        // [ERROR] Incorrect window type
        throw domain_error(HERE, "Mismatched child window type");
      }

      // [ERROR] Unable to find child window
      throw logic_error(HERE, "Missing child window");
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::onPaint
    //! Called to paint the client area of the window
    //! 
    //! \param[in,out] args - Message arguments containing drawing data
    //! \return LResult - Message result and routing
    ///////////////////////////////////////////////////////////////////////////////
    LResult  onPaint(PaintWindowEventArgs<encoding>& args) 
    { 
      // Handled
      return 0; 
    }
  
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::post
    //! Posts a message to the window
    //! 
    //! \param[in] msg - Message identifier
    //! \param[in] wParam - First parameter
    //! \param[in] lParam - Second parameter
    ///////////////////////////////////////////////////////////////////////////////
    void post(WindowMessage msg, WPARAM wParam = 0, LPARAM lParam = 0)
    {
      getFunc<char_t>(::PostMessageA,::PostMessageW)(*Handle, enum_cast(msg), wParam, lParam);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::send
    //! Sends a message to the window
    //! 
    //! \param[in] msg - Message identifier
    //! \param[in] wParam - First parameter
    //! \param[in] lParam - Second parameter
    //! \return LRESULT - Message return value
    ///////////////////////////////////////////////////////////////////////////////
    LRESULT send(WindowMessage msg, WPARAM wParam = 0, LPARAM lParam = 0)
    {
      return getFunc<char_t>(::SendMessageA,::SendMessageW)(*Handle, enum_cast(msg), wParam, lParam);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::show
    //! Show or hide the window
    //! 
    //! \param[in] mode - Display method
    ///////////////////////////////////////////////////////////////////////////////
    void show(ShowWindowFlags mode)
    {
      ::ShowWindow(*Handle, enum_cast(mode));
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::getText
    //! Get the window text
    //! 
    //! \tparam LEN - Output buffer capacity
    //! 
    //! \param[in,out] &txt - On return this contains the window text
    //! \return int32 - Length in characters
    //! 
    //! \throw wtl::platform_error - Unable to get window text
    ///////////////////////////////////////////////////////////////////////////////
    template <unsigned LEN>
    int32 getText(const CharArray<encoding,LEN>& txt)
    {
      // Lookup window text
      int32 n = getFunc<char_t>(::GetWindowTextA,::GetWindowTextW)(*Handle, txt, LEN);
      if (n || !::GetLastError())
        return n;

      // Failed: Error
      throw platform_error(HERE, "Unable to get window text");
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::setText
    //! Set the window text
    //! 
    //! \tparam LEN - Input buffer capacity
    //! 
    //! \param[in] const& txt - Window text
    //! 
    //! \throw wtl::platform_error - Unable to set window text
    ///////////////////////////////////////////////////////////////////////////////
    template <unsigned LEN>
    void setText(const CharArray<encoding,LEN>& txt)
    {
      // Set window text
      if (getFunc<char_t>(::SetWindowTextA,::SetWindowTextW)(*Handle, txt, LEN) == FALSE)
        throw platform_error(HERE, "Unable to set window text");
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::update
    //! Updates the window (Sends a WM_PAINT message if any portion of the window is invalidated)
    ///////////////////////////////////////////////////////////////////////////////
    void update()
    {
      ::UpdateWindow(*Handle);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::operator +=
    //! Add an event handler
    //!
    //! \param[in] *ptr - Pointer to event handler
    //! \return window_t& - Reference to self
    ///////////////////////////////////////////////////////////////////////////////
    window_t& operator += (IEventHandler<ENC>* ptr)
    {
      Handlers += ptr;
      return *this;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::operator -=
    //! Remove an event handler
    //!
    //! \param[in] *ptr - Pointer to event handler
    //! \return window_t& - Reference to self
    ///////////////////////////////////////////////////////////////////////////////
    window_t& operator -= (IEventHandler<ENC>* ptr)
    {
      Handlers -= ptr;
      return *this;
    }

    // -------------------- REPRESENTATION ---------------------
  protected:
    wndclass_t&            Class;         //!< Window class 
    ChildWindowCollection  Children;      //!< Child window collection
    HandlerCollection      Handlers;      //!< Event handlers
    Lazy<HWnd>             Handle;        //!< Window handle
    SubClassCollection     SubClasses;    //!< Sub-classed windows collection
  };

  

}

#endif // WTL_WINDOW_BASE_HPP
