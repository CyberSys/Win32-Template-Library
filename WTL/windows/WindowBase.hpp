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
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct WindowBase - Base for all window types
  //! 
  //! \tparam ENC - Window character encoding
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct WindowBase 
  {
    struct SubClass;

    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias char_t - Define window character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias command_t - Define gui command base type
    using command_t = IGuiCommand<ENC>;

    //! \alias resource_t - Resource identifier type
    using resource_t = ResourceId<ENC>;
    
    //! \alias CreateStruct - Define WM_CREATE/WM_NCCREATE creation data
    using CreateStruct = getType<char_t,::CREATESTRUCTA,::CREATESTRUCTW>;

    //! \alias CommandQueue - Define gui command queue type
    using CommandQueue = GuiCommandQueue<ENC>;
    
    //! \struct CommandCollection - Provides a collection of Gui Commands, indexed by Command Id
    struct CommandCollection : std::map<CommandId,std::shared_ptr<command_t>>
    {
      //! \alias base - Define base type
      using base = std::map<CommandId,std::shared_ptr<command_t>>;

      ///////////////////////////////////////////////////////////////////////////////
      // CommandCollection::operator +=
      //! Add a command to the collection
      //!
      //! \param[in] *ptr - Gui command
      //! \return CommandCollection& - Reference to self
      ///////////////////////////////////////////////////////////////////////////////
      CommandCollection& operator += (command_t* ptr)
      {
        // Insert/overwrite
        emplace(ptr->ident(), std::shared_ptr<command_t>(ptr));
        return *this;
      }
    };
    
    //! \alias WindowCollection - Window collection type
    using WindowCollection = std::list<WindowBase*>;
    
    //! \alias WindowHandleCollection - Provides an association between native window handles and WindowBase objects
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
    using wndclass_t = WindowClass<ENC>;

    //! \alias wndproc_t - Win32 Window procedure type
    using wndproc_t = LRESULT (__stdcall*)(HWND, uint32, WPARAM, LPARAM);

    //! \alias wtlproc_t - WTL Window procedure type
    using wtlproc_t = LResult (__thiscall*)(WindowMessage, WPARAM, LPARAM);  // = decltype(onMessage);

    //! \alias window_t - Define own type
    using window_t = WindowBase;
    
    //! \var encoding - Define window character encoding
    static constexpr Encoding encoding = ENC;

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

    // -------------------- REPRESENTATION ---------------------
  public:
    //! \var ActiveWindows - Static collection of all existing WTL windows for the current process
    static WindowHandleCollection  ActiveWindows;

    //! \var ActiveCommands - Static collection of all existing gui commands for the current process
    static CommandCollection  ActiveCommands;
    
  public:
    CreateWindowEvent<encoding>      Create;        //!< Raised in response to WM_CREATE
    CloseWindowEvent<encoding>       Close;         //!< Raised in response to WM_CLOSE
    DestroyWindowEvent<encoding>     Destroy;       //!< Raised in response to WM_DESTROY
    PaintWindowEvent<encoding>       Paint;         //!< Raised in response to WM_PAINT
    ShowWindowEvent<encoding>        Show;          //!< Raised in response to WM_SHOWWINDOW
    CommandEvent<encoding>           Command;       //!< Raised in response to WM_COMMAND from menu/accelerators
    CtrlCommandEvent<encoding>       CtrlCommand;   //!< Raised in response to WM_COMMAND from child controls
    CtrlNotifyEvent<encoding>        CtrlNotify;    //!< Raised in response to WM_NOTIFY from child controls

  protected:
    wndclass_t&            Class;         //!< Window class 
    ChildWindowCollection  Children;      //!< Child window collection
    CommandQueue           Actions;       //!< Command queue
    Lazy<HWnd>             Handle;        //!< Window handle
    SubClassCollection     SubClasses;    //!< Sub-classed windows collection

    // --------------------- CONSTRUCTION ----------------------
  public: 
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::WindowBase
    //! Creates the window object (but not window handle) for an instance of a registered window class
    //! 
    //! \param[in] &cls - Registered window class 
    ///////////////////////////////////////////////////////////////////////////////
    WindowBase(wndclass_t& cls) : Class(cls)
    {
      // Accept window creation by default
      Create += new CreateWindowEventHandler<encoding>(this, &WindowBase::onCreate);
      
      // Paint window background by default
      Paint += new PaintWindowEventHandler<encoding>(this, &WindowBase::onPaint);
      
      // Reflect control events/notifications by default
      CtrlCommand += new CtrlCommandEventHandler<encoding>(this, &WindowBase::onControlEvent);
      CtrlNotify += new CtrlNotifyEventHandler<encoding>(this, &WindowBase::onControlNotify);

      // Execute gui commands by default
      Command += new CommandEventHandler<encoding>(this, &WindowBase::onCommand);
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
    // WindowBase::routeMessage
    //! Routes messages to an instance's handlers (This is the 'Instance window procedure')
    //!
    //! \param[in] message - Window message identifier
    //! \param[in] w - [optional] First message parameter
    //! \param[in] l - [optional] Second message parameter
    //! \return LResult - Message routing and result 
    ///////////////////////////////////////////////////////////////////////////////
    LResult routeMessage(WindowMessage message, WPARAM w, LPARAM l)
    {
      try
      {
        LResult ret;       //!< Message result, defaults to unhandled

        // [EVENT] Raise event associated with message
        switch (message)
        {
        case WindowMessage::CREATE:         ret = Create.raise(CreateWindowEventArgs<encoding>(w,l));             break;
        case WindowMessage::CLOSE:          ret = Close.raise();                                                  break;
        case WindowMessage::DESTROY:        ret = Destroy.raise();                                                break;
        case WindowMessage::SHOWWINDOW:     ret = Show.raise(ShowWindowEventArgs<encoding>(w,l));                 break;
        case WindowMessage::NOTIFY:         ret = CtrlNotify.raise(CtrlNotifyEventArgs<encoding>(w,l));           break;

        case WindowMessage::COMMAND:  
          // [CTRL-EVENT] Default implementation reflects message to child window
          if (l != 0)
            ret = CtrlCommand.raise(CtrlCommandEventArgs<encoding>(w,l));  

          // [MENU/ACCELERATOR] Default implemenation executes the appropriate command object
          else
            ret = Command.raise(CommandEventArgs<encoding>(w,l));
          break;

        case WindowMessage::PAINT:          
          if (!Paint.empty())
            ret = Paint.raise(PaintWindowEventArgs<encoding>(*Handle,w,l));       
          break;
        }

        // [SUB-CLASS] Offer message to each subclass in turn (if any)
        for (auto& wnd : SubClasses)
          switch (wnd.Type)
          {
          // [WTL WINDOW] Delegate to window object 
          case WindowType::Library:
            // Delegate to instance window procedure
            ret = wnd.WndProc.Library(message, w, l);

            // [HANDLED/REFLECTED] Return result & routing
            if (ret.Route == MsgRoute::Handled || ret.Route == MsgRoute::Reflected)
              return ret;
            break;

          // [NATIVE WINDOW] Call window procedure via Win32 API and determine routing from result
          case WindowType::Native:
            // Delegate to native class window procedure and infer routing
            ret.Result = getFunc<char_t>(::CallWindowProcA,::CallWindowProcW)(wnd.WndProc.Native, *Handle, enum_cast(message), w, l);
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

        // [WINDOW EXTENT] Unable to handle on first call in a thread-safe manner
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
        LResult msg = wnd->routeMessage(static_cast<WindowMessage>(message), wParam, lParam);

        // [HANDLED/REFLECTED] Return result
        if (msg.Route != MsgRoute::Unhandled)
          return msg.Result;

        // TODO: Gui Commands

        //// [UNHANDLED GUI-COMMAND] Search for a handler
        //for (window_t* parent = wnd->parent(); parent; parent = parent->parent())
        //{
        //  // Delegate to parent's instance procedure
        //  msg = wnd->routeMessage(static_cast<WindowMessage>(message), wParam, lParam);

        //  // [PARENT HANDLED/REFLECTED] Return result
        //  if (msg.Route != MsgRoute::Unhandled)
        //    return msg.Result;
        //}
      }
      // [ERROR] Exception thrown by handler
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
    // WindowBase::parent const
    //! Get the parent window
    //! 
    //! \return window_t* - Parent window if any, otherwise nullptr
    //!
    //! \throw wtl::domain_error - Parent is native window
    ///////////////////////////////////////////////////////////////////////////////
    window_t* parent() const
    {
      // Query parent
      if (HWND wnd = ::GetParent(*Handle))
      {
        // Lookup window in 'Active Windows' collection
        auto pos = ActiveWindows.find(wnd);
        if (pos != ActiveWindows.end())
          pos->second;

        // [ERROR] Parent is native window
        throw domain_error(HERE, "Parent is native window");
      }
      
      // Top level window
      return nullptr;
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
      if (Handle.exists())
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
      if (Handle.exists())
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
      if (Handle.exists())
        Handle.destroy();
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::execute
    //! Executes a gui command
    //! 
    //! \throw wtl::logic_error - Gui command not recognised
    ///////////////////////////////////////////////////////////////////////////////
    void  execute(CommandId id) 
    { 
      // Lookup & Execute associated command
      Actions.execute( ActiveCommands[id]->clone() );
    }

    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::find
    //! Find a child window
    //! 
    //! \tparam WINDOW - [optional] Child window type (If unspecified a base window pointer is returned)
    //! \tparam IDENT - [optional] Window id type (If unspecified, WindowId is used)
    //! 
    //! \param[in] child - Child window Id
    //! \return WINDOW& - Reference to child window
    //! 
    //! \throw wtl::domain_error - Mismatched child window type
    //! \throw wtl::logic_error - Missing child window
    ///////////////////////////////////////////////////////////////////////////////
    template <typename WINDOW = window_t, typename IDENT = WindowId>
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
    // WindowBase::onCreate
    //! Called during window creation to modify window parameters and create child windows
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Message result and routing
    ///////////////////////////////////////////////////////////////////////////////
    virtual LResult  onCreate(CreateWindowEventArgs<encoding>& args) 
    { 
      // [Handled] Accept parameters
      return 0; 
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::onControlEvent
    //! Called in response to events from child controls (ie. WM_COMMAND)
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Message result and routing
    ///////////////////////////////////////////////////////////////////////////////
    virtual LResult  onControlEvent(CtrlCommandEventArgs<encoding>& args) 
    { 
      // [Reflected] Reflect message to sender
      return args.reflect(); 
    }

    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::onControlNotify
    //! Called in response to notifications from child controls (ie. WM_NOTIFY)
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Message result and routing
    ///////////////////////////////////////////////////////////////////////////////
    virtual LResult  onControlNotify(CtrlNotifyEventArgs<encoding>& args) 
    { 
      // [Reflected] Reflect message to sender
      return args.reflect(); 
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::onCommand
    //! Called in response to a command raised by menu or accelerator (ie. WM_COMMAND)
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Message result and routing
    //! 
    //! \throw wtl::logic_error - Gui command not recognised
    ///////////////////////////////////////////////////////////////////////////////
    virtual LResult  onCommand(CommandEventArgs<encoding>& args) 
    { 
      // Execute associated command
      execute(args.Ident);

      // Handled
      return 0;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::onPaint
    //! Called to paint the client area of the window
    //! 
    //! \param[in,out] args - Message arguments containing drawing data
    //! \return LResult - Message result and routing
    ///////////////////////////////////////////////////////////////////////////////
    virtual LResult  onPaint(PaintWindowEventArgs<encoding>& args) 
    { 
      // [Handled] Validate client area
      return 0; 
    }
  
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::post
    //! Posts a message to the window
    //! 
    //! \tparam WM - Window Message 
    //!
    //! \param[in] w- First parameter
    //! \param[in] l - Second parameter
    ///////////////////////////////////////////////////////////////////////////////
    template <WindowMessage WM> 
    void post(WPARAM w = 0, LPARAM l = 0)
    {
      post_message<encoding,WM>(*Handle, w, l);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::send
    //! Sends a message to the window
    //! 
    //! \tparam WM - Window Message 
    //!
    //! \param[in] w- First parameter
    //! \param[in] l - Second parameter
    //! \return LResult - Message result & routing
    ///////////////////////////////////////////////////////////////////////////////
    template <WindowMessage WM> 
    LResult send(WPARAM w = 0, LPARAM l = 0)
    {
      return send_message<encoding,WM>(*Handle, w, l);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowBase::setFont
    //! Set the window font
    //! 
    //! \param[in] const& f - New window font
    //! \param[in] l - Second parameter
    //! \return HFont - Weak reference to previous window font
    ///////////////////////////////////////////////////////////////////////////////
    HFont setFont(const HFont& f, bool redraw)
    {
      ::HFONT prev = (::HFONT)send<WindowMessage::SETFONT>((::WPARAM)f.get(), boolean_cast(redraw)).Result;
      return { prev, AllocType::WeakRef };
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
  };

  
  //! \var ActiveCommands - Collection of all existing gui commands for the current process
  template <Encoding ENC>
  typename WindowBase<ENC>::CommandCollection   WindowBase<ENC>::ActiveCommands;
  
  //! \var ActiveWindows - Collection of all existing WTL windows for the current process
  template <Encoding ENC>
  typename WindowBase<ENC>::WindowHandleCollection   WindowBase<ENC>::ActiveWindows;


}

#endif // WTL_WINDOW_BASE_HPP
