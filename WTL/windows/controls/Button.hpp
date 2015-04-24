////////////////////////////////////////////////////////////////////////////////
//! \file wtl\controls\Button.hpp
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
    }

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
      static SystemWindowClass<encoding,SystemClass::Button>  wc;  //!< Standard button system window class
      
      // Return singleton
      return wc;
    }

    // ---------------------- ACCESSORS ------------------------			
    
    // ----------------------- MUTATORS ------------------------
    
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
    //template <Encoding ENC, unsigned LEN, typename IDENT> 
    //void create2(window_t& parent, const CharArray<ENC,LEN>& text, const Rect<int32>& rc, IDENT id, ButtonStyle style = wtl::ButtonStyle::Centre|wtl::ButtonStyle::Notify, WindowStyleEx exStyle = WindowStyleEx::None)
    //{
    //  // Create window
    //  base::create<ENC,LEN>(parent,text,rc,static_cast<WindowId>(id),WindowStyle::Child|style,exStyle);

    //  // Set initial font
    //  base::setFont(wtl::StockFont::Window, false);
    //}

    void setImage(HBITMAP bmp)
    {
      int32 dummy;
      Button_SetImageList(this->Handle, &dummy);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Button::onControlEvent
    //! Called in response to events from child controls (ie. WM_COMMAND)
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Message result and routing
    ///////////////////////////////////////////////////////////////////////////////
    virtual LResult  onControlEvent(ControlEventArgs<encoding>& args) 
    { 
      switch (static_cast<ButtonEvent>(args.Message))
      {
      case ButtonEvent::Click:      return Click.raise();
      case ButtonEvent::Disabled:
      case ButtonEvent::Pushed:
      }

      return args.unhandled();
    }

    // -------------------- REPRESENTATION ---------------------
    
  };

}

#endif // WTL_BUTTON_HPP
