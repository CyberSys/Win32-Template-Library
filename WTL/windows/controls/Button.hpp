////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\Button.hpp
//! \brief Encapsulates button controls
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BUTTON_HPP
#define WTL_BUTTON_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct Button - Encapsulates a button control
  //! 
  //! \tparam ENC - Character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct Button : WindowBase<ENC>
  {
    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias base - Define base type
    using base = WindowBase<ENC>;
    
    //! \var encoding - Inherit character encoding
    static constexpr Encoding  encoding = base::encoding;
    
    // -------------------- REPRESENTATION ---------------------
    
    ButtonClickEvent<encoding>        Click;         //!< Button click
    ButtonGainFocusEvent<encoding>    GainFocus;     //!< Button gained input focus
    ButtonLoseFocusEvent<encoding>    LoseFocus;     //!< Button lost input focus

    // --------------------- CONSTRUCTION ----------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // Button::Button
    //! Creates a standard button control
    //! 
    //! \param[in] instance - Owning instance
    //! 
    //! \throw wtl::platform_error - Unrecognised system window class
    ///////////////////////////////////////////////////////////////////////////////
    Button(HINSTANCE instance) : base(getClass(instance))
    {
      // Painting handled by system window class
      this->Paint.clear();

      // Subclass prior to creation
      SubClasses.push_back(SubClass(WindowType::Native, getSystemWndProc()));
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Button::Button
    //! Creates a custom button control
    //! 
    //! \param[in] &custom - Custom window class
    //! 
    //! \throw wtl::platform_error - Unrecognised system window class
    ///////////////////////////////////////////////////////////////////////////////
    Button(wndclass_t& custom) : base(custom)
    {
      // Remove paint handlers
      this->Paint.clear();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Button::~Button
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~Button()
    {}

    // ------------------------ STATIC -------------------------
  
    ///////////////////////////////////////////////////////////////////////////////
    // Button::getClass 
    //! Get the window class
    //! 
    //! \param[in] instance - Module handle
    //! \return wndclass_t& - Window class 
    ///////////////////////////////////////////////////////////////////////////////
    static wndclass_t& getClass(HINSTANCE instance) 
    {
      static SystemWindowClass<encoding>  std(SystemClass::Button);  //!< Standard system button class
      
      static WindowClass<encoding>  btn(instance,
                                        std.Name,
                                        std.Style,
                                        base::WndProc,   
                                        std.Menu,
                                        std.Cursor,
                                        std.Background,
                                        std.SmallIcon,
                                        std.LargeIcon,
                                        std.ClassStorage,
                                        std.WindowStorage);    //!< Compile-time button subclass

      // Return custom button class
      return btn;
    }
    
  protected:
    ///////////////////////////////////////////////////////////////////////////////
    // Button::getSystemWndProc 
    //! Get the standard button window procedure
    //! 
    //! \return ::WNDPROC - System window procedure
    ///////////////////////////////////////////////////////////////////////////////
    static ::WNDPROC getSystemWndProc() 
    {
      static SystemWindowClass<encoding>  std(SystemClass::Button);  //!< Standard button system window class
      
      // Return window proc
      return std.WndProc;
    }

    // ---------------------- ACCESSORS ------------------------			
    
    // ----------------------- MUTATORS ------------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // Button::create
    //! Create as child window
    //! 
    //! \tparam ENC - Text string encoding
    //! \tparam LEN - Text buffer capacity
    //!
    //! \param[in] const& parent - Parent window
    //! \param[in] const& text - Window text
    //! \param[in] const& rc - Initial position
    //! \param[in] id - Window id (Can be specified using any type)
    //! \param[in] style - Window styles (if unspecified then 'Child' is specified)
    //! \param[in] exStyle - [optional] Extended window styles (If 'none' then window has no extended window styles)
    //! 
    //! \throw wtl::platform_error - Unable to create window
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC, unsigned LEN, typename IDENT = WindowId, typename STYLE = WindowStyle>
    void create(WindowBase<ENC>& parent, const CharArray<ENC,LEN>& text, const Rect<int32>& rc, IDENT id, STYLE style = (STYLE)WindowStyle::Child, WindowStyleEx exStyle = WindowStyleEx::None)
    {
      // Create window
      base::create<ENC,LEN>(parent,text,rc,static_cast<WindowId>(id),WindowStyle::Child|style,exStyle);
      
      // Set initial font
      base::setFont(wtl::StockFont::Window, false);
    }

    
    ///////////////////////////////////////////////////////////////////////////////
    // Button::onControlEvent
    //! Called in response to a reflected message to raise the associated event
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Message result and routing
    ///////////////////////////////////////////////////////////////////////////////
    LResult  onControlEvent(CtrlCommandEventArgs<encoding>& args) override
    { 
      // Raise associated event
      switch (static_cast<ButtonNotification>(args.Message))
      {
      case ButtonNotification::Click:      return Click.raise(ButtonClickEventArgs<encoding>(args));
      case ButtonNotification::SetFocus:   return GainFocus.raise(ButtonGainFocusEventArgs<encoding>(args));
      case ButtonNotification::KillFocus:  return LoseFocus.raise(ButtonLoseFocusEventArgs<encoding>(args));
      }

      return args.unhandled;
    }

  };

}

#endif // WTL_BUTTON_HPP
