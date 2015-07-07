//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\WindowBase.hpp
//! \brief Base class for windows
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_BASE_HPP
#define WTL_WINDOW_BASE_HPP

#include "wtl/WTL.hpp"
#include "wtl/casts/BooleanCast.hpp"                              //!< BooleanCast
#include "wtl/casts/EnumCast.hpp"                                 //!< EnumCast
#include "wtl/casts/NativeCast.hpp"                               //!< NativeCast
#include "wtl/casts/OpaqueCast.hpp"                               //!< OpaqueCast
#include "wtl/traits/EncodingTraits.hpp"                          //!< Encoding
#include "wtl/traits/WindowTraits.hpp"                            //!< HWnd
#include "wtl/utils/Exception.hpp"                                //!< exception
#include "wtl/utils/ExceptionLog.hpp"                             //!< exception_log
#include "wtl/utils/List.hpp"                                     //!< List
#include "wtl/utils/Default.hpp"                                  //!< Default
#include "wtl/utils/CharArray.hpp"                                //!< CharArray
#include "wtl/utils/Zero.hpp"                                     //!< Zero
#include "wtl/io/Console.hpp"                                     //!< Console
#include "wtl/platform/ResourceId.hpp"                            //!< ResourceId
#include "wtl/platform/WindowFlags.hpp"                           //!< WindowStyle
#include "wtl/platform/CommonApi.hpp"                             //!< send_message
#include "wtl/platform/WindowMessage.hpp"                         //!< WindowMesssage
#include "wtl/windows/Command.hpp"                                 //!< Command
#include "wtl/windows/CommandGroup.hpp"                            //!< CommandGroup
#include "wtl/windows/CommandGroupCollection.hpp"                  //!< CommandGroup
#include "wtl/windows/CommandQueue.hpp"                            //!< CommandQueue
#include "wtl/windows/Property.hpp"                               //!< Property
#include "wtl/windows/WindowClass.hpp"                            //!< WindowClass
#include "wtl/windows/WindowMenu.hpp"                             //!< WindowMenu
#include "wtl/windows/events/CommandEvent.hpp"                     //!< CommandEvent
#include "wtl/windows/events/CloseWindowEvent.hpp"                //!< CloseWindowEvent
#include "wtl/windows/events/CreateWindowEvent.hpp"               //!< CreateWindowEven
#include "wtl/windows/events/CtrlCommandEvent.hpp"                //!< CtrlCommandEvent
#include "wtl/windows/events/CtrlNotifyEvent.hpp"                 //!< CtrlNotifyEvent
#include "wtl/windows/events/DestroyWindowEvent.hpp"              //!< DestroyWindowEvent
#include "wtl/windows/events/OwnerDrawCtrlEvent.hpp"              //!< OwnerDrawCtrlEvent
#include "wtl/windows/events/OwnerDrawMenuEvent.hpp"              //!< OwnerDrawMenuEvent
#include "wtl/windows/events/OwnerMeasureCtrlEvent.hpp"           //!< OwnerMeasureCtrlEvent
#include "wtl/windows/events/OwnerMeasureMenuEvent.hpp"           //!< OwnerMeasureMenuEvent
#include "wtl/windows/events/PaintWindowEvent.hpp"                //!< PaintWindowEvent
#include "wtl/windows/events/ShowWindowEvent.hpp"                 //!< ShowWindowEvent
#include "wtl/windows/events/PositionChangedEvent.hpp"            //!< PositionChangedEvent
#include "wtl/windows/properties/ClientRectProperty.hpp"          //!< ClientRectProperty
#include "wtl/windows/properties/EnabledProperty.hpp"             //!< EnabledProperty
#include "wtl/windows/properties/FontProperty.hpp"                //!< FontProperty
#include "wtl/windows/properties/IdentProperty.hpp"               //!< IdentProperty
#include "wtl/windows/properties/PositionProperty.hpp"            //!< PositionProperty
#include "wtl/windows/properties/WindowRectProperty.hpp"          //!< WindowRectProperty
#include "wtl/windows/properties/SizeProperty.hpp"                //!< SizeProperty
#include "wtl/windows/properties/StyleProperty.hpp"               //!< StyleProperty
#include "wtl/windows/properties/StyleExProperty.hpp"             //!< StyleExProperty
#include "wtl/windows/properties/TextProperty.hpp"                //!< TextProperty
#include "wtl/windows/properties/TextLengthProperty.hpp"          //!< TextLengthProperty
#include "wtl/windows/properties/VisibilityProperty.hpp"          //!< VisibilityProperty
#include <map>                                                    //!< std::map

//! \namespace wtl - Windows template library
namespace wtl 
{
  // Forward declaration
  template <Encoding ENC>
  struct WindowBase;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias WindowCollection - Window collection type
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using WindowCollection = List<WindowBase<ENC>*>;
    
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias WindowHandleCollection - Provides an association between native window handles and WindowBase objects
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using WindowHandleCollection = std::map<::HWND,WindowBase<ENC>*>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias WindowIdCollection - Provides an association between window Ids and WindowBase objects
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using WindowIdCollection = std::map<WindowId,WindowBase<ENC>*>;
    

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct WindowBase - Base for all window types
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct WindowBase 
  {
    struct SubClass;

    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \var encoding - Define window character encoding
    static constexpr Encoding encoding = ENC;

    //! \alias type - Define own type
    using type = WindowBase<encoding>;
    
    //! \alias char_t - Define window character type
    using char_t = encoding_char_t<encoding>;
    
    //! \alias action_t - Define Command type
    using action_t = Command<encoding>;
    
    //! \alias resource_t - Resource identifier type
    using resource_t = ResourceId<encoding>;
    
    //! \alias wndclass_t - Window class type
    using wndclass_t = WindowClass<encoding>;
    
    //! \alias wndmenu_t - Window menu type
    using wndmenu_t = WindowMenu<encoding>;

    //! \alias wndproc_t - Class window procedure signature
    using wndproc_t = LRESULT (__stdcall*)(::HWND, uint32_t, ::WPARAM, ::LPARAM);

    //! \alias wtlproc_t - Instance window procedure signature
    using wtlproc_t = LResult (__thiscall*)(WindowMessage, ::WPARAM, ::LPARAM);  

    //! \alias CreateStruct - Define WM_CREATE/WM_NCCREATE creation data
    using CreateStruct = getType<char_t,::CREATESTRUCTA,::CREATESTRUCTW>;

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
      WindowBase<ENC>&  Parent;        //!< Parent/owner of collection
      
      // ------------------------------------ CONSTRUCTION ------------------------------------
    public:
      /////////////////////////////////////////////////////////////////////////////////////////
      // ChildWindowCollection::ChildWindowCollection
      //! Create empty collection
      //! 
      //! \param[in] &parent - Parent/owner of collection
      /////////////////////////////////////////////////////////////////////////////////////////
      ChildWindowCollection(WindowBase<ENC>& parent) : Parent(parent)
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
      void create(WindowBase<ENC>& child)
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
      void insert(WindowBase<ENC>& child)
      {
        // Ensure child exists
        if (!child.exists())
          throw logic_error(HERE, "Window does not exist");

        // Add to collection
        this->emplace(child.Ident, &child);
      }
    };
    
    //! \enum WindowType - Define window types
    enum class WindowType
    {
      Library,   //!< Wtl window 
      Native,    //!< Native window
    };

    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct SubClass - Represents a subclassed window
    /////////////////////////////////////////////////////////////////////////////////////////
    struct SubClass
    {
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------

      //! \union WindowProc - Window procedure
      union WindowProc
      {
        WindowProc(wndproc_t n) : Native(n)  {}
        WindowProc(wtlproc_t l) : Library(l) {}

        wndproc_t  Native;     //!< Win32
        wtlproc_t  Library;    //!< WTL
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
      
      // -------------------------------- COPYING & DESTRUCTION -------------------------------

      // ----------------------------------- STATIC METHODS -----------------------------------

      // ---------------------------------- ACCESSOR METHODS ----------------------------------

      // ----------------------------------- MUTATOR METHODS ----------------------------------
    };
    
    //! \alias SubClassCollection - Define subclassed windows collection
    using SubClassCollection = List<SubClass>;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  public:
    //! \var ActiveWindows - Static collection of all existing WTL windows 
    static WindowHandleCollection<encoding>  ActiveWindows;

    //! \var CommandGroups - Static collection of all Commands groups
    static CommandGroupCollection<encoding>   CommandGroups;

    //! \var DefaultPosition - Default window creation position
    static constexpr PointL  DefaultPosition = {CW_USEDEFAULT, CW_USEDEFAULT};

    //! \var DefaultSize - Default window creation size
    static constexpr SizeL  DefaultSize = {CW_USEDEFAULT, CW_USEDEFAULT};

    // ----------------------------------- REPRESENTATION -----------------------------------
  public:
    // Events
    CommandEvent<encoding>               Command;        //!< Raised in response to WM_COMMAND from menu/accelerators
    CreateWindowEvent<encoding>         Create;        //!< Raised in response to WM_CREATE
    CloseWindowEvent<encoding>          Close;         //!< Raised in response to WM_CLOSE
    DestroyWindowEvent<encoding>        Destroy;       //!< Raised in response to WM_DESTROY
    PaintWindowEvent<encoding>          Paint;         //!< Raised in response to WM_PAINT
    ShowWindowEvent<encoding>           Show;          //!< Raised in response to WM_SHOWWINDOW
    PositionChangedEvent<encoding>      Repositioned;  //!< Raised in response to WM_WINDOWPOSCHANGED (sent by ::SetWindowPos(..) after moving/resizing window)
    
    // Fields
    CommandQueue<encoding>               Commands;       //!< Commands queue
    ChildWindowCollection               Children;      //!< Child window collection
    WindowMenu<encoding>                Menu;          //!< Window menu, possibly empty

    // Properties
    ClientRectProperty<encoding>        ClientRect;    //!< Client rectangle property
    EnabledProperty<encoding>           Enabled;       //!< Window enabled property
    FontProperty<encoding>              Font;          //!< Window font property
    IdentProperty<encoding>             Ident;         //!< Child Window Id property
    PositionProperty<encoding>          Position;      //!< Window position property
    SizeProperty<encoding>              Size;          //!< Window size property
    StyleProperty<encoding>             Style;         //!< Window style property
    StyleExProperty<encoding>           StyleEx;       //!< Extended window style property
    TextProperty<encoding>              Text;          //!< Window text property
    TextLengthProperty<encoding>        TextLength;    //!< Window text length property
    VisibilityProperty<encoding>        Visible;       //!< Visibility property
    WindowRectProperty<encoding>        WindowRect;    //!< Window rectangle property

  protected:
    WindowClass<encoding>&              Class;         //!< Window class reference
    HWnd                                Handle;        //!< Window handle
    SubClassCollection                  SubClasses;    //!< Sub-classed windows collection

    // ------------------------------------- CONSTRUCTION -----------------------------------
  public: 
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::WindowBase
    //! Creates the window object (but not window handle) for an instance of a registered window class
    //! 
    //! \param[in] &cls - Registered window class 
    /////////////////////////////////////////////////////////////////////////////////////////
    WindowBase(wndclass_t& cls) : Class(cls), 
                                  ClientRect(*this),
                                  Children(*this),
                                  Enabled(*this, true),
                                  Font(*this, StockObject::SystemFont),
                                  Ident(*this, zero<WindowId>()),
                                  Handle(default<HWnd>()),
                                  Position(*this, DefaultPosition),
                                  Size(*this, DefaultSize),
                                  Style(*this, WindowStyle::OverlappedWindow),
                                  Text(*this),
                                  TextLength(*this),
                                  StyleEx(*this, WindowStyleEx::None),
                                  Visible(*this, Visibility::ShowNormal),
                                  WindowRect(*this)
    {
      // Accept window creation by default
      Create += new CreateWindowEventHandler<encoding>(this, &WindowBase::onCreate);
      
      // Execute gui commands by default
      Command += new CommandEventHandler<encoding>(this, &WindowBase::onCommand);
        
      // Paint window background by default
      Paint += new PaintWindowEventHandler<encoding>(this, &WindowBase::onPaint);
    }

    // -------------------------------- COPYING & DESTRUCTION -------------------------------
  public:
    DISABLE_COPY(WindowBase);     //!< Cannot be copied
    ENABLE_MOVE(WindowBase);      //!< Can be moved
    ENABLE_POLY(WindowBase);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::getFocus
    //! Get the window with input focus
    //!
    //! \return WindowBase<ENC>* - Window with input focus, or nullptr if focus belongs to another thread
    //!
    //! \throw wtl::domain_error - Input focus belongs to native window on current thread
    /////////////////////////////////////////////////////////////////////////////////////////
    static WindowBase<ENC>*  getFocus()
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
    // WindowBase::isUnhandled
    //! Query whether a result is the 'Unhandled' return code
    //!
    //! \param[in] message - Window message identifier
    //! \param[in] res - Message result 
    //! \return bool - True iff result indicates message was unhandled
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool  isUnhandled(WindowMessage msg, LRESULT res)
    {
      switch (msg)
      {
      case WindowMessage::CREATE:         return res == unhandled_result<WindowMessage::CREATE>::value;
      case WindowMessage::DESTROY:        return res == unhandled_result<WindowMessage::DESTROY>::value;
      case WindowMessage::DRAWITEM:       return res == unhandled_result<WindowMessage::DRAWITEM>::value;
      case WindowMessage::GETMINMAXINFO:  return res == unhandled_result<WindowMessage::GETMINMAXINFO>::value;
      case WindowMessage::KILLFOCUS:      return res == unhandled_result<WindowMessage::KILLFOCUS>::value;
      case WindowMessage::SHOWWINDOW:     return res == unhandled_result<WindowMessage::SHOWWINDOW>::value;
      case WindowMessage::SIZE:           return res == unhandled_result<WindowMessage::SIZE>::value;
      case WindowMessage::SETFOCUS:       return res == unhandled_result<WindowMessage::SETFOCUS>::value;
      default:                            return res != 0;
      }
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::WndProc
    //! Class window procedure 
    //!
    //! \param[in] hWnd - Window handle
    //! \param[in] message - Message ident
    //! \param[in] wParam - [optional] Parameter1
    //! \param[in] lParam - [optional] Parameter2
    //! \return LRESULT - Typically zero if handled, non-zero if not    (but not always)
    /////////////////////////////////////////////////////////////////////////////////////////
    static ::LRESULT WINAPI  WndProc(::HWND hWnd, uint32_t message, ::WPARAM wParam, ::LPARAM lParam)
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
          wnd->Handle = HWnd(hWnd, AllocType::WeakRef);    // Overwritten by strong reference returned from ::CreateWindow over message is processed

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
        LResult msg = wnd->route(static_cast<WindowMessage>(message), wParam, lParam);

        // [HANDLED/REFLECTED] Return result
        if (msg.Route != MsgRoute::Unhandled)
          return msg.Result;
      }
      // [ERROR] Exception thrown by handler
      catch (wtl::exception& e)
      {
        cdebug << exception_log(HERE,e,"Unable to route message") << endl;
      }

      // [UNHANDLED/ERROR] Pass back to OS
      ::LRESULT result = getFunc<char_t>(::DefWindowProcA,::DefWindowProcW)(hWnd, message, wParam, lParam);
      
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

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::data const
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
      return reinterpret_cast<DATA*>( getFunc<char_t>(::GetWindowLongPtrA,::GetWindowLongPtrW)(Handle.get(), GWLP_USERDATA) );
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::exists const
    //! Query whether the window exists
    //! 
    //! \return bool - True iff window exists
    /////////////////////////////////////////////////////////////////////////////////////////
    bool exists() const
    {
      return Handle.exists();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::handle const
    //! Get the shared window handle 
    //! 
    //! \return const HWnd& - Shared window handle
    /////////////////////////////////////////////////////////////////////////////////////////
    const HWnd& handle() const
    {
      return Handle;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::parent const
    //! Get the parent window
    //! 
    //! \return WindowBase<ENC>* - Parent window if any, otherwise nullptr
    //!
    //! \throw wtl::domain_error - Parent is native window
    /////////////////////////////////////////////////////////////////////////////////////////
    WindowBase<ENC>* parent() const
    {
      // Query parent
      if (::HWND wnd = ::GetParent(Handle))
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
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::operator ::HWND const
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
    // WindowBase::create
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
        Handle = HWnd(Class, *this, owner->handle(), Ident(), Style, StyleEx, Text, Position, Size);

        // Add to parent's collection of child windows
        owner->Children.insert(*this);
      }
      // [POPUP/OVERLAPPED] Create window (possibly with menu)
      else
      {
        ::HWND parent = owner ? (::HWND)owner->handle() : default<::HWND>();          //!< Use parent if any

        // Create as popup/overlapped (Do not supply menu yet to allow client to populate it)
        Handle = HWnd(Class, *this, parent, Style, StyleEx, default<::HMENU>(), Text, Position, Size);

        // [MENU] Attach menu if populated during onCreate(..)
        if (!Menu.empty())
          ::SetMenu(Handle, Menu.handle());
      }
    }
      
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::destroy
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
    // WindowBase::execute
    //! Executes an Command, adding it to the actions queue
    //! 
    //! \param[in] id - Command id
    //!
    //! \throw wtl::logic_error - Command not found
    /////////////////////////////////////////////////////////////////////////////////////////
    void  execute(CommandId id) 
    { 
      // Lookup action and execute 
      if (auto cmd = CommandGroups.find(id))
        Commands.execute(cmd->clone());
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::find
    //! Find a child window of any type
    //! 
    //! \tparam WINDOW - [optional] Child window type (Default is WindowBase)
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
    // WindowBase::onCreate
    //! Called during window creation to modify window parameters and create child windows
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual LResult  onCreate(CreateWindowEventArgs<encoding>& args) 
    { 
      // [Handled] Accept parameters
      return 0; 
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::onCommand
    //! Called in response to a command raised by menu or accelerator (ie. WM_COMMAND)
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Message result and routing
    //! 
    //! \throw wtl::logic_error - Gui command not recognised
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual LResult  onCommand(CommandEventArgs<encoding>& args) 
    { 
      // Execute associated command
      execute(args.Ident);

      // Handled
      return 0;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::onPaint
    //! Called to paint the client area of the window
    //! 
    //! \param[in,out] args - Message arguments containing drawing data
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual LResult  onPaint(PaintWindowEventArgs<encoding>& args) 
    { 
      // [Handled] Validate client area
      return 0; 
    }
  
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::post
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
    // WindowBase::route
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
        case WindowMessage::CREATE:           ret = Create.raise(CreateWindowEventArgs<encoding>(w,l));             break;
        case WindowMessage::CLOSE:            ret = Close.raise();                                                          break;
        case WindowMessage::DESTROY:          ret = Destroy.raise();                                                        break;
        case WindowMessage::SHOWWINDOW:       ret = Show.raise(ShowWindowEventArgs<encoding>(w,l));                 break;
        case WindowMessage::WINDOWPOSCHANGED: ret = Repositioned.raise(PositionChangedEventArgs<encoding>(w,l));    break;

        // [COMMAND] Reflect control events. Raise Gui events.
        case WindowMessage::COMMAND:  
          if (l != 0)
            // [CONTROL] Reflect to sender
            ret = CtrlCommandEventArgs<encoding>(w,l).reflect();
          else
            // [ACTION] Raise event (Default executes the appropriate command object)
            ret = Command.raise(CommandEventArgs<encoding>(w,l));
          break;

        // [NOTIFY] Reflect to sender
        case WindowMessage::NOTIFY:  
          ret = CtrlNotifyEventArgs<encoding>(w,l).reflect();   
          break;

        // [OWNER-DRAW] Reflect to sender
        case WindowMessage::DRAWITEM:       
          // [CONTROL] Reflect to originator control
          if (w != 0)
            ret = OwnerDrawCtrlEventArgs<encoding>(w,l).reflect();

          // [MENU] Raise menu's OwnerDraw event
          else 
            ret = Menu.OwnerDraw.raise(OwnerDrawMenuEventArgs<encoding>(w,l));
          break;
        
        // [OWNER-MEASURE] Reflect to sender
        case WindowMessage::MEASUREITEM: 
          // [CONTROL] Reflect to originator
          if (w != 0)
            ret = OwnerMeasureCtrlEventArgs<encoding>(find(window_id(w)).handle(),w,l).reflect();

          // [MENU] Raise associated menu event
          else 
            ret = Menu.OwnerMeasure.raise(OwnerMeasureMenuEventArgs<encoding>(Handle,w,l));
          break;

        // [PAINT] Avoid instantiating arguments if event is empty (thereby leaving update region invalidated)
        case WindowMessage::PAINT:          
          if (!Paint.empty())
            ret = Paint.raise(PaintWindowEventArgs<encoding>(Handle,w,l));       
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
            ret.Result = getFunc<char_t>(::CallWindowProcA,::CallWindowProcW)(wnd.WndProc.Native, Handle, enum_cast(message), w, l);
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
        cdebug << exception_log(HERE,e,"Unable to route message") << endl;
        
        // [ERROR] Unhandled
        return MsgRoute::Unhandled;
      }
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::send
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
    // WindowBase::show
    //! Show or hide the window
    //! 
    //! \param[in] mode - Display method
    /////////////////////////////////////////////////////////////////////////////////////////
    void show(ShowWindowFlags mode = ShowWindowFlags::Show)
    {
      ::ShowWindow(Handle, enum_cast(mode));
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::update
    //! Updates the window (Sends a WM_PAINT message if any portion of the window is invalidated)
    /////////////////////////////////////////////////////////////////////////////////////////
    void update()
    {
      ::UpdateWindow(Handle);
    }
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \var WindowBase::CommandGroups - Collection of all Command groups 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  CommandGroupCollection<ENC>   WindowBase<ENC>::CommandGroups;
  

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \var WindowBase::ActiveWindows - Collection of all WTL windows 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  WindowHandleCollection<ENC>   WindowBase<ENC>::ActiveWindows;

  
} // namespace wtl


#include "wtl/windows/properties/ClientRectPropertyImpl.hpp"    //!< ClientRectProperty
#include "wtl/windows/properties/EnabledPropertyImpl.hpp"       //!< EnabledProperty
#include "wtl/windows/properties/FontPropertyImpl.hpp"          //!< FontProperty
#include "wtl/windows/properties/IdentPropertyImpl.hpp"         //!< IdentProperty
#include "wtl/windows/properties/PositionPropertyImpl.hpp"      //!< PositionProperty
#include "wtl/windows/properties/WindowRectPropertyImpl.hpp"    //!< WindowRectProperty
#include "wtl/windows/properties/SizePropertyImpl.hpp"          //!< SizeProperty
#include "wtl/windows/properties/StylePropertyImpl.hpp"         //!< StyleProperty
#include "wtl/windows/properties/StyleExPropertyImpl.hpp"       //!< StyleExProperty
#include "wtl/windows/properties/TextPropertyImpl.hpp"          //!< TextProperty
#include "wtl/windows/properties/TextLengthPropertyImpl.hpp"    //!< TextLengthProperty
#include "wtl/windows/properties/VisibilityPropertyImpl.hpp"    //!< VisibilityProperty
  
#endif // WTL_WINDOW_BASE_HPP
