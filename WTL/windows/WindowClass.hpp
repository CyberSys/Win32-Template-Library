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
#include "wtl/casts/EnumCast.hpp"               //!< EnumCast
#include "wtl/utils/Handle.hpp"                 //!< Handle
#include "wtl/utils/Default.hpp"                //!< Default
#include "wtl/platform/ResourceId.hpp"          //!< ResourceId
#include "wtl/platform/WindowFlags.hpp"         //!< ClassStyle
#include "wtl/traits/EncodingTraits.hpp"        //!< Encoding
#include "wtl/traits/BrushTraits.hpp"           //!< HBrush
#include "wtl/traits/CursorTraits.hpp"          //!< HCursor
#include "wtl/traits/IconTraits.hpp"            //!< HIcon
#include "wtl/traits/WindowClassTraits.hpp"     //!< HAtom

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
    using native_t = choose_t<ENC,::WNDCLASSEXA,::WNDCLASSEXW>;
    
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
    int32_t     ClassStorage,      //!< Size of class storage, in bytes
                WindowStorage;     //!< Size of window storage, in bytes

    // ------------------------------------- CONSTRUCTION -----------------------------------
    
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
                                 Atom(defvalue<HAtom>()),
                                 Background(defvalue<HBrush>()),
                                 ClassStorage(defvalue(ClassStorage)),
                                 Cursor(defvalue<HCursor>()),
                                 Instance(defvalue<::HINSTANCE>()),
                                 LargeIcon(defvalue<HIcon>()),
                                 Menu(defvalue<resource_t>()),
                                 SmallIcon(defvalue<HIcon>()),
                                 Style(defvalue<ClassStyle>()),
                                 WindowStorage(defvalue(WindowStorage)),
                                 WndProc(defvalue<::WNDPROC>())
    {
      //! \var getClassInfoEx - Functor for 'GetClassInfoEx' 
      static const auto getClassInfoEx = choose<ENC>(::GetClassInfoExA,::GetClassInfoExW);

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
                int32_t clsBytes = 0, 
                int32_t wndBytes = 0) : Atom(instance, name, style, proc, menu, (::HCURSOR)cursor, (::HBRUSH)background, (::HICON)smIcon, (::HICON)bgIcon, clsBytes, wndBytes),
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

    // --------------------------------- COPY, MOVE & DESTROY  ------------------------------
   public:
    DISABLE_COPY(WindowClass);     //!< Cannot be copied
    ENABLE_MOVE(WindowClass);      //!< Can be moved
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowClass::~WindowClass
    //! Unregisters the window class
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~WindowClass() 
    {
      //! \var unregisterClass - Functor for 'UnregisterClass' 
      static const auto unregisterClass = choose<ENC>(::UnregisterClassA,::UnregisterClassW);

      // Unregister window class
      if (unregisterClass(Name, Instance) == False)
        cdebug << Cons::Error << "Unable to unregister window class" << Cons::Endl;
    }
    
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
      case SystemClass::Animate:			return choose<encoding>(ANIMATE_CLASSA,ANIMATE_CLASSW);
      case SystemClass::DateTime:			return choose<encoding>(DATETIMEPICK_CLASSA,DATETIMEPICK_CLASSW);
      case SystemClass::HotKey:			  return choose<encoding>(HOTKEY_CLASSA,HOTKEY_CLASSW);
      case SystemClass::Calendar:			return choose<encoding>(MONTHCAL_CLASSA,MONTHCAL_CLASSW);
      case SystemClass::ProgressBar:	return choose<encoding>(PROGRESS_CLASSA,PROGRESS_CLASSW);
      case SystemClass::CoolBar:			return choose<encoding>(REBARCLASSNAMEA,REBARCLASSNAMEW);
      case SystemClass::StatusBar:		return choose<encoding>(STATUSCLASSNAMEA,STATUSCLASSNAMEW);
      case SystemClass::ToolBar:			return choose<encoding>(TOOLBARCLASSNAMEA,TOOLBARCLASSNAMEW);
      case SystemClass::ToolTip:			return choose<encoding>(TOOLTIPS_CLASSA,TOOLTIPS_CLASSW);
      case SystemClass::TrackBar:			return choose<encoding>(TRACKBAR_CLASSA,TRACKBAR_CLASSW);
      case SystemClass::Spin:			    return choose<encoding>(UPDOWN_CLASSA,UPDOWN_CLASSW);

      //! Common controls
      case SystemClass::Button:			  return choose<encoding>(WC_BUTTONA,WC_BUTTONW);
      case SystemClass::ComboBox:			return choose<encoding>(WC_COMBOBOXA,WC_COMBOBOXW);
      case SystemClass::ComboBoxEx:		return choose<encoding>(WC_COMBOBOXEXA,WC_COMBOBOXEXW);
      case SystemClass::Edit:			    return choose<encoding>(WC_EDITA,WC_EDITW);
      case SystemClass::Header:			  return choose<encoding>(WC_HEADERA,WC_HEADERW);
      case SystemClass::ListBox:			return choose<encoding>(WC_LISTBOXA,WC_LISTBOXW);
      case SystemClass::IpAddress:		return choose<encoding>(WC_IPADDRESSA,WC_IPADDRESSW);
      case SystemClass::Link:			    return choose<encoding>("SysLink",L"SysLink");

      case SystemClass::ListView:			return choose<encoding>(WC_LISTVIEWA,WC_LISTVIEWW);
      case SystemClass::NativeFont:		return choose<encoding>(WC_NATIVEFONTCTLA,WC_NATIVEFONTCTLW);
      case SystemClass::PageScroller:	return choose<encoding>(WC_PAGESCROLLERA,WC_PAGESCROLLERW);
      case SystemClass::ScrollBar:	  return choose<encoding>(WC_SCROLLBARA,WC_SCROLLBARW);
      case SystemClass::Static:			  return choose<encoding>(WC_STATICA,WC_STATICW);
      case SystemClass::Tab:			    return choose<encoding>(WC_TABCONTROLA,WC_TABCONTROLW);
      case SystemClass::TreeView:			return choose<encoding>(WC_TREEVIEWA,WC_TREEVIEWW);
  
      //! Special classes
      case SystemClass::MessageOnly:  return choose<encoding>("Message",L"Message");
      }

      // [ERROR] Unrecognised
      throw invalid_argument(HERE, "Unrecognised system class");
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
  };

      
} // namespace wtl

#endif // WTL_WINDOW_CLASS_HPP
