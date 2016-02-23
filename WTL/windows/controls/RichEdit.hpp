//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\RichEdit.hpp
//! \brief Encapsulates standard rich-edit controls
//! \date 6 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RICH_EDIT_HPP
#define WTL_RICH_EDIT_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/Control.hpp>                                        //!< Control
#include <wtl/gdi/Theme.hpp>                                              //!< Theme
#include <wtl/windows/controls/properties/RichEditBackColourProperty.h>   //!< RichEditBackColourProperty
#include <wtl/windows/controls/properties/RichEditCharFormatProperty.h>   //!< RichEditCharFormatProperty
#include <wtl/windows/controls/properties/RichEditSelectedTextProperty.h> //!< RichEditSelectedTextProperty
#include <wtl/windows/controls/properties/EditSelectionProperty.h>        //!< EditSelectionProperty

//! \namespace wtl - Windows template library
namespace wtl 
{

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct RichEditLibrary - Initializes the richedit library upon program startup
  //////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct RichEditLibrary
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
   
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // RichEditLibrary::RichEditLibrary
    //! Initializes the RichEditVersion library
    //! 
    //! \param[in] ver - Desired version
    //!
    //! \throw wtl::platform_error - Failed to initialize library
    //////////////////////////////////////////////////////////////////////////////////////////
    RichEditLibrary(RichEditVersion ver = RichEditVersion::v4_0)
    {
      String<ENC> name;

      // Determine library name
      switch (ver)
      {
      case RichEditVersion::v1_0: name = "richedit32.dll";  break;
      default:                    name = "richedit20.dll";  break;
      case RichEditVersion::v4_0: name = "msftedit.dll";    break;
      }

      // Initialize the library
      if (!WinAPI<ENC>::loadLibrary(name.c_str())) 
        throw platform_error(HERE, "Unable to initialize RichEdit library");
    }
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
		DISABLE_COPY(RichEditLibrary);			//!< Singleton type
		DISABLE_MOVE(RichEditLibrary);			//!< Singleton type
		
    // ----------------------------------- STATIC METHODS -----------------------------------
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct RichEdit - Encapsulates the standard rich-edit control
  //! 
  //! \tparam ENC - Character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct RichEdit : Control<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = RichEdit<ENC>;
  
    //! \alias base - Define base type
    using base = Control<ENC>;
    
    //! \alias class_t - Inherit class type
    using class_t = typename base::class_t;
    
    //! \var encoding - Inherit character encoding
    static constexpr Encoding  encoding = base::encoding;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    // Events (TODO)
    
    // Properties (TODO)
    RichEditBackColourProperty<encoding>    BackgroundColour;   //!< Backgrounf colour
    RichEditCharFormatProperty<encoding>    CharacterFormat;    //!< Character formatting
    RichEditSelectedTextProperty<encoding>  SelectedText;       //!< Current text selection range
    EditSelectionProperty<encoding>         SelectionRange;     //!< Current text selection range
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // RichEdit::RichEdit
    //! Creates the window object for a rich-edit control without creating the window handle
    //! 
    //! \param[in] id - Control identifier
    //! 
    //! \throw wtl::platform_error - Unrecognised system window class
    /////////////////////////////////////////////////////////////////////////////////////////
    RichEdit(WindowId id) : base(id), 
                            BackgroundColour(*this),
                            CharacterFormat(*this),
                            SelectionRange(*this),
                            SelectedText(*this)
    {
      // Set properties
      this->Style = WindowStyle::ChildWindow|WindowStyle::TabStop|WindowStyle::VScroll|WindowStyle::Border | RichEditStyle::Left;
      this->StyleEx = WindowStyleEx::ClientEdge;
      
      // Clear paint handlers (Handled by subclass)
      this->Paint.clear();

      // Compile-time subclass the standard rich-edit control
      this->SubClasses.push(getNativeSubClass());
    }

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    DISABLE_COPY(RichEdit);     //!< Cannot be copied
    ENABLE_MOVE(RichEdit);      //!< Can be moved
    ENABLE_POLY(RichEdit);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // RichEdit::registerClass 
    //! Registers the window-class 
    //! 
    //! \param[in] instance - Handle to registering module  [Used only during initial call]
    //! \return const class_t& - Window class 
    //! 
    //! \throw wtl::platform_error - Unable to register window class
    /////////////////////////////////////////////////////////////////////////////////////////
    static const WindowClass<encoding>&  registerClass(::HINSTANCE instance) 
    {
      static String<encoding> name("WTL.RichEdit");
      
      // Define WTL rich-edit window-class
      static WindowClass<encoding>  std(SystemClass::RichEdit);    //!< Lookup standard rich-edit window-class
      static WindowClass<encoding>  ctrl(instance,
                                         name.c_str(),
                                         std.Style,
                                         base::WndProc,           //!< Replace the window procedure 'Compile-time subclass'
                                         std.Menu,
                                         std.Cursor,
                                         std.Background,
                                         std.SmallIcon,
                                         std.LargeIcon,
                                         std.ClassStorage,
                                         std.WindowStorage);    

      // Return WTL rich-edit class
      return ctrl;
    }
    
  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // RichEdit::getNativeSubClass 
    //! Get the window procedure for the standard rich-edit
    //! 
    //! \return SubClass - SubClass representing the window procedure of the standard rich-edit
    /////////////////////////////////////////////////////////////////////////////////////////
    static SubClass getNativeSubClass() 
    {
      static WindowClass<encoding>  std(SystemClass::RichEdit);    //!< Lookup standard rich-edit window-class
      
      // Return native window proc
      return { SubClass::WindowType::Native, std.WndProc };
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // RichEdit::send
    //! Sends a message to the window
    //! 
    //! \tparam WM - Window Message 
    //!
    //! \param[in] w- [optional] First parameter
    //! \param[in] l - [optional] Second parameter
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    using base::send;
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // RichEdit::send
    //! Sends an edit message to the window
    //! 
    //! \tparam EM - Edit Message 
    //!
    //! \param[in] w- [optional] First parameter
    //! \param[in] l - [optional] Second parameter
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    template <EditMessage EM> 
    LResult send(::WPARAM w = 0, ::LPARAM l = 0)
    {
      return send_message<encoding>(EM, this->Handle, w, l);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // RichEdit::send
    //! Sends a rich-edit message to the window
    //! 
    //! \tparam EM - RichEdit Message 
    //!
    //! \param[in] w- [optional] First parameter
    //! \param[in] l - [optional] Second parameter
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    template <RichEditMessage EM> 
    LResult send(::WPARAM w = 0, ::LPARAM l = 0)
    {
      return send_message<encoding>(EM, this->Handle, w, l);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // RichEdit::wndclass const
    //! Get the window class
    //! 
    //! \return const class_t& - Shared window class
    /////////////////////////////////////////////////////////////////////////////////////////
    const WindowClass<encoding>& wndclass() const override
    {
      return registerClass(nullptr);
    }
    
  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // RichEdit::route
    //! Routes messages to an instance's handlers (This is the 'Instance window procedure')
    //!
    //! \param[in] message - Window message identifier
    //! \param[in] w - [optional] First message parameter
    //! \param[in] l - [optional] Second message parameter
    //! \return LResult - Message routing and result 
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult route(WindowMessage message, WPARAM w, LPARAM l) override
    {
      try
      {
        LResult ret;       //!< Message result, defaults to unhandled

        // Examine message
        switch (message)
        {
        // [COMMAND (REFLECTED)] Raise associated event
        case WindowMessage::ReflectCommand:  
          // Extract notification
          switch (static_cast<RichEditNotification>(ControlEventArgs<encoding,WindowMessage::Command>(w,l).Message))
          {
          case RichEditNotification::Change:      /* TODO: Raise notification */            break;
          case RichEditNotification::Update:      /* TODO: Raise notification */            break;
          case RichEditNotification::HScroll:     /* TODO: Raise notification */            break;
          case RichEditNotification::VScroll:     /* TODO: Raise notification */            break;
          }
          break;
        }

        // [UNHANDLED] Pass to default window procedure
        return base::route(message, w, l);
      }
      catch (std::exception& e)
      {
        cdebug << caught_exception("Unable to route message", HERE, e);
        
        // [ERROR] Unhandled
        return {MsgRoute::Unhandled, -1};
      }
    }
    
  private:
  };
} // namespace wtl

#include <wtl/windows/controls/properties/RichEditBackColourProperty.hpp>     //!< RichEditBackColourProperty
#include <wtl/windows/controls/properties/RichEditCharFormatProperty.hpp>     //!< RichEditCharFormatProperty
#include <wtl/windows/controls/properties/RichEditSelectedTextProperty.hpp>   //!< RichEditSelectedTextProperty
#include <wtl/windows/controls/properties/EditSelectionProperty.hpp>          //!< EditSelectionProperty

#endif // WTL_RICH_EDIT_HPP
