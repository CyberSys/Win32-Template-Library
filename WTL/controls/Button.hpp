//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\controls\Button.hpp
//! \brief Encapsulates standard button controls
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BUTTON_HPP
#define WTL_BUTTON_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/Control.hpp>                        //!< Control
#include <wtl/controls/events/ButtonEvents.hpp>           //!< ButtonClickEvent
#include <wtl/controls/properties/ButtonIconProperty.h>   //!< ButtonIconProperty
#include <wtl/controls/properties/ButtonStateProperty.h>  //!< ButtonStateProperty
#include <wtl/gdi/Theme.hpp>                              //!< Theme

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Button - Encapsulates a button control
  //! 
  //! \tparam ENC - Character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct Button : Control<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = Button<ENC>;
  
    //! \alias base - Define base type
    using base = Control<ENC>;
    
    //! \alias class_t - Inherit class type
    using class_t = typename base::class_t;
    
    //! \var encoding - Inherit character encoding
    static constexpr Encoding  encoding = base::encoding;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    ButtonClickEvent<encoding>        Click;         //!< Button click
    //ButtonGainFocusEvent<encoding>    GainFocus;     //!< Button gained input focus
    //ButtonLoseFocusEvent<encoding>    LoseFocus;     //!< Button lost input focus
    OwnerDrawCtrlEvent<encoding>      OwnerDraw;     //!< Owner draw button
    OwnerMeasureCtrlEvent<encoding>   OwnerMeasure;  //!< Measure button for owner draw
    //CustomDrawEvent<encoding>         CustomDraw;    //!< Custom draw

    // Properties
    ButtonIconProperty<encoding>      Icon;          //!< Icon
    ButtonStateProperty<encoding>     State;         //!< State

    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::Button
    //! Creates the window object for a button control without creating the window handle
    //! 
    //! \param[in] id - Control identifier
    //! 
    //! \throw wtl::platform_error - Unrecognised system window class
    /////////////////////////////////////////////////////////////////////////////////////////
    Button(WindowId id) : base(id),
                          Icon(*this),
                          State(*this)
    {
      // Set properties
      this->Style = WindowStyle::ChildWindow | ButtonStyle::PushButton|ButtonStyle::Centre|ButtonStyle::Notify|ButtonStyle::OwnerDraw;
      
      // Clear paint handlers (Handled by subclass)
      this->Paint.clear();

      // Compile-time subclass the standard button control
      this->SubClasses.push_back(getNativeSubClass());

      // Owner draw handlers
      OwnerDraw += new OwnerDrawCtrlEventHandler<encoding>(this, &Button::onOwnerDraw);
      OwnerMeasure += new OwnerMeasureCtrlEventHandler<encoding>(this, &Button::onOwnerMeasure);

      // Mouse handlers (Handles 'hot' notification)
      this->MouseEnter += new MouseEnterEventHandler<encoding>(this, &Button::onMouseEnter);
      this->MouseLeave += new MouseLeaveEventHandler<encoding>(this, &Button::onMouseLeave);
    }

    // -------------------------------- COPY, MOVE & DESTROY  -------------------------------
  public:
    DISABLE_COPY(Button);     //!< Cannot be copied
    ENABLE_MOVE(Button);      //!< Can be moved
    ENABLE_POLY(Button);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::registerClass 
    //! Registers the window-class 
    //! 
    //! \param[in] instance - Handle to registering module  [Used only during initial call]
    //! \return const class_t& - Window class 
    //! 
    //! \throw wtl::platform_error - Unable to register window class
    /////////////////////////////////////////////////////////////////////////////////////////
    static const WindowClass<encoding>&  registerClass(::HINSTANCE instance) 
    {
      static WindowClass<encoding>  std(SystemClass::Button);    //!< Lookup standard button windowclass
      
      // Define WTL button window-class
      static WindowClass<encoding>  btn(instance,
                                        std.Name,   
                                        std.Style,
                                        base::WndProc,           //!< Replace the window procedure 'Compile-time subclass'
                                        std.Menu,
                                        std.Cursor,
                                        std.Background,
                                        std.SmallIcon,
                                        std.LargeIcon,
                                        std.ClassStorage,
                                        std.WindowStorage);    

      // Return WTL button class
      return btn;
    }
    
  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::getNativeSubClass 
    //! Get the window procedure for the standard button
    //! 
    //! \return SubClass - SubClass representing the window procedure of the standard button
    /////////////////////////////////////////////////////////////////////////////////////////
    static SubClass getNativeSubClass() 
    {
      static WindowClass<encoding>  std(SystemClass::Button);    //!< Lookup standard button window-class
      
      // Return native window proc
      return { SubClass::WindowType::Native, std.WndProc };
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::send
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
    // Button::send
    //! Sends a button message to the window
    //! 
    //! \tparam BM - Button Message 
    //!
    //! \param[in] w- [optional] First parameter
    //! \param[in] l - [optional] Second parameter
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    template <ButtonMessage BM> 
    LResult send(::WPARAM w = 0, ::LPARAM l = 0)
    {
      return send_message<encoding,window_msg(BM)>(this->Handle, w, l);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::wndclass const
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
    // Button::route
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
          switch (static_cast<ButtonNotification>(ControlEventArgs<encoding,WindowMessage::Command>(w,l).Message))
          {
          case ButtonNotification::Click:      ret = Click.raise(ButtonClickEventArgs<encoding>(w,l));            break;
          }
          break;

        // [OWNER-DRAW (REFLECTED)] Raise 'Owner Draw' 
        case WindowMessage::ReflectDrawItem: 
          { OwnerDrawCtrlEventArgs<encoding> args(w,l);  
            ret = OwnerDraw.raise(args); }
          break;

        // [OWNER-DRAW (REFLECTED)] Raise 'Owner Measure'
        case WindowMessage::ReflectMeasureItem:  
          { OwnerMeasureCtrlEventArgs<encoding> args(this->Handle,w,l);  
            ret = OwnerMeasure.raise(args); }
          break;
        }

        // [UNHANDLED] Pass to default window procedure
        return base::route(message, w, l);
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
    // Button::onMouseEnter
    //! Invalidate the button when the cursor enters the button
    //! 
    //! \param[in] args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onMouseEnter(MouseEnterEventArgs<encoding> args) 
    {
      //cdebug << __func__ << endl;
      
      // Redraw
      this->invalidate();

      // Handle message
      return {MsgRoute::Handled, 0};
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::onMouseLeave
    //! Invalidate the button when the cursor enters the button
    //! 
    //! \param[in] args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onMouseLeave(MouseLeaveEventArgs<encoding> args) 
    {
      //cdebug << __func__ << endl;

      // Redraw
      this->invalidate();
      
      // Handle message
      return {MsgRoute::Handled, 0};
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::onOwnerDraw
    //! Called in response to a reflected 'owner draw' message to draw the button
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    //!
    //! \throw wtl::platform_error - Unable to draw button
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual LResult  onOwnerDraw(OwnerDrawCtrlEventArgs<encoding>& args) 
    { 
      // debug
      cdebug << object_info(__func__, "Ident", args.Ident, "Action",args.Action, "State",args.State) << endl;

      Theme theme(this->handle(), L"Button");
      RectL rc = args.Rect;

      // Determine state
      PUSHBUTTONSTATES state = (!this->Enabled                           ? PBS_DISABLED 
                                : args.State && OwnerDrawState::Selected ? PBS_PRESSED 
                                : this->isMouseOver()                    ? PBS_HOT : PBS_NORMAL);
        
      // Draw background and edge
      theme.fill(args.Graphics, BP_PUSHBUTTON, state, args.Rect);

      // Pressed: Offset drawing rect
      if (state == PBS_PRESSED)
        rc += PointL(1,1);

      // Draw icon
      if (Icon.exists()) 
      {
        RectL iconRect = rc.arrange(Metrics::WindowIcon, {RectL::FromLeft,Metrics::WindowEdge.Width}, RectL::Centre);
        args.Graphics.draw(Icon, iconRect);

        // Adjust drawing rectangle
        rc.Left += Metrics::WindowIcon.Width + Metrics::WindowEdge.Width;
      }

      // Draw text
      theme.write(args.Graphics, BP_PUSHBUTTON, state, this->Text(), rc, DrawTextFlags::Centre|DrawTextFlags::VCentre|DrawTextFlags::SingleLine);

      // Handle message
      return {MsgRoute::Handled, 0};
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::onOwnerMeasure
    //! Called in response to a reflected 'owner measure' message to 
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    //!
    //! \throw wtl::platform_error - Unable to measure button
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual LResult  onOwnerMeasure(OwnerMeasureCtrlEventArgs<encoding>& args) 
    { 
      // Measure button text
      args.Size = args.Graphics.measure(this->Text());

      // Handle message
      return {MsgRoute::Handled, 0};
    }
  };
} // namespace wtl

#include <wtl/controls/properties/ButtonIconProperty.hpp>      //!< IconProperty
#include <wtl/controls/properties/ButtonStateProperty.hpp>     //!< StateProperty

#endif // WTL_BUTTON_HPP
