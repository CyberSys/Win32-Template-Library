//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\ComboBox.hpp
//! \brief Encapsulates standard combobox control
//! \date 23 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BUTTON_HPP
#define WTL_BUTTON_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/Control.hpp>                        //!< Control
#include <wtl/windows/controls/events/ButtonEvents.hpp>           //!< ButtonClickEvent
#include <wtl/windows/controls/properties/ButtonIconProperty.h>   //!< ButtonIconProperty
#include <wtl/windows/controls/properties/ButtonStateProperty.h>  //!< ButtonStateProperty
#include <wtl/gdi/Theme.hpp>                              //!< Theme

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ComboBox - Encapsulates a standard ComboBox control
  //! 
  //! \tparam ENC - Character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ComboBox : Control<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = ComboBox<ENC>;
  
    //! \alias base - Define base type
    using base = Control<ENC>;
    
    //! \alias class_t - Inherit class type
    using class_t = typename base::class_t;
    
    //! \var encoding - Inherit character encoding
    static constexpr Encoding  encoding = base::encoding;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    OwnerDrawCtrlEvent<encoding>      OwnerDraw;     //!< Owner draw button
    OwnerMeasureCtrlEvent<encoding>   OwnerMeasure;  //!< Measure button for owner draw
    
    // Properties
    ButtonIconProperty<encoding>      Icon;          //!< Icon
    ButtonStateProperty<encoding>     State;         //!< State

    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBox::ComboBox
    //! Creates the window object for a button control without creating the window handle
    //! 
    //! \param[in] id - Control identifier
    //! 
    //! \throw wtl::platform_error - Unrecognised system window class
    /////////////////////////////////////////////////////////////////////////////////////////
    ComboBox(WindowId id) : base(id),
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
      OwnerDraw += new OwnerDrawCtrlEventHandler<encoding>(this, &ComboBox::onOwnerDraw);
      OwnerMeasure += new OwnerMeasureCtrlEventHandler<encoding>(this, &ComboBox::onOwnerMeasure);

      // Mouse handlers (Handles 'hot' notification)
      this->MouseEnter += new MouseEnterEventHandler<encoding>(this, &ComboBox::onMouseEnter);
      this->MouseLeave += new MouseLeaveEventHandler<encoding>(this, &ComboBox::onMouseLeave);
    }

    // -------------------------------- COPY, MOVE & DESTROY  -------------------------------
  public:
    DISABLE_COPY(ComboBox);     //!< Cannot be copied
    ENABLE_MOVE(ComboBox);      //!< Can be moved
    ENABLE_POLY(ComboBox);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBox::registerClass 
    //! Registers the window-class 
    //! 
    //! \param[in] instance - Handle to registering module  [Used only during initial call]
    //! \return const class_t& - Window class 
    //! 
    //! \throw wtl::platform_error - Unable to register window class
    /////////////////////////////////////////////////////////////////////////////////////////
    static const WindowClass<encoding>&  registerClass(::HINSTANCE instance) 
    {
      static String<encoding> name("WTL.ComboBox");
      
      // Define WTL button window-class
      static WindowClass<encoding>  std(SystemClass::ComboBox);    //!< Lookup standard button windowclass
      static WindowClass<encoding>  btn(instance,
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

      // Return WTL button class
      return btn;
    }
    
  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBox::getNativeSubClass 
    //! Get the window procedure for the standard button
    //! 
    //! \return SubClass - SubClass representing the window procedure of the standard button
    /////////////////////////////////////////////////////////////////////////////////////////
    static SubClass getNativeSubClass() 
    {
      static WindowClass<encoding>  std(SystemClass::ComboBox);    //!< Lookup standard button window-class
      
      // Return native window proc
      return { SubClass::WindowType::Native, std.WndProc };
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBox::send
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
    // ComboBox::send
    //! Sends a button message to the window
    //! 
    //! \tparam BM - ComboBox Message 
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
    // ComboBox::wndclass const
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
    // ComboBox::route
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
    // ComboBox::onMouseEnter
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
    // ComboBox::onMouseLeave
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
      //return {MsgRoute::Unhandled, 0};
      return {MsgRoute::Unhandled};
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBox::onOwnerDraw
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
      //cdebug << object_info(__func__, "Ident", args.Ident, "Action",args.Action, "State",args.State) << endl;

      Theme theme(this->handle(), L"ComboBox");

      // Determine state
      PUSHBUTTONSTATES state = PBS_NORMAL;
      if (!this->Enabled)
        state = PBS_DISABLED;
      else if (args.State && OwnerDrawState::Selected)
        state = PBS_PRESSED;
      else if (this->isMouseOver())
        state = PBS_HOT;
      
      // Draw background 
      theme.fill(args.Graphics, BP_PUSHBUTTON, state, args.Rect);

      // Query content rect
      RectL rcContent = theme.content(args.Graphics, BP_CHECKBOX, state, args.Rect);

      // Pressed: Offset drawing rect
      if (state == PBS_PRESSED)
        rcContent += PointL(1,1);

      // Draw icon
      if (Icon.exists()) 
      {
        RectL rcIcon = rcContent.arrange(Metrics::WindowIcon, {RectL::FromLeft,Metrics::WindowEdge.Width}, RectL::Centre);
        args.Graphics.draw(Icon, rcIcon);
      }
      
      // Calculate text rectangle
      RectL rcText = rcContent;
      if (Icon.exists()) 
        rcText.Left += Metrics::WindowIcon.Width + Metrics::WindowEdge.Width;

      // Draw text
      theme.write(args.Graphics, BP_PUSHBUTTON, state, this->Text(), rcText, DrawTextFlags::Centre|DrawTextFlags::VCentre|DrawTextFlags::SingleLine);
      
      // [FOCUS] Draw focus rectangle
      if (args.State && OwnerDrawState::Focus)
      {
        RectL rcFocus = rcContent.inflate(-Metrics::WindowEdge);
        args.Graphics.focus(rcFocus);
      }

      // Handle message
      return {MsgRoute::Handled, 0};
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBox::onOwnerMeasure
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

#include <wtl/windows/controls/properties/ButtonIconProperty.hpp>      //!< IconProperty
#include <wtl/windows/controls/properties/ButtonStateProperty.hpp>     //!< StateProperty

#endif // WTL_BUTTON_HPP