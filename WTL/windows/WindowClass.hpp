//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\WindowClass.hpp
//! \brief Encapsulates Window classes
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_CLASS_HPP
#define WTL_WINDOW_CLASS_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct WindowClass - Encapsulates window class properties
  //! 
  //! \tparam ENC - Character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct WindowClass 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias char_t - Character type
    using char_t = encoding_char_t<ENC>;

    //! \alias resource_t - Resource identifier type
    using resource_t = ResourceId<ENC>;

    //! \alias native_t - Type accessor for native ::WNDCLASSEX type
    using native_t = getType<char_t,::WNDCLASSEXA,::WNDCLASSEXW>;
    
    //! \var encoding - Define window character encoding
    static constexpr Encoding encoding = ENC;

    // ----------------------------------- REPRESENTATION -----------------------------------
    
    ::HINSTANCE Instance;          //!< Registered module
    ::WNDPROC   WndProc;           //!< Window procedure
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

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowClass::WindowClass
    //! Create a weak reference to a system window class 
    //! 
    //! \param[in] id - System window class
    //! 
    //! \throw wtl::invalid_argument - Unrecognised system window class
    //! \throw wtl::platform_error - Unable to register class
    /////////////////////////////////////////////////////////////////////////////////////////
    WindowClass(SystemClass cls) : WindowClass(getSystemClassName(cls))
    {
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowClass::WindowClass
    //! Create a weak reference to an existing window class 
    //! 
    //! \param[in] id - Registered window class
    //! 
    //! \throw wtl::platform_error - Unable to register class
    /////////////////////////////////////////////////////////////////////////////////////////
    WindowClass(resource_t id) : Name(id),
                                 Atom(default<HAtom>()),
                                 Background(default<HBrush>()),
                                 ClassStorage(default(ClassStorage)),
                                 Cursor(default<HCursor>()),
                                 Instance(default<::HINSTANCE>()),
                                 LargeIcon(default<HIcon>()),
                                 Menu(default<resource_t>()),
                                 SmallIcon(default<HIcon>()),
                                 Style(default<ClassStyle>()),
                                 WindowStorage(default(WindowStorage)),
                                 WndProc(default<::WNDPROC>())
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
      WndProc       = wndClass.lpfnWndProc; 
      Style         = enum_cast<ClassStyle>(wndClass.style);
      ClassStorage  = wndClass.cbClsExtra;
      WindowStorage = wndClass.cbWndExtra;
      Name          = wndClass.lpszClassName;
      Menu          = wndClass.lpszMenuName;
      
      // Shallow copy class atom
      if (Name.isOrdinal())
        Atom = HAtom(Name.toOrdinal(), AllocType::WeakRef);   //!< WeakRef
    }

    /////////////////////////////////////////////////////////////////////////////////////////
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
    /////////////////////////////////////////////////////////////////////////////////////////
    WindowClass(::HINSTANCE instance, 
                resource_t name,
                ClassStyle style, 
                ::WNDPROC proc, 
                resource_t menu, 
                const HCursor& cursor, 
                const HBrush& background, 
                const HIcon& smIcon, 
                const HIcon& bgIcon, 
                int32 clsBytes = 0, 
                int32 wndBytes = 0) : Atom(instance, name, style, proc, menu, (::HCURSOR)cursor, (::HBRUSH)background, (::HICON)smIcon, (::HICON)bgIcon, clsBytes, wndBytes),
                                      Background(background),
                                      ClassStorage(clsBytes),
                                      Cursor(cursor),
                                      Instance(instance),
                                      LargeIcon(bgIcon),              
                                      Menu(menu),
                                      Name(name),
                                      SmallIcon(smIcon),
                                      Style(style),
                                      WindowStorage(wndBytes),
                                      WndProc(proc)
    {}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowClass::~WindowClass
    //! Unregisters the window class
    /////////////////////////////////////////////////////////////////////////////////////////
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

    // ----------------------------------- STATIC METHODS -----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowClass::getSystemClassName
    //! Query name of a system window class
    //! 
    //! \param[in] cls - System class
    //! \return resource_t - Resource id
    //! 
    //! \throw wtl::invalid_argument - Unrecognised system window class
    /////////////////////////////////////////////////////////////////////////////////////////
    static resource_t  getSystemClassName(SystemClass cls)
    {
      // Query class
      switch (cls)
      {
      //! Standard controls
      case SystemClass::Animate:			return getValue<encoding>(ANIMATE_CLASSA,ANIMATE_CLASSW);
      case SystemClass::DateTime:			return getValue<encoding>(DATETIMEPICK_CLASSA,DATETIMEPICK_CLASSW);
      case SystemClass::HotKey:			  return getValue<encoding>(HOTKEY_CLASSA,HOTKEY_CLASSW);
      case SystemClass::Calendar:			return getValue<encoding>(MONTHCAL_CLASSA,MONTHCAL_CLASSW);
      case SystemClass::ProgressBar:	return getValue<encoding>(PROGRESS_CLASSA,PROGRESS_CLASSW);
      case SystemClass::CoolBar:			return getValue<encoding>(REBARCLASSNAMEA,REBARCLASSNAMEW);
      case SystemClass::StatusBar:		return getValue<encoding>(STATUSCLASSNAMEA,STATUSCLASSNAMEW);
      case SystemClass::ToolBar:			return getValue<encoding>(TOOLBARCLASSNAMEA,TOOLBARCLASSNAMEW);
      case SystemClass::ToolTip:			return getValue<encoding>(TOOLTIPS_CLASSA,TOOLTIPS_CLASSW);
      case SystemClass::TrackBar:			return getValue<encoding>(TRACKBAR_CLASSA,TRACKBAR_CLASSW);
      case SystemClass::Spin:			    return getValue<encoding>(UPDOWN_CLASSA,UPDOWN_CLASSW);

      //! Common controls
      case SystemClass::Button:			  return getValue<encoding>(WC_BUTTONA,WC_BUTTONW);
      case SystemClass::ComboBox:			return getValue<encoding>(WC_COMBOBOXA,WC_COMBOBOXW);
      case SystemClass::ComboBoxEx:		return getValue<encoding>(WC_COMBOBOXEXA,WC_COMBOBOXEXW);
      case SystemClass::Edit:			    return getValue<encoding>(WC_EDITA,WC_EDITW);
      case SystemClass::Header:			  return getValue<encoding>(WC_HEADERA,WC_HEADERW);
      case SystemClass::ListBox:			return getValue<encoding>(WC_LISTBOXA,WC_LISTBOXW);
      case SystemClass::IpAddress:		return getValue<encoding>(WC_IPADDRESSA,WC_IPADDRESSW);
      case SystemClass::Link:			    return getValue<encoding>("SysLink",L"SysLink");

      case SystemClass::ListView:			return getValue<encoding>(WC_LISTVIEWA,WC_LISTVIEWW);
      case SystemClass::NativeFont:		return getValue<encoding>(WC_NATIVEFONTCTLA,WC_NATIVEFONTCTLW);
      case SystemClass::PageScroller:	return getValue<encoding>(WC_PAGESCROLLERA,WC_PAGESCROLLERW);
      case SystemClass::ScrollBar:	  return getValue<encoding>(WC_SCROLLBARA,WC_SCROLLBARW);
      case SystemClass::Static:			  return getValue<encoding>(WC_STATICA,WC_STATICW);
      case SystemClass::Tab:			    return getValue<encoding>(WC_TABCONTROLA,WC_TABCONTROLW);
      case SystemClass::TreeView:			return getValue<encoding>(WC_TREEVIEWA,WC_TREEVIEWW);
  
      //! Special classes
      case SystemClass::MessageOnly:  return getValue<encoding>("Message",L"Message");
      }

      // [ERROR] Unrecognised
      throw invalid_argument(HERE, "Unrecognised system class");
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
  };

  

}

#endif // WTL_WINDOW_CLASS_HPP
