//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\Edit.hpp
//! \brief Encapsulates standard edit controls
//! \date 1 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EDIT_HPP
#define WTL_EDIT_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/Control.hpp>                                //!< Control
#include <wtl/gdi/Theme.hpp>                                      //!< Theme

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Edit - Encapsulates the standard edit control
  //! 
  //! \tparam ENC - Character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct Edit : Control<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = Edit<ENC>;
  
    //! \alias base - Define base type
    using base = Control<ENC>;
    
    //! \alias class_t - Inherit class type
    using class_t = typename base::class_t;
    
    //! \var encoding - Inherit character encoding
    static constexpr Encoding  encoding = base::encoding;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    // Events (TODO)
    

    // Properties (TODO)
    

    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Edit::Edit
    //! Creates the window object for a edit control without creating the window handle
    //! 
    //! \param[in] id - Control identifier
    //! 
    //! \throw wtl::platform_error - Unrecognised system window class
    /////////////////////////////////////////////////////////////////////////////////////////
    Edit(WindowId id) : base(id)
    {
      // Set properties
      this->Style = WindowStyle::ChildWindow|WindowStyle::TabStop|WindowStyle::VScroll|WindowStyle::Border | EditStyle::Left;
      this->StyleEx = WindowStyleEx::ClientEdge;
      
      // Clear paint handlers (Handled by subclass)
      this->Paint.clear();

      // Compile-time subclass the standard edit control
      this->SubClasses.push_back(getNativeSubClass());
    }

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    DISABLE_COPY(Edit);     //!< Cannot be copied
    ENABLE_MOVE(Edit);      //!< Can be moved
    ENABLE_POLY(Edit);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Edit::registerClass 
    //! Registers the window-class 
    //! 
    //! \param[in] instance - Handle to registering module  [Used only during initial call]
    //! \return const class_t& - Window class 
    //! 
    //! \throw wtl::platform_error - Unable to register window class
    /////////////////////////////////////////////////////////////////////////////////////////
    static const WindowClass<encoding>&  registerClass(::HINSTANCE instance) 
    {
      static String<encoding> name("WTL.Edit");
      
      // Define WTL edit window-class
      static WindowClass<encoding>  std(SystemClass::Edit);    //!< Lookup standard edit window-class
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

      // Return WTL edit class
      return ctrl;
    }
    
  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Edit::getNativeSubClass 
    //! Get the window procedure for the standard edit
    //! 
    //! \return SubClass - SubClass representing the window procedure of the standard edit
    /////////////////////////////////////////////////////////////////////////////////////////
    static SubClass getNativeSubClass() 
    {
      static WindowClass<encoding>  std(SystemClass::Edit);    //!< Lookup standard edit window-class
      
      // Return native window proc
      return { SubClass::WindowType::Native, std.WndProc };
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Edit::send
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
    // Edit::send
    //! Sends a edit message to the window
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
      return send_message<encoding,window_msg(EM)>(this->Handle, w, l);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Edit::wndclass const
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
    // Edit::route
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
          switch (static_cast<EditNotification>(ControlEventArgs<encoding,WindowMessage::Command>(w,l).Message))
          {
          case EditNotification::Change:      /* TODO: Raise notification */            break;
          case EditNotification::Update:      /* TODO: Raise notification */            break;
          case EditNotification::HScroll:     /* TODO: Raise notification */            break;
          case EditNotification::VScroll:     /* TODO: Raise notification */            break;
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



#endif // WTL_EDIT_HPP
