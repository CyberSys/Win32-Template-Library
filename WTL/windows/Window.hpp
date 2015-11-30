//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\Window.hpp
//! \brief Basis for all window classes
//! \date 19 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_HPP
#define WTL_WINDOW_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/BooleanCast.hpp>                              //!< BooleanCast
#include <wtl/casts/EnumCast.hpp>                                 //!< EnumCast
#include <wtl/casts/OpaqueCast.hpp>                               //!< OpaqueCast
#include <wtl/traits/EncodingTraits.hpp>                          //!< Encoding
#include <wtl/traits/WindowTraits.hpp>                            //!< HWnd
#include <wtl/utils/Exception.hpp>                                //!< exception
#include <wtl/utils/List.hpp>                                     //!< List
#include <wtl/utils/Default.hpp>                                  //!< Default
#include <wtl/utils/CharArray.hpp>                                //!< CharArray
#include <wtl/utils/Zero.hpp>                                     //!< Zero
#include <wtl/io/Console.hpp>                                     //!< Console
#include <wtl/resources/ResourceId.hpp>                            //!< ResourceId
#include <wtl/platform/WindowFlags.hpp>                           //!< WindowStyle
#include <wtl/platform/CommonApi.hpp>                             //!< send_message
#include <wtl/platform/WindowMessage.hpp>                         //!< WindowMesssage
#include <wtl/windows/Command.hpp>                                //!< Command
#include <wtl/windows/CommandGroup.hpp>                           //!< CommandGroup
#include <wtl/windows/CommandQueue.hpp>                           //!< CommandQueue
#include <wtl/windows/ControlEventArgs.hpp>                       //!< ControlEventArgs
#include <wtl/windows/Property.hpp>                               //!< Property
#include <wtl/windows/WindowClass.hpp>                            //!< WindowClass
#include <wtl/windows/WindowMenu.hpp>                             //!< WindowMenu
#include <wtl/windows/WindowSkin.hpp>                             //!< IWindowSkin
#include <wtl/windows/SubClass.hpp>                               //!< SubClass
#include <wtl/windows/events/CommandEvent.hpp>                    //!< CommandEvent
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
#include <wtl/windows/properties/EnabledProperty.h>               //!< EnabledProperty
#include <wtl/windows/properties/FontProperty.h>                  //!< FontProperty
#include <wtl/windows/properties/IdentProperty.h>                 //!< IdentProperty
#include <wtl/windows/properties/PositionProperty.h>              //!< PositionProperty
#include <wtl/windows/properties/WindowRectProperty.h>            //!< WindowRectProperty
#include <wtl/windows/properties/SizeProperty.h>                  //!< SizeProperty
#include <wtl/windows/properties/StyleProperty.h>                 //!< StyleProperty
#include <wtl/windows/properties/StyleExProperty.h>               //!< StyleExProperty
#include <wtl/windows/properties/TextProperty.h>                  //!< TextProperty
#include <wtl/windows/properties/TextLengthProperty.h>            //!< TextLengthProperty
#include <wtl/windows/properties/VisibilityProperty.h>            //!< VisibilityProperty
#include <map>                                                    //!< std::map

//! \namespace wtl - Windows template library
namespace wtl 
{
  // Forward declaration
  template <Encoding ENC>
  struct Window;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias WindowCollection - Window collection type
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using WindowCollection = List<Window<ENC>*>;
    
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias WindowHandleCollection - Provides an association between native window handles and Window objects
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using WindowHandleCollection = std::map<::HWND,Window<ENC>*>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias WindowIdCollection - Provides an association between window Ids and Window objects
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using WindowIdCollection = std::map<WindowId,Window<ENC>*>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::window_id
  //! Creates a strongly typed window id from any integral or enumeration type
  //!
  //! \tparam TYPE - Integral or enumeration type
  //! 
  //! \param[in] id - Value representing window id
  //! \return WindowId - WindowId representation of 'id'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename VALUE, typename = std::enable_if_t<std::is_integral<VALUE>::value || std::is_enum<VALUE>::value>>
  WindowId window_id(VALUE id)
  {
    // Convert into underlying type then cast to enumeration
    return enum_cast<WindowId>( static_cast<std::underlying_type_t<WindowId>>(id) );
  }
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ColourProperty - Define property type used to represent colours
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using ColourProperty = Property<PropertyImpl<ENC,Colour>>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Window - Base for all window types
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct Window 
  {
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

    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct ChildWindowCollection - Define child window collection type
    /////////////////////////////////////////////////////////////////////////////////////////
    struct ChildWindowCollection : WindowIdCollection<encoding>
    {
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
      //! \alias base - Define base type
      using base = WindowIdCollection<encoding>;
  
      //! \alias type - Define own type
      using type = ChildWindowCollection;
  
      // ----------------------------------- REPRESENTATION -----------------------------------
    protected:
      Window<ENC>&  Parent;        //!< Parent/owner of collection
      
      // ------------------------------------ CONSTRUCTION ------------------------------------
    public:
      /////////////////////////////////////////////////////////////////////////////////////////
      // ChildWindowCollection::ChildWindowCollection
      //! Create empty collection
      //! 
      //! \param[in] &parent - Parent/owner of collection
      /////////////////////////////////////////////////////////////////////////////////////////
      ChildWindowCollection(Window<ENC>& parent) : Parent(parent)
      {}
      
      // ----------------------------------- STATIC METHODS -----------------------------------

      // ---------------------------------- ACCESSOR METHODS ----------------------------------

      // ----------------------------------- MUTATOR METHODS ----------------------------------

      /////////////////////////////////////////////////////////////////////////////////////////
      // ChildWindowCollection::create
      //! Creates a child window and inserts it into the collection
      //! 
      //! \param[in,out] &child - Child window object  (Handle must not exist)
      //! 
      //! \throw wtl::logic_error - Window already exists
      //! \throw wtl::platform_error - Unable to create window
      /////////////////////////////////////////////////////////////////////////////////////////
      void create(Window<ENC>& child)
      {
        // Ensure child doesn't already exist
        if (child.exists())
          throw logic_error(HERE, "Window already exists");

        // Create child window  (calls 'insert()' if successful)
        child.create(&Parent);
      }
      
      /////////////////////////////////////////////////////////////////////////////////////////
      // ChildWindowCollection::insert
      //! Inserts an existing child window into the collection
      //!
      //! \param[in,out] &child - Child window object  (Handle must exist)
      //! 
      //! \throw wtl::logic_error - Window does not exist
      /////////////////////////////////////////////////////////////////////////////////////////
      void insert(Window<ENC>& child)
      {
        // Ensure child exists
        if (!child.exists())
          throw logic_error(HERE, "Window does not exist");

        // Add to collection
        this->emplace(child.Ident, &child);
      }
    };
    
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
  public:
    // Events
    CommandEvent<encoding>          Command;        //!< Raised in response to WM_COMMAND from menu/accelerators
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
    
    // Fields
    CommandQueue<encoding>          ActionQueue;    //!< GUI Command queue
    ChildWindowCollection           Children;       //!< Child window collection
    WindowMenu<encoding>            Menu;           //!< Window menu, possibly empty

    // Properties
    ClientRectProperty<encoding>    ClientRect;     //!< Client rectangle property
    EnabledProperty<encoding>       Enabled;        //!< Window enabled property
    FontProperty<encoding>          Font;           //!< Window font property
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

  protected:
    HWnd                            Handle;         //!< Window handle
    SubClassCollection              SubClasses;     //!< Sub-classed windows collection

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
      // Create events: Accept window creation parameters
      Create += new CreateWindowEventHandler<encoding>(this, &Window::onCreate);
      
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

    // -------------------------------- COPY, MOVE & DESTROY  -------------------------------
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
    // Window::isUnhandled
    //! Query whether a result is the 'Unhandled' return code
    //!
    //! \param[in] message - Window message identifier
    //! \param[in] res - Message result 
    //! \return bool - True iff result indicates message was unhandled
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool  isUnhandled(WindowMessage msg, ::LRESULT res)
    {
      switch (msg)
      {
      case WindowMessage::Create:         return res == unhandled_result<WindowMessage::Create>::value;
      case WindowMessage::Destroy:        return res == unhandled_result<WindowMessage::Destroy>::value;
      case WindowMessage::DrawItem:       return res == unhandled_result<WindowMessage::DrawItem>::value;
      case WindowMessage::GetMinMaxInfo:  return res == unhandled_result<WindowMessage::GetMinMaxInfo>::value;
      case WindowMessage::KillFocus:      return res == unhandled_result<WindowMessage::KillFocus>::value;
      case WindowMessage::ShowWindow:     return res == unhandled_result<WindowMessage::ShowWindow>::value;
      case WindowMessage::Size:           return res == unhandled_result<WindowMessage::Size>::value;
      case WindowMessage::SetFocus:       return res == unhandled_result<WindowMessage::SetFocus>::value;
      default:                            return res != 0;
      }
    }

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
      Window* wnd(nullptr);   //!< Window object associated with message, if any
       
      try
      {
        // Attempt to lookup window object 
        switch (static_cast<WindowMessage>(message))
        {
        // [CREATE/NCCREATE] Extract instance pointer from ::CreateWindow(..) call parameter data
        case WindowMessage::Create:
        case WindowMessage::NonClientCreate:
          //! \alias CreationData - Define WM_CREATE/WM_NCCREATE creation data
          using CreationData = typename EventArgs<encoding,WindowMessage::Create>::CreationData;

          // Extract instance pointer
          wnd = reinterpret_cast<Window*>( opaque_cast<CreationData>(lParam)->lpCreateParams );

          // Temporarily assign a weak handle reference for the duration of creation process
          wnd->Handle = HWnd(hWnd, AllocType::WeakRef);    // Overwritten by strong reference returned from ::CreateWindow over message is processed

          // Add to 'Active Windows' collection
          ActiveWindows[hWnd] = wnd;
          break;

        // [WINDOW EXTENT] Unable to handle on first call in a thread-safe manner
        case WindowMessage::GetMinMaxInfo:
          return WinAPI<encoding>::defWindowProc(hWnd, message, wParam, lParam);

        // [REMAINDER] Lookup native handle from the 'Active Windows' collection
        default:
          // Lookup window handle
          if (ActiveWindows.find(hWnd) != ActiveWindows.end())
            wnd = ActiveWindows[hWnd];
          break;
        }
        
        // Delegate to instance procedure
        LResult msg = wnd->route(static_cast<WindowMessage>(message), wParam, lParam);

        // [HANDLED/REFLECTED] Return result
        if (msg.Route != MsgRoute::Unhandled)
          return msg.Result;
      }
      // [ERROR] Exception thrown by handler
      catch (std::exception& e)
      {
        cdebug << caught_exception("Unable to route message", HERE, e);
      }

      // [UNHANDLED/ERROR] Pass back to OS
      ::LRESULT result = WinAPI<encoding>::defWindowProc(hWnd, message, wParam, lParam);
      
      // [CREATE/NCCREATE] Cleanup
      switch (auto msg = static_cast<WindowMessage>(message))
      {
      case WindowMessage::Create:
      case WindowMessage::NonClientCreate:
        // [FAILED] Remove from 'Active Windows' collection
        if (result == -1)
          ActiveWindows.erase(hWnd);
        break;
      }

      // [UNHANDLED/ERROR] Return result
      return result;
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
    // Window::wndclass const
    //! Get the window class
    //! 
    //! \return const class_t& - Shared window class
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
    //! \remarks The window handle is first accessible during WM_CREATE (not before during WM_GETMINMAXINFO)
    //! \remarks This is a weak-ref handle assigned by the class window procedure, a strong-ref is returned 
    //! \remarks and saved here if the creation is successful.
    //!
    //! \remarks Child windows are automatically added to the ChildWindowCollection of the parent
    /////////////////////////////////////////////////////////////////////////////////////////
    void create(type* owner = nullptr)
    {
      // Ensure window does not exist
      if (Handle.exists())
        throw logic_error(HERE, "Window already exists");

      // [CHILD] Create window  
      if (Ident != zero<WindowId>())
      {
        // Require parent window
        if (!owner)
          throw invalid_argument(HERE, "Missing parent window");

        // Require existant parent window
        if (!owner->exists())
          throw logic_error(HERE, "Parent window does not exist");

        // Create as child 
        Handle = HWnd(wndclass(), *this, owner->handle(), Ident, Style, StyleEx, Text(), Position, Size);

        // Add to parent's collection of child windows
        owner->Children.insert(*this);
      }
      // [POPUP/OVERLAPPED] Create window (possibly with menu)
      else
      {
        ::HWND parent = owner ? (::HWND)owner->handle() : defvalue<::HWND>();          //!< Use parent if any

        // Create as popup/overlapped (Do not supply menu yet to allow client to populate it)
        Handle = HWnd(wndclass(), *this, parent, Style, StyleEx, defvalue<::HMENU>(), Text(), Position, Size);

        // [MENU] Attach menu if populated during onCreate(..)
        if (!Menu.empty())
          ::SetMenu(Handle, Menu.handle());
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
    // Window::find
    //! Find a child window of any type
    //! 
    //! \tparam WINDOW - [optional] Child window type (Default is Window)
    //! \tparam IDENT - [optional] Window id type (Default is WindowId)
    //! 
    //! \param[in] child - Child window Id
    //! \return WINDOW& - Strongly-typed child window reference
    //! 
    //! \throw wtl::domain_error - Mismatched window type
    //! \throw wtl::logic_error - Child not found
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename WINDOW = type, typename IDENT = WindowId>
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
    //! \tparam WM - Window Message 
    //!
    //! \param[in] w - [optional] First parameter
    //! \param[in] l - [optional] Second parameter
    /////////////////////////////////////////////////////////////////////////////////////////
    template <WindowMessage WM> 
    void post(::WPARAM w = 0, ::LPARAM l = 0)
    {
      post_message<encoding,WM>(Handle, w, l);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::send
    //! Sends a message to the window
    //! 
    //! \tparam WM - Window Message 
    //!
    //! \param[in] w- [optional] First parameter
    //! \param[in] l - [optional] Second parameter
    //! \return LResult - Message result & routing
    /////////////////////////////////////////////////////////////////////////////////////////
    template <WindowMessage WM> 
    LResult send(::WPARAM w = 0, ::LPARAM l = 0)
    {
      return send_message<encoding,WM>(Handle, w, l);
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
        case WindowMessage::Create: { 
            CreateWindowEventArgs<encoding> args(w,l); //!< [Pass arguments by reference]
            ret = Create.raise(args); 
          } break;

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
        case WindowMessage::ShowWindow:       ret = Show.raise(ShowWindowEventArgs<encoding>(w,l));                 break;
        case WindowMessage::WindowPositionChanged: ret = Reposition.raise(PositionChangedEventArgs<encoding>(w,l));      break;

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
          else  {
            OwnerDrawMenuEventArgs<encoding> args(w,l);
            ret = Menu.OwnerDraw.raise(args); 
          }
          break;
        
        // [OWNER-MEASURE] Reflect to sender
        case WindowMessage::MeasureItem: 
          // [CONTROL] Reflect to originator
          if (w != 0) 
            ret = OwnerMeasureCtrlEventArgs<encoding>(find(window_id(w)).handle(), w, l).reflect();
          
          // [MENU] Raise associated menu event  
          else {
            OwnerMeasureMenuEventArgs<encoding> args(Handle,w,l);
            ret = Menu.OwnerMeasure.raise(args);                          //!< [Pass arguments by reference]
          }
          break;

        // [PAINT] Avoid instantiating arguments if event is empty (thereby leaving update region invalidated)
        case WindowMessage::Paint:          
          if (!Paint.empty()) {
            PaintWindowEventArgs<encoding> args(Handle,w,l);
            ret = Paint.raise(args);                                      //!< [Pass arguments by reference]
          }
          break;
        }

        // [SUB-CLASS] Offer message to each subclass in turn (if any)
        for (auto& wnd : SubClasses)
          switch (wnd.Type)
          {
          // [WTL WINDOW] Delegate to window object 
          case SubClass::WindowType::Library:
            // Delegate to instance window procedure
            ret = wnd.WndProc.Library(message, w, l);

            // [HANDLED/REFLECTED] Return result & routing
            if (ret.Route == MsgRoute::Handled || ret.Route == MsgRoute::Reflected)
              return ret;
            break;

          // [NATIVE WINDOW] Call window procedure via Win32 API and determine routing from result
          case SubClass::WindowType::Native:
            // Delegate to native class window procedure and infer routing
            ret.Result = WinAPI<encoding>::callWindowProc(wnd.WndProc.Native, Handle, enum_cast(message), w, l);
            ret.Route = (isUnhandled(message, ret.Result) ? MsgRoute::Unhandled : MsgRoute::Handled);
          
            // [HANDLED] Return result & routing
            if (ret.Route == MsgRoute::Handled)
              return ret;
          }

        // [UNHANDLED] Return result & routing
        return ret;
      }
      catch (std::exception& e)
      {
        cdebug << caught_exception("Unable to route message", HERE, e);
        
        // [ERROR] Unhandled
        return MsgRoute::Unhandled;
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
    LResult  onCreate(CreateWindowEventArgs<encoding>& args) 
    { 
      // [Handled] Accept window parameters
      return {MsgRoute::Handled, 0};
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

      // Handle message
      return {MsgRoute::Unhandled};
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Window::onPaint
    //! Called to paint the client area of the window
    //! 
    //! \param[in,out] args - Message arguments containing drawing data
    //! \return LResult - Routing indicating message was handled
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onPaint(PaintWindowEventArgs<encoding>& args) 
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
