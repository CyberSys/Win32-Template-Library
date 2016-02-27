//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\edit\Edit.hpp
//! \brief Encapsulates standard edit controls
//! \date 1 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EDIT_HPP
#define WTL_EDIT_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/Control.hpp>                                    //!< Control
#include <wtl/gdi/Theme.hpp>                                          //!< Theme
#include <wtl/windows/controls/edit/EditConstants.hpp>                //!< (Constants)
#include <wtl/windows/controls/edit/EditLinesCollection.hpp>          //!< EditLinesCollection
#include <wtl/windows/controls/edit/EditChangedEvent.hpp>             //!< EditChangedEvent
#include <wtl/windows/controls/edit/EditUpdatedEvent.hpp>             //!< EditUpdatedEvent
#include <wtl/windows/controls/edit/EditMaxTextEvent.hpp>             //!< EditMaxTextEvent
#include <wtl/windows/controls/edit/EditReadOnlyProperty.h>           //!< EditReadOnlyProperty
#include <wtl/windows/controls/edit/EditSelectionProperty.h>          //!< EditSelectionProperty
#include <wtl/windows/controls/edit/EditModifiedProperty.h>           //!< EditModifiedProperty

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
    
    //! \var encoding - Inherit character encoding
    static constexpr Encoding  encoding = base::encoding;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    // Events 
    EditChangedEvent<encoding>       Changed;           //!< Text has changed
    EditUpdatedEvent<encoding>       Updated;           //!< Text has been updated
    EditMaxTextEvent<encoding>       MaxText;           //!< Text limit reached

    // Properties 
    EditLinesCollection<encoding>    Lines;             //!< Lines collection
    EditModifiedProperty<encoding>   Modified;          //!< Whether text has been modified
    EditReadOnlyProperty<encoding>   ReadOnly;          //!< Whether text is read-only
    EditSelectionProperty<encoding>  SelectionRange;    //!< Current text selection range

    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Edit::Edit
    //! Creates the window object for a edit control without creating the window handle
    //! 
    //! \param[in] id - Control identifier
    //! 
    //! \throw wtl::platform_error - Unrecognised system window class
    /////////////////////////////////////////////////////////////////////////////////////////
    Edit(WindowId id) : base(id), 
                        Lines(*this),
                        Modified(*this),
                        ReadOnly(*this),
                        SelectionRange(*this)
    {
      static const WindowClass<encoding>  std(SystemClass::Edit);    //!< Lookup standard edit window-class

      // Set properties
      this->Style = WindowStyle::ChildWindow|WindowStyle::TabStop|WindowStyle::VScroll|WindowStyle::Border | EditStyle::Left;
      this->StyleEx = WindowStyleEx::ClientEdge;
      
      // Clear paint handlers (Handled by subclass)
      this->Paint.clear();

      // Attempt to provide custom background
      this->Colourize += new ColourizeEventHandler<encoding>(this, &Edit::onColourize);

      // Compile-time subclass the standard edit control
      this->SubClasses += std.WndProc;
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
    //! Registers the window-class on the first call, retrieves the pre-registered class upon subsequent calls
    //! 
    //! \param[in] instance - [optional] Module for which to register the window class [Necessary on first call]
    //! \return const WindowClass<encoding>& - Registered window class 
    //! 
    //! \throw wtl::platform_error - Unable to register window class
    /////////////////////////////////////////////////////////////////////////////////////////
    static const WindowClass<encoding>&  registerClass(::HINSTANCE instance = nullptr)
    {
      static const String<encoding> name("WTL.Edit");
      
      // Define WTL edit window-class
      static const WindowClass<encoding>  std(SystemClass::Edit);    //!< Lookup standard edit window-class
      static const WindowClass<encoding>  ctrl(instance,
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
      return send_message<encoding>(EM, this->Handle, w, l);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Edit::wndclass const
    //! Get the window class
    //! 
    //! \return const WindowClass<encoding>& - Shared window class
    /////////////////////////////////////////////////////////////////////////////////////////
    const WindowClass<encoding>& wndclass() const override
    {
      return registerClass();
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
        LResult ret;        //!< Message result, defaults to unhandled
        uint16_t code;      //!< Notification code

        // Examine message
        switch (message)
        {
        // [COMMAND (REFLECTED)] Raise associated event
        case WindowMessage::Command | WindowMessage::Reflect:
          // Extract notification
          code = ControlEventArgs<encoding,WindowMessage::Command>(w,l).Message;
          switch (static_cast<EditNotification>(code))
          {
          case EditNotification::Change:   return Changed.raise(EditChangedEventArgs<encoding>(w,l)); 
          case EditNotification::Update:   return Updated.raise(EditUpdatedEventArgs<encoding>(w,l)); 
          case EditNotification::HScroll:     /* TODO: Raise notification */            break;
          case EditNotification::VScroll:     /* TODO: Raise notification */            break;
          case EditNotification::MaxText:  return MaxText.raise(EditMaxTextEventArgs<encoding>(w,l)); 
          }
          break;

        // [CTLCOLOR (REFLECTED)] Raise 'Colourize' event
        case WindowMessage::CtrlColourEdit | WindowMessage::Reflect:    // (Ctrl is enabled)
        case WindowMessage::CtrlColourStatic | WindowMessage::Reflect:  // (Ctrl is disabled)
          if (!this->Colourize.empty())
            return this->Colourize.raise( ColourizeEventArgs<encoding>(w,l) ); 
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
    /////////////////////////////////////////////////////////////////////////////////////////
    // Edit::onColourize
    //! Called to provide a background brush and set drawing colours
    //! 
    //! \param[in] args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onColourize(ColourizeEventArgs<encoding>& args) 
    {
      // Handle message
      return {MsgRoute::Handled, opaque_cast(StockBrush::Leaves.get()) };
    }
    
  };

  
} // namespace wtl

#include <wtl/windows/controls/edit/EditReadOnlyProperty.hpp>      //!< EditReadOnlyProperty
#include <wtl/windows/controls/edit/EditSelectionProperty.hpp>     //!< EditSelectionProperty
#include <wtl/windows/controls/edit/EditModifiedProperty.hpp>      //!< EditModifiedProperty

#endif // WTL_EDIT_HPP
