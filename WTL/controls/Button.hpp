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
#include <wtl/windows/WindowBase.hpp>                     //!< WindowBase
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
  struct Button : WindowBase<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = Button<ENC>;
  
    //! \alias base - Define base type
    using base = WindowBase<ENC>;
    
    //! \alias wndclass_t - Inherit class type
    using wndclass_t = typename base::wndclass_t;
    
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
    //! Creates the window object for a standard button control (without creating the window handle)
    //! 
    //! \param[in] instance - Owning instance
    //! 
    //! \throw wtl::platform_error - Unrecognised system window class
    /////////////////////////////////////////////////////////////////////////////////////////
    Button(::HINSTANCE instance) : base(getClass(instance)),
                                   Icon(*this),
                                   State(*this)
    {
      // Set properties
      this->Style = WindowStyle::ChildWindow | ButtonStyle::PushButton|ButtonStyle::Centre|ButtonStyle::Notify|ButtonStyle::OwnerDraw;

      // Clear paint handlers (Painting handled by system window class)
      this->Paint.clear();

      // Owner draw handler
      OwnerDraw += new OwnerDrawCtrlEventHandler<encoding>(this, &Button::onOwnerDraw);
      OwnerMeasure += new OwnerMeasureCtrlEventHandler<encoding>(this, &Button::onOwnerMeasure);

      // Invalidate button on mouse enter/leave
      this->MouseEnter += new MouseEnterEventHandler<encoding>(this, &Button::onMouseEnter);
      this->MouseLeave += new MouseLeaveEventHandler<encoding>(this, &Button::onMouseLeave);

      // Place standard Subclass prior to creation
      this->SubClasses.push_back(getNativeSubClass());
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::Button
    //! Creates the window object for a custom button control (without creating the window handle)
    //! 
    //! \param[in] &custom - Custom window class
    //! 
    //! \throw wtl::platform_error - Unrecognised system window class
    /////////////////////////////////////////////////////////////////////////////////////////
    Button(wndclass_t& custom) : base(custom)
    {
      // Set properties
      this->Style = WindowStyle::ChildWindow | ButtonStyle::Centre|ButtonStyle::Notify|ButtonStyle::OwnerDraw;

      // Remove paint handlers
      this->Paint.clear();
    }

    // -------------------------------- COPY, MOVE & DESTROY  -------------------------------
  public:
    DISABLE_COPY(Button);     //!< Cannot be copied
    ENABLE_MOVE(Button);      //!< Can be moved
    ENABLE_POLY(Button);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------
  
    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::getClass 
    //! Get the default window-class for WTL buttons
    //! 
    //! \param[in] instance - Module handle
    //! \return wndclass_t& - Window class 
    /////////////////////////////////////////////////////////////////////////////////////////
    static wndclass_t& getClass(::HINSTANCE instance) 
    {
      static wndclass_t  std(SystemClass::Button);    //!< Lookup standard button windowclass
      
      // Define WTL button window-class
      static wndclass_t  btn(instance,
                             std.Name,   
                             std.Style,
                             base::WndProc,         //!< Replace the window procedure 'Compile-time subclass'
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
      static wndclass_t  std(SystemClass::Button);    //!< Lookup standard button window-class
      
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
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual LResult  onOwnerDraw(OwnerDrawCtrlEventArgs<encoding>& args) 
    { 
      // debug
      cdebug << object_info(__func__, "Ident", args.Ident, "Action",args.Action, "State",args.State) << endl;

      try
      {        
        Theme theme(this->handle(), L"Button");
        RectL rc = args.Rect;

        // Determine state
        PUSHBUTTONSTATES state = (!this->Enabled                          ? PBS_DISABLED 
                                 : args.State && OwnerDrawState::Selected ? PBS_PRESSED 
                                 : this->isMouseOver()                    ? PBS_HOT : PBS_NORMAL);
        
        // Draw background
        theme.fill(args.Graphics, BP_PUSHBUTTON, state, args.Rect);

        // Pressed: Offset drawing rect
        if (state == PBS_PRESSED)
          rc += PointL(1,1);

        // Draw icon
        if (Icon.exists()) 
        {
          const SizeL iconSize(32,32);

          RectL iconRect = rc.arrange(iconSize, {RectL::FromLeft,::GetSystemMetrics(SM_CXEDGE)}, RectL::Centre);
          
          args.Graphics.draw(Icon, iconRect);
          rc.Left += iconSize.Width;
        }

        // Draw text
        DrawTextFlags flags = DrawTextFlags::SingleLine|DrawTextFlags::VCentre|DrawTextFlags::Centre;
        theme.write(args.Graphics, BP_PUSHBUTTON, state, this->Text(), rc, flags);
      }
      catch (const std::exception&)
      {
      }

      // Handle message
      return {MsgRoute::Handled, 0};
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Button::onOwnerMeasure
    //! Called in response to a reflected 'owner measure' message to 
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Routing indicating message was handled
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
