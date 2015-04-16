////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\WindowClass.hpp
//! \brief Encapsulates Window classes
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_CLASS_HPP
#define WTL_WINDOW_CLASS_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct WindowClass - Encapsulates window class properties
  //! 
  //! \tparam ENC - Character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct WindowClass 
  {
    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias char_t - Character type
    using char_t = encoding_char_t<ENC>;

    //! \alias resource_t - Resource identifier type
    using resource_t = ResourceId<ENC>;

    //! \alias native_t - Type accessor for native ::WNDCLASSEX type
    using native_t = getType<char_t,::WNDCLASSEXA,::WNDCLASSEXW>;

    // --------------------- CONSTRUCTION ----------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowClass::WindowClass
    //! Create a weak reference to a system window class 
    //! 
    //! \param[in] id - System window class
    //! 
    //! \throw wtl::platform_error - Unrecognised system window class
    ///////////////////////////////////////////////////////////////////////////////
    /*WindowClass(SystemClass cls) : WindowClass()
    {}*/

    ///////////////////////////////////////////////////////////////////////////////
    // WindowClass::WindowClass
    //! Create a weak reference to an existing window class 
    //! 
    //! \param[in] id - Registered window class
    //! 
    //! \throw wtl::platform_error - Unrecognised window class
    ///////////////////////////////////////////////////////////////////////////////
    WindowClass(resource_t id) : Background(HBrush::npos),
                                 Cursor(HCursor::npos),
                                 SmallIcon(HIcon::npos),
                                 LargeIcon(HIcon::npos),
                                 ClassStorage(0),
                                 WindowStorage(0),
                                 Style(zero<ClassStyle>::value),
                                 Instance(nullptr),
                                 Name(id),
                                 Menu(resource_t::npos),
                                 Atom(HAtom::npos)
    {
      //! \var getClassInfoEx - Functor for 'GetClassInfoEx' 
      static const auto getClassInfoEx = getFunc<ENC>(::GetClassInfoExA,::GetClassInfoExW);

      native_t wndClass = {sizeof(native_t)};   //!< Window class data

      // Query system class info
      if (!getClassInfoEx(nullptr, id, &wndClass))
        throw platform_error(HERE, "Unrecognised window class");

      // Perform shallow copy all properties
      Background    = HBrush(wndClass.hbrBackground, AllocType::WeakRef);   //!< WeakRef
      Cursor        = HCursor(wndClass.hCursor, AllocType::WeakRef);        //!< WeakRef
      SmallIcon     = HIcon(wndClass.hIconSm, AllocType::WeakRef);          //!< WeakRef
      LargeIcon     = HIcon(wndClass.hIcon, AllocType::WeakRef);            //!< WeakRef
      Instance      = wndClass.hInstance;
      Style         = enum_cast<ClassStyle>(wndClass.style);
      ClassStorage  = wndClass.cbClsExtra;
      WindowStorage = wndClass.cbWndExtra;
      Name          = wndClass.lpszClassName;
      Menu          = wndClass.lpszMenuName;
      
      // Shallow copy class atom
      if (Name.isOrdinal())
        Atom = HAtom(Name.toOrdinal(), AllocType::WeakRef);   //!< WeakRef
    }

    ///////////////////////////////////////////////////////////////////////////////
    // WindowClass::WindowClass
    //! Registers a new window class
    //! 
    //! \param[in] instance - Instance handle
    //! \param[in] name - Class name
    //! \param[in] style - Window style
    //! \param[in] proc - Window procedure
    //! \param[in] menu - Menu resource
    //! \param[in] const& cursor - Window cursor
    //! \param[in] const& background - Background brush
    //! \param[in] const& smIcon - Small icon
    //! \param[in] const& bgIcon - Large icon 
    //! \param[in] clsBytes - Size of class storage, if any
    //! \param[in] wndBytes - Size of handle storage, if any
    //! 
    //! \throw wtl::platform_error - Unable to register window class
    ///////////////////////////////////////////////////////////////////////////////
    WindowClass(HINSTANCE instance, 
                resource_t name,
                ClassStyle style, 
                WNDPROC proc, 
                resource_t menu, 
                const HCursor& cursor, 
                const HBrush& background, 
                const HIcon& smIcon, 
                const HIcon& bgIcon, 
                int32 clsBytes = 0, 
                int32 wndBytes = 0)
      : Background(background),
        Cursor(cursor),
        SmallIcon(smIcon),
        LargeIcon(bgIcon),
        ClassStorage(clsBytes),
        WindowStorage(wndBytes),
        Style(style),
        Instance(instance),
        Name(name),
        Menu(menu),
        Atom(instance, name, style, proc, menu, (HCURSOR)cursor, (HBRUSH)background, (HICON)smIcon, (HICON)bgIcon, clsBytes, wndBytes)
    {}
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowClass::~WindowClass
    //! Unregisters the window class
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~WindowClass() 
    {
      //! \var unregisterClass - Functor for 'UnregisterClass' 
      static const auto unregisterClass = getFunc<ENC>(::UnregisterClassA,::UnregisterClassW);

      // Unregister window class
      if (unregisterClass(Name, Instance) == FALSE)
        cdebug << Cons::Error << "Unable to unregister window class" << Cons::Endl;
    }

    // Deep copy semantics
    WindowClass(const WindowClass&) = default;
    WindowClass(WindowClass&&) = default;

    // ------------------------ STATIC -------------------------
    
    // ---------------------- ACCESSORS ------------------------			
    
    // ----------------------- MUTATORS ------------------------
    
    // -------------------- REPRESENTATION ---------------------
    
    HINSTANCE   Instance;          //!< Registered module
    ClassStyle  Style;             //!< Class styles
    HAtom       Atom;              //!< Registered class atom
    HBrush      Background;        //!< Background brush
    HCursor     Cursor;            //!< Cursor 
    resource_t  Name,              //!< Class name
                Menu;              //!< Window menu name
    HIcon       SmallIcon,         //!< Small icon
                LargeIcon;         //!< Large icon
    int32       ClassStorage,      //!< Size of class storage, in bytes
                WindowStorage;     //!< Size of window storage, in bytes
  };

  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct system_class - Defines the names of system window classes
  //! 
  //! \tparam CHAR - Character type
  //! \tparam S - System class
  ///////////////////////////////////////////////////////////////////////////////
  template <typename CHAR, SystemClass S> 
  struct system_class 
  { 
    //! \alias char_t - Define character type
    using char_t = CHAR;

    //! \var name - System window class name
    static const char_t name[];    
  };

  
  //! \var system_class<wchar_t>::value - Defines standard controls window class names
  template <> struct system_class<wchar_t,SystemClass::Animate>       { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::DateTime>      { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::HotKey>        { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::Calendar>      { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::ProgressBar>   { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::CoolBar>       { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::StatusBar>     { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::ToolBar>       { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::ToolTip>       { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::TrackBar>      { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::Spin>          { static const wchar_t name[];     };

  //! \var system_class<wchar_t>::value - Defines common control window class names
  template <> struct system_class<wchar_t,SystemClass::Button>        { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::ComboBox>      { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::ComboBoxEx>    { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::Edit>          { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::Header>        { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::ListBox>       { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::IpAddress>     { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::Link>          { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::ListView>      { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::NativeFont>    { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::PageScroller>  { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::ScrollBar>     { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::Static>        { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::Tab>           { static const wchar_t name[];     };
  template <> struct system_class<wchar_t,SystemClass::TreeView>      { static const wchar_t name[];     };


  ///////////////////////////////////////////////////////////////////////////////
  //! \struct system_class - Defines the names of system window classes
  //! 
  //! \tparam CHAR - Character type
  //! \tparam CLS - System class
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, SystemClass CLS> 
  struct SystemWindowClass : WindowClass<ENC>
  { 
    //! \alias base - Define base type
    using base = WindowClass<ENC>;

    ///////////////////////////////////////////////////////////////////////////////
    // SystemWindowClass::SystemWindowClass
    //! Create a weak reference to a system window class 
    //! 
    //! \param[in] id - System window class
    //! 
    //! \throw wtl::platform_error - Unrecognised system window class
    ///////////////////////////////////////////////////////////////////////////////
    SystemWindowClass() : base(system_class<encoding_char_t<ENC>,CLS>::name)
    {}

  };


}

#endif // WTL_WINDOW_CLASS_HPP
