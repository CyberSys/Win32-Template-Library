//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\Window.hpp
//! \brief Basis for all window objects
//! \date 25 February 2016
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_HPP
#define WTL_WINDOW_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/BooleanCast.hpp>                              //!< boolean_cast
#include <wtl/casts/EnumCast.hpp>                                 //!< enum_cast
#include <wtl/casts/OpaqueCast.hpp>                               //!< opaque_cast
#include <wtl/traits/EncodingTraits.hpp>                          //!< Encoding
#include <wtl/traits/WindowTraits.hpp>                            //!< HWnd
#include <wtl/utils/Exception.hpp>                                //!< exception
#include <wtl/utils/Default.hpp>                                  //!< defvalue
#include <wtl/utils/ScopeGuard.hpp>                               //!< ScopeGuard
#include <wtl/utils/SFINAE.hpp>                                   //!< enable_if_numeric_t
#include <wtl/utils/Zero.hpp>                                     //!< zero
#include <wtl/io/Console.hpp>                                     //!< Console
#include <wtl/resources/ResourceId.hpp>                           //!< ResourceId
#include <wtl/platform/WindowFlags.hpp>                           //!< WindowStyle
#include <wtl/platform/CommonApi.hpp>                             //!< send_message
#include <wtl/platform/WindowMessage.hpp>                         //!< WindowMesssage
#include <wtl/windows/ChildWindowCollection.hpp>                  //!< ChildWindowCollection
#include <wtl/windows/Command.hpp>                                //!< Command
#include <wtl/windows/CommandGroup.hpp>                           //!< CommandGroup
#include <wtl/windows/CommandQueue.hpp>                           //!< CommandQueue
#include <wtl/windows/ControlEventArgs.hpp>                       //!< ControlEventArgs
#include <wtl/windows/Property.hpp>                               //!< Property
#include <wtl/windows/WindowClass.hpp>                            //!< WindowClass
#include <wtl/windows/WindowMenu.hpp>                             //!< WindowMenu
#include <wtl/windows/WindowSkin.hpp>                             //!< IWindowSkin, SkinFactory
#include <wtl/windows/SubClass.hpp>                               //!< SubClass
#include <wtl/windows/WindowId.hpp>                               //!< WindowId
#include <wtl/windows/events/AsyncSocketEvent.hpp>                //!< AsyncSocketEvent
#include <wtl/windows/events/CommandEvent.hpp>                    //!< CommandEvent
#include <wtl/windows/events/ColourizeEvent.hpp>                  //!< ColourizeEvent
#include <wtl/windows/events/CloseWindowEvent.hpp>                //!< CloseWindowEvent
#include <wtl/windows/events/CreateWindowEvent.hpp>               //!< CreateWindowEven
#include <wtl/windows/events/DestroyWindowEvent.hpp>              //!< DestroyWindowEvent
#include <wtl/windows/events/OwnerDrawCtrlEvent.hpp>              //!< OwnerDrawCtrlEvent
#include <wtl/windows/events/OwnerDrawMenuEvent.hpp>              //!< OwnerDrawMenuEvent
#include <wtl/windows/events/OwnerMeasureCtrlEvent.hpp>           //!< OwnerMeasureCtrlEvent
#include <wtl/windows/events/OwnerMeasureMenuEvent.hpp>           //!< OwnerMeasureMenuEvent
#include <wtl/windows/events/GainFocusEvent.hpp>                  //!< GainFocusEvent
#include <wtl/windows/events/LoseFocusEvent.hpp>                  //!< LoseFocusEvent
#include <wtl/windows/events/MouseMoveEvent.hpp>                  //!< MouseMoveEvent
#include <wtl/windows/events/MouseEnterEvent.hpp>                 //!< MouseEnterEvent
#include <wtl/windows/events/MouseLeaveEvent.hpp>                 //!< MouseLeaveEvent
#include <wtl/windows/events/MouseHoverEvent.hpp>                 //!< MouseHoverEvent
#include <wtl/windows/events/PaintWindowEvent.hpp>                //!< PaintWindowEvent
#include <wtl/windows/events/ShowWindowEvent.hpp>                 //!< ShowWindowEvent
#include <wtl/windows/events/PositionChangedEvent.hpp>            //!< PositionChangedEvent
#include <wtl/windows/properties/ClientRectProperty.h>            //!< ClientRectProperty
#include <wtl/windows/properties/ColourProperty.hpp>              //!< ColourProperty
#include <wtl/windows/properties/EnabledProperty.h>               //!< EnabledProperty
#include <wtl/windows/properties/FontProperty.h>                  //!< FontProperty
#include <wtl/windows/properties/FocusProperty.h>                 //!< FocusProperty
#include <wtl/windows/properties/IdentProperty.h>                 //!< IdentProperty
#include <wtl/windows/properties/PositionProperty.h>              //!< PositionProperty
#include <wtl/windows/properties/WindowRectProperty.h>            //!< WindowRectProperty
#include <wtl/windows/properties/SizeProperty.h>                  //!< SizeProperty
#include <wtl/windows/properties/StyleProperty.h>                 //!< StyleProperty
#include <wtl/windows/properties/StyleExProperty.h>               //!< StyleExProperty
#include <wtl/windows/properties/TextProperty.h>                  //!< TextProperty
#include <wtl/windows/properties/TextLengthProperty.h>            //!< TextLengthProperty
#include <wtl/windows/properties/VisibilityProperty.h>            //!< VisibilityProperty

//! \namespace wtl - Windows template library
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Window - Base for all window types
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct Window 
  {
    // Permit child window collection to modify 'this->Handle'
    template <Encoding E2>
    friend struct ChildWindowCollection;

    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \var encoding - Define window character encoding
    static constexpr Encoding encoding = ENC;

    //! \alias type - Define own type
    using type = Window<encoding>;
    
    //! \alias char_t - Define window character type
    using char_t = encoding_char_t<encoding>;
    
    //! \alias class_t - Window class type
    using class_t = WindowClass<encoding>;
    
    //! \alias command_t - Define Command type
    using command_t = Command<encoding>;
    
    //! \alias menu_t - Window menu type
    using menu_t = WindowMenu<encoding>;

    //! \alias resource_t - Resource identifier type
    using resource_t = ResourceId<encoding>;
    
    //! \alias skin_t - Window skin type
    using skin_t = IWindowSkin<encoding>;

    // ----------------------------------- REPRESENTATION -----------------------------------
  public:
    //! \var ActiveWindows - Static collection of all existing WTL windows 
    static WindowHandleCollection<ENC>  ActiveWindows;

    //! \var CommandGroups - Static collection of all Commands groups
    static CommandGroupCollection<ENC>   CommandGroups;

    //! \var DefaultPosition - Default window position upon creation
    //static constexpr PointL  DefaultPosition = {CW_USEDEFAULT, CW_USEDEFAULT};
    static const PointL  DefaultPosition;     //!< FIX: MSVC-14 does not support constexpr in-class initializers (C4579)

    //! \var DefaultSize - Default window size upon creation
    //static constexpr SizeL  DefaultSize  {CW_USEDEFAULT, CW_USEDEFAULT};
    static const SizeL  DefaultSize;      //!< FIX: MSVC-14 does not support constexpr in-class initializers (C4579)

    // ----------------------------------- REPRESENTATION -----------------------------------
    
    // Events
  public:
    AsyncSocketEvent<encoding>      AsyncSocket;    //!< Raised in response to custom message from sockets
    CommandEvent<encoding>          Command;        //!< Raised in response to WM_COMMAND from menu/accelerators
    ColourizeEvent<encoding>        Colourize;      //!< Raised in response to WM_CTLCOLOR___ from controls/dialogs
    CreateWindowEvent<encoding>     Create;         //!< Raised in response to WM_CREATE
    CloseWindowEvent<encoding>      Close;          //!< Raised in response to WM_CLOSE
    DestroyWindowEvent<encoding>    Destroy;        //!< Raised in response to WM_DESTROY
    LoseFocusEvent<encoding>        LoseFocus;      //!< Raised in response to WM_KILLFOCUS
    GainFocusEvent<encoding>        GainFocus;      //!< Raised in response to WM_SETFOCUS
    MouseEnterEvent<encoding>       MouseEnter;     //!< Raised in response to WM_MOUSEENTER
    MouseHoverEvent<encoding>       MouseHover;     //!< Raised in response to WM_MOUSEHOVER
    MouseLeaveEvent<encoding>       MouseLeave;     //!< Raised in response to WM_MOUSELEAVE
    MouseMoveEvent<encoding>        MouseMove;      //!< Raised in response to WM_MOUSEMOVE
    PaintWindowEvent<encoding>      Paint;          //!< Raised in response to WM_PAINT
    ShowWindowEvent<encoding>       Show;           //!< Raised in response to WM_SHOWWINDOW
    PositionChangedEvent<encoding>  Reposition;     //!< Raised in response to WM_WINDOWPOSCHANGED (sent by ::SetWindowPos(..) after moving/resizing window)
    
    // Properties
  public:
    ClientRectProperty<encoding>    ClientRect;     //!< Client rectangle property
    EnabledProperty<encoding>       Enabled;        //!< Window enabled property
    FontProperty<encoding>          Font;           //!< Window font property
    FocusProperty<encoding>         Focus;          //!< Keyboard focus property
    IdentProperty<encoding>         Ident;          //!< Child Window Id property
    PositionProperty<encoding>      Position;       //!< Window position property
    SizeProperty<encoding>          Size;           //!< Window size property
    StyleProperty<encoding>         Style;          //!< Window style property
    StyleExProperty<encoding>       StyleEx;        //!< Extended window style property
    TextProperty<encoding>          Text;           //!< Window text property
    //ColourProperty<encoding>        TextColour;     //!< Window text colour
    TextLengthProperty<encoding>    TextLength;     //!< Window text length property
    VisibilityProperty<encoding>    Visible;        //!< Visibility property
    WindowRectProperty<encoding>    WindowRect;     //!< Window rectangle property
    
    // Data
  public:
    CommandQueue<encoding>          ActionQueue;    //!< GUI Command queue
    ChildWindowCollection<encoding> Children;       //!< Child window collection
    WindowMenu<encoding>            Menu;           //!< Window menu, possibly empty

  protected:
    HWnd                            Handle;         //!< Window handle
    SubClassCollection<encoding>    SubClasses;     //!< Sub-classed windows collection

  private:
    bool                            IsMouseOver;    //!< True iff mouse is over the window while window has keyboard focus

    // ------------------------------------- CONSTRUCTION -----------------------------------
  public: 
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::Window
    //! Creates the window object (but not window handle) for an instance of a registered window class
    /////////////////////////////////////////////////////////////////////////////////////////
    Window() : ClientRect(*this),
               Children(*this),
               Enabled(*this, true),
               Font(*this, StockFont::Window),
               Focus(*this),
               Ident(*this, zero<WindowId>()),
               IsMouseOver(false),
               Handle(defvalue<HWnd>()),
               Position(*this, DefaultPosition),
               Size(*this, DefaultSize),
               Style(*this, WindowStyle::OverlappedWindow),
               Text(*this),
               TextLength(*this),
               StyleEx(*this, WindowStyleEx::None),
               Visible(*this, false),
               WindowRect(*this)
    {
      // Create/destroy events: Accept window creation parameters and destroy children upon destruction
      Create += new CreateWindowEventHandler<encoding>(this, &Window::onCreate);
      Destroy += new DestroyWindowEventHandler<encoding>(this, &Window::onDestroy);
      
      // Command events: Execute gui commands by default
      Command += new CommandEventHandler<encoding>(this, &Window::onCommand);
      
      // Focus events: Clears the 'mouse over' flag when losing focus
      //LoseFocus += new LoseFocusEventHandler<encoding>(this, &Window::onLoseFocus);

      // Mouse events: Registers for hover/leave notifications
      MouseMove += new MouseMoveEventHandler<encoding>(this, &Window::onMouseMove);
      MouseLeave += new MouseLeaveEventHandler<encoding>(this, &Window::onMouseLeave);
      
      // Paint events: Validate the client area
      Paint += new PaintWindowEventHandler<encoding>(this, &Window::onPaint);
    }

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    DISABLE_COPY(Window);     //!< Cannot be copied
    ENABLE_MOVE(Window);      //!< Can be moved
    ENABLE_POLY(Window);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::getFocus
    //! Get the window with input focus
    //!
    //! \return Window<encoding>* - Window with input focus, or nullptr if focus belongs to another thread
    //!
    //! \throw wtl::domain_error - Input focus belongs to native window on current thread
    /////////////////////////////////////////////////////////////////////////////////////////
    static Window<encoding>*  getFocus()
    {
      if (::HWND focus = ::GetFocus())
      {
        // Lookup & return window
        auto wnd = ActiveWindows.find(focus);
        if (wnd != ActiveWindows.end())
          return wnd->second;
        
        // [FAILED] Native window
        throw domain_error(HERE, "Input focus belongs to native window");
      }

      // [N/A] Input focus belongs to another thread
      return nullptr;
    }

  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::WndProc
    //! Class window procedure which receives messages dispatched by the system
    //!
    //! \param[in] hWnd - Window handle
    //! \param[in] message - Message ident
    //! \param[in] wParam - [optional] Parameter1
    //! \param[in] lParam - [optional] Parameter2
    //! \return LRESULT - Typically zero if handled, non-zero if not    (but not always)
    /////////////////////////////////////////////////////////////////////////////////////////
    static ::LRESULT WINAPI  WndProc(::HWND hWnd, uint32_t message, ::WPARAM wParam, ::LPARAM lParam)
    {
      LResult            msg;                 //!< Result and routing
      Window<encoding>*  wnd = nullptr;       //!< Window object associated with window handle
      BasicScopeGuard    onExit = [&]()       
      {
        // Ensure window object is removed from 'Active Windows' collection when processing of creation message fails
        if (msg.Result == -1 && (message == WM_CREATE || message == WM_NCCREATE))
          Window<encoding>::ActiveWindows.erase(hWnd);
      };
 
      try
      {
        // Attempt to lookup window object 
        switch (static_cast<WindowMessage>(message))
        {
        // [WINDOW EXTENT] Unable to associate with window object at this stage
        case WindowMessage::GetMinMaxInfo:
          return WinAPI<encoding>::defWindowProc(hWnd, message, wParam, lParam);

        // [CREATE/NCCREATE] Extract instance pointer from ::CreateWindow(..) call parameter data
        case WindowMessage::Create:
        case WindowMessage::NonClientCreate:
          //! \alias CreationData - Define WM_CREATE/WM_NCCREATE creation data
          using CreationData = typename EventArgs<encoding,WindowMessage::Create>::CreationData;

          // Extract instance pointer
          wnd = reinterpret_cast<Window<encoding>*>( opaque_cast<CreationData>(lParam)->lpCreateParams );

          // Temporarily assign a weak handle reference for the duration of creation process
          wnd->Handle = HWnd(hWnd, AllocType::WeakRef);    // Overwritten by strong reference returned from ::CreateWindow over message is processed

          // Add to 'Active Windows' collection
          type::ActiveWindows[hWnd] = wnd;
          break;

        // [REMAINDER] Lookup native handle from the 'Active Windows' collection
        default:
          // Lookup window handle
          if (type::ActiveWindows.find(hWnd) != type::ActiveWindows.end())
            wnd = type::ActiveWindows[hWnd];
          break;
        }
        
        // [ROUTE] Delegate to instance procedure
        msg = wnd->route(static_cast<WindowMessage>(message), wParam, lParam);

        // [UNHANDLED] 
        if (msg == MsgRoute::Unhandled)
        {
          // [SUB-CLASS] Delegate to subclass procedure
          if (!wnd->SubClasses.empty())
            msg = wnd->SubClasses.peek().route(*wnd, static_cast<WindowMessage>(message), wParam, lParam);
          else
            // [DEFAULT] Delegate to default window procedure
            msg = WinAPI<encoding>::defWindowProc(hWnd, message, wParam, lParam);
        }

        // Return result
        return msg.Result;
      }
      // [ERROR] Exception thrown by handler
      catch (std::exception& e)
      {
        cdebug << caught_exception("Unable to route message", HERE, e);

        // Delegate to the default procedure
        msg = WinAPI<encoding>::defWindowProc(hWnd, message, wParam, lParam);
        return msg.Result;
      }
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::data const
    //! Get window data
    //! 
    //! \param[in] atom - Registered class atom
    //! \param[in] instance - Associated instance handle
    //! \return DATA* - Mutable window data pointer
    //! 
    //! \throw wtl::platform_error - Unrecognised window class name
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename DATA>
    DATA* data() const
    {
      return reinterpret_cast<DATA*>( WinAPI<encoding>::getWindowLongPtr(Handle.get(), GWLP_USERDATA) );
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::exists const
    //! Query whether the window exists
    //! 
    //! \return bool - True iff window exists
    /////////////////////////////////////////////////////////////////////////////////////////
    bool exists() const
    {
      return Handle.exists();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::handle const
    //! Get the shared window handle 
    //! 
    //! \return const HWnd& - Shared window handle
    /////////////////////////////////////////////////////////////////////////////////////////
    const HWnd& handle() const
    {
      return Handle;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::isMouseOver const
    //! Query whether cursor is over window
    //! 
    //! \return bool - True iff mouse over window
    /////////////////////////////////////////////////////////////////////////////////////////
    bool isMouseOver() const
    {
      return IsMouseOver;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::parent const
    //! Get the parent window
    //! 
    //! \return Window<encoding>* - Parent window if any, otherwise nullptr
    //!
    //! \throw wtl::domain_error - Parent is native window
    /////////////////////////////////////////////////////////////////////////////////////////
    Window<encoding>* parent() const
    {
      // Query parent
      if (::HWND wnd = ::GetParent(Handle))
      {
        // Lookup window in 'Active Windows' collection
        auto pos = ActiveWindows.find(wnd);
        if (pos != ActiveWindows.end())
          return pos->second;

        // [ERROR] Parent is native window
        throw domain_error(HERE, "Parent is native window");
      }
      
      // Top level window
      return nullptr;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::toClient const
    //! Converts screen co-ordinates to client co-ordinates
    //! 
    //! \param[in] const& rc - Rectangle in screen co-ordinates 
    //! 
    //! \throw wtl::platform_error - Unable to convert rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    RectL toClient(const RectL& rc) const
    {
      PointL pt = rc.topLeft();

      // Convert screen-to-client
      if (!::ScreenToClient(handle(), pt))
        throw platform_error(HERE, "Unable to convert co-ordinates");

      return { pt, rc.size() };
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::toScreen const
    //! Converts client co-ordinates to screen co-ordinates
    //! 
    //! \param[in] const& rc - Rectangle in client co-ordinates 
    //! 
    //! \throw wtl::platform_error - Unable to convert rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    RectL toScreen(const RectL& rc) const
    {
      PointL pt = rc.topLeft();

      // Convert client-to-client
      if (!::ClientToScreen(handle(), pt))
        throw platform_error(HERE, "Unable to convert co-ordinates");

      return { pt, rc.size() };
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::wndclass const
    //! Get the window class
    //! 
    //! \return const WindowClass<encoding>& - Shared window class
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual const WindowClass<encoding>& wndclass() const = 0;
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::operator ::HWND const
    //! Implicit user conversion to native window handle 
    //! 
    //! \return ::HWND - Native handle
    /////////////////////////////////////////////////////////////////////////////////////////
    operator ::HWND() const
    {
      return handle();
    }
  
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::create
    //! Creates the window (as a child, popup, or overlapped window)
    //!
    //! \param[in,out] *owner - [optional] Parent/owner window   (Required for child windows)
    //! 
    //! \throw wtl::invalid_argument - Attempting to create a child window without a parent window
    //! \throw wtl::logic_error - Window already exists
    //! \throw wtl::platform_error - Unable to create window
    //! 
    //! \remarks The window handle is initialized twice during the construction process. When the ::CreateWindow() call sends a
    //! \remarks WM_CREATE message, the wndproc saves a weak-ref in this->Handle that is later overwritten by the strong-ref returned
    //! \remarks from the ::CreateWindow() call.
    //!
    //! \remarks When creating a child window, it is automatically added to the ChildWindowCollection of its owner
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual void create(type* owner = nullptr)
    {
      // Ensure window does not exist
      if (Handle.exists())
        throw logic_error(HERE, "Window already exists");

      try
      {
        // [CHILD] Create window  
        if (Ident != zero<WindowId>())
        {
          // Require parent window
          if (!owner)
            throw invalid_argument(HERE, "Missing parent window");

          // Ensure parent window exists
          if (!owner->exists())
            throw logic_error(HERE, "Parent window does not exist");
          
          // Add self to parent's child windows
          owner->Children.add(*this);
        }
        // [POPUP/OVERLAPPED] Create window (possibly with menu)
        else
        {
          ::HWND parent = owner ? (::HWND)owner->handle() : defvalue<::HWND>();          //!< Use parent if any

          // Create as popup/overlapped (Do not supply menu yet to allow client to populate it)
          this->Handle = HWnd(wndclass(), this, parent, Style, StyleEx, defvalue<::HMENU>(), Text(), Position, Size);

          // [MENU] Attach menu if populated during onCreate(..)
          if (!Menu.empty())
            ::SetMenu(Handle, Menu.handle());
        }
      }
      // [FAILED] Ensure weak-ref handle is cleared
      catch (std::exception&)
      {
        Handle.release();
        throw;
      }
    }
      
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::destroy
    //! Destroys the window and menu 
    //!
    //! \throw wtl::platform_error - Unable to destroy window
    /////////////////////////////////////////////////////////////////////////////////////////
    void destroy()
    {
      // Ensure exists
      if (Handle.exists())
      {
        // TODO: Detach menu?
        ::SetMenu(Handle, nullptr);

        // Destroy window 
        Handle.release();

        // NB: Font & Menu handles are released by members
      }
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::execute
    //! Executes an Command, adding it to the commands queue
    //! 
    //! \param[in] id - Command id
    //!
    //! \throw wtl::logic_error - Command not found
    /////////////////////////////////////////////////////////////////////////////////////////
    void  execute(CommandId id) 
    { 
      // Lookup command and execute 
      if (auto cmd = CommandGroups.find(id))
        ActionQueue.execute(cmd->clone());
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::focus
    //! Sets keyboard focus to this window
    /////////////////////////////////////////////////////////////////////////////////////////
    void focus()
    {
      ::SetFocus(Handle);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::invalidate
    //! Invalidates the entire client rectangle
    //! 
    //! \throw wtl::platform_error - Unable to invalidate window
    /////////////////////////////////////////////////////////////////////////////////////////
    void invalidate()
    {
      if (!::InvalidateRect(Handle, nullptr, True))
        throw platform_error(HERE, "Unable to invalidate window");
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::invalidate
    //! Invalidates a portion of the client rectangle
    //! 
    //! \param[in] const& rc - Rectangle to invalidate
    //! \param[in] erase - [optional] Whether to erase the background
    //! 
    //! \throw wtl::platform_error - Unable to invalidate window
    /////////////////////////////////////////////////////////////////////////////////////////
    void invalidate(const RectL& rc, bool erase = false)
    {
      if (!::InvalidateRect(Handle, rc, boolean_cast(erase)))
        throw platform_error(HERE, "Unable to invalidate window");
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::post
    //! Posts a message to the window
    //! 
    //! \tparam MESSAGE - Message type
    //!
    //! \param[in] msg - Message
    //! \param[in] w - [optional] First parameter
    //! \param[in] l - [optional] Second parameter
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename MESSAGE> 
    void post(MESSAGE msg, ::WPARAM w = 0, ::LPARAM l = 0)
    {
      post_message<encoding>(msg, Handle, w, l);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::send
    //! Sends a message to the window
    //! 
    //! \tparam MESSAGE - Message type
    //!
    //! \param[in] msg - Message
    //! \param[in] w - [optional] First parameter
    //! \param[in] l - [optional] Second parameter
    //! \return LResult - Message result & routing
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename MESSAGE> 
    LResult send(MESSAGE msg, ::WPARAM w = 0, ::LPARAM l = 0)
    {
      return send_message<encoding>(msg, Handle, w, l);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::show
    //! Show or hide the window
    //! 
    //! \param[in] mode - Display method
    /////////////////////////////////////////////////////////////////////////////////////////
    void show(ShowWindowFlags mode = ShowWindowFlags::Show)
    {
      ::ShowWindow(Handle, enum_cast(mode));
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::update
    //! Updates the window (Sends a WM_PAINT message if any portion of the window is invalidated)
    /////////////////////////////////////////////////////////////////////////////////////////
    void update()
    {
      ::UpdateWindow(Handle);
    }

  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::route
    //! Routes messages to an instance's handlers (This is the 'Instance window procedure')
    //!
    //! \param[in] message - Window message identifier
    //! \param[in] w - [optional] First message parameter
    //! \param[in] l - [optional] Second message parameter
    //! \return LResult - Message routing and result 
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual LResult route(WindowMessage message, ::WPARAM w, ::LPARAM l)
    {
      try
      {
        LResult ret;       //!< Message result, defaults to unhandled
        
        // [EVENT] Raise event associated with message
        switch (message)
        {
        // [CREATE] Create window
        case WindowMessage::Create:
          if (!Create.empty()) 
            ret = Create.raise( CreateWindowEventArgs<encoding>(w,l) ); 
          break;

        // [CLOSE/DESTROY] 
        case WindowMessage::Close:            ret = Close.raise();                                                  break;
        case WindowMessage::Destroy:          ret = Destroy.raise();                                                break;

        // [FOCUS] 
        case WindowMessage::SetFocus:         ret = GainFocus.raise(GainFocusEventArgs<encoding>(w,l));             break;
        case WindowMessage::KillFocus:        ret = LoseFocus.raise(LoseFocusEventArgs<encoding>(w,l));             break;

        // [MOUSE] 
        case WindowMessage::MouseHover:       ret = MouseHover.raise(MouseHoverEventArgs<encoding>(w,l));           break;
        case WindowMessage::MouseLeave:       ret = MouseLeave.raise(MouseLeaveEventArgs<encoding>(w,l));           break;
        case WindowMessage::MouseMove:        ret = MouseMove.raise(MouseMoveEventArgs<encoding>(w,l));             break;

        // [SHOW/MOVE] 
        case WindowMessage::ShowWindow:            ret = Show.raise(ShowWindowEventArgs<encoding>(w,l));            break;
        case WindowMessage::WindowPositionChanged: ret = Reposition.raise(PositionChangedEventArgs<encoding>(w,l)); break;

        // [SOCKET]
        case WindowMessage::Socket:           ret = AsyncSocket.raise(AsyncSocketEventArgs<encoding>(w,l));         break;

        // [COMMAND] Reflect control events. Raise Gui events.
        case WindowMessage::Command:  
          if (l != 0)
            // [CONTROL] Reflect to sender
            ret = ControlEventArgs<encoding,WindowMessage::Command>(w,l).reflect();
          else
            // [COMMAND] Raise event (Default executes the appropriate command object)
            ret = Command.raise(CommandEventArgs<encoding>(w,l));
          break;

        // [NOTIFY] Reflect to sender
        case WindowMessage::Notify:  
          ret = ControlEventArgs<encoding,WindowMessage::Notify>(w,l).reflect();   
          break;

        // [OWNER-DRAW] Reflect to sender
        case WindowMessage::DrawItem:       
          // [CONTROL] Reflect to originator control
          if (w != 0)
            ret = OwnerDrawCtrlEventArgs<encoding>(w,l).reflect();

          // [MENU] Raise menu's OwnerDraw event
          else  
            ret = Menu.OwnerDraw.raise( OwnerDrawMenuEventArgs<encoding>(w,l) ); 
          break;
        
        // [OWNER-MEASURE] Reflect to sender
        case WindowMessage::MeasureItem: 
          // [CONTROL] Reflect to originator
          if (w != 0) 
            ret = OwnerMeasureCtrlEventArgs<encoding>(Children.find(window_id(w)).handle(), w, l).reflect();
          
          // [MENU] Raise associated menu event  
          else 
            ret = Menu.OwnerMeasure.raise( OwnerMeasureMenuEventArgs<encoding>(Handle,w,l) );   //!< [Pass arguments by reference]
          break;

        // [PAINT] Avoid instantiating arguments if event is empty (thereby leaving update region invalidated)
        case WindowMessage::Paint:          
          if (!Paint.empty()) 
            ret = Paint.raise( PaintWindowEventArgs<encoding>(Handle,w,l) );  //!< [Pass arguments by reference]
          break;

        // [CONTROL-COLOUR] Reflect to sender
        case WindowMessage::CtrlColourButton:     ret = ColourizeEventArgs<encoding,WindowMessage::CtrlColourButton>(w,l).reflect();     break;
        case WindowMessage::CtrlColourEdit:       ret = ColourizeEventArgs<encoding,WindowMessage::CtrlColourEdit>(w,l).reflect();       break;
        case WindowMessage::CtrlColourListbox:    ret = ColourizeEventArgs<encoding,WindowMessage::CtrlColourListbox>(w,l).reflect();    break;
        case WindowMessage::CtrlColourScrollbar:  ret = ColourizeEventArgs<encoding,WindowMessage::CtrlColourScrollbar>(w,l).reflect();  break;
        case WindowMessage::CtrlColourStatic:     ret = ColourizeEventArgs<encoding,WindowMessage::CtrlColourStatic>(w,l).reflect();     break;
        }

        // [UNHANDLED] Return result & routing
        return ret;
      }
      catch (std::exception& e)
      {
        cdebug << caught_exception("Unable to route message", HERE, e);
        
        // [ERROR] Unhandled
        return { MsgRoute::Unhandled, -1 };
      }
    }
    
  private:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::onCreate
    //! Called during window creation to modify window parameters and create child windows
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual LResult  onCreate(CreateWindowEventArgs<encoding>& args) 
    { 
      // [Handled] Accept window parameters
      return {MsgRoute::Handled, 0};
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Window::onDestroy
    //! Called during window destruction to destroy all child windows
    //! 
    //! \return wtl::LResult - Routing indicating message was handled
    ///////////////////////////////////////////////////////////////////////////////
    virtual LResult  onDestroy() 
    { 
      // Destroy child windows
      this->Children.clear();

      // [Handled] 
      return {wtl::MsgRoute::Handled, 0};
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::onCommand
    //! Called in response to a command raised by menu or accelerator (ie. WM_COMMAND)
    //! 
    //! \param[in] args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    //! 
    //! \throw wtl::logic_error - Gui command not recognised
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onCommand(CommandEventArgs<encoding> args) 
    { 
      // Execute associated command
      execute(args.Ident);

      // Handled
      return {MsgRoute::Handled, 0};
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::onMouseLeave
    //! Called to clear 'IsMouseOver' flag when cursor leaves window
    //! 
    //! \param[in] args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onMouseLeave(MouseLeaveEventArgs<encoding> args) 
    {
      // Clear flag
      IsMouseOver = false;

      // Handle message
      return {MsgRoute::Handled, 0};
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::onMouseMove
    //! Called to track mouse events when the mouse is moved over the window
    //! 
    //! \param[in] args - Message arguments 
    //! \return LResult - Routing indicating message was not handled
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onMouseMove(MouseMoveEventArgs<encoding> args) 
    {
      // Register for mouse hover/leave notifications
      if (!IsMouseOver)
      {
        ::TRACKMOUSEEVENT data { sizeof(::TRACKMOUSEEVENT), TME_HOVER|TME_LEAVE, Handle, HOVER_DEFAULT };
      
        // Register for client-area leave and hover notifications
        if (!::TrackMouseEvent(&data))
          throw platform_error(HERE, "Unable to track mouse events");

        // Mark as registered
        IsMouseOver = true;

        // [EVENT] Raise 'MouseEnter'
        MouseEnter.raise(args);
      }

      // Pass-through message
      return {MsgRoute::Unhandled, -1};
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::onPaint
    //! Called to paint the client area of the window
    //! 
    //! \param[in,out] args - Message arguments containing drawing data
    //! \return LResult - Routing indicating message was handled
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual LResult  onPaint(PaintWindowEventArgs<encoding>& args) 
    { 
      // Perform fallback drawing
      SkinFactory<encoding>::get()->draw(*this, args.Graphics, args.Rect);

      // Handle message
      return {MsgRoute::Handled, 0};
    }
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \var Window::DefaultPosition - Default window position upon creation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  const PointL  Window<ENC>::DefaultPosition = {CW_USEDEFAULT, CW_USEDEFAULT};    //!< FIX: MSVC-14 does not support constexpr in-class initializers (C4579)
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \var Window::DefaultSize - Default window size upon creation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  const SizeL  Window<ENC>::DefaultSize = {CW_USEDEFAULT, CW_USEDEFAULT};     //!< FIX: MSVC-14 does not support constexpr in-class initializers (C4579)

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \var Window::CommandGroups - Collection of all Command groups 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  CommandGroupCollection<ENC>   Window<ENC>::CommandGroups;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \var Window::ActiveWindows - Collection of all WTL windows 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  WindowHandleCollection<ENC>   Window<ENC>::ActiveWindows;

  
} // namespace wtl


#include <wtl/windows/properties/ClientRectProperty.hpp>        //!< ClientRectProperty
#include <wtl/windows/properties/EnabledProperty.hpp>           //!< EnabledProperty
#include <wtl/windows/properties/FontProperty.hpp>              //!< FontProperty
#include <wtl/windows/properties/FocusProperty.hpp>             //!< FocusProperty
#include <wtl/windows/properties/IdentProperty.hpp>             //!< IdentProperty
#include <wtl/windows/properties/PositionProperty.hpp>          //!< PositionProperty
#include <wtl/windows/properties/WindowRectProperty.hpp>        //!< WindowRectProperty
#include <wtl/windows/properties/SizeProperty.hpp>              //!< SizeProperty
#include <wtl/windows/properties/StyleProperty.hpp>             //!< StyleProperty
#include <wtl/windows/properties/StyleExProperty.hpp>           //!< StyleExProperty
#include <wtl/windows/properties/TextProperty.hpp>              //!< TextProperty
#include <wtl/windows/properties/TextLengthProperty.hpp>        //!< TextLengthProperty
#include <wtl/windows/properties/VisibilityProperty.hpp>        //!< VisibilityProperty

#endif // WTL_WINDOW_HPP
