//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\radiobutton\RadioButton.hpp
//! \brief Encapsulates the standard radio button control
//! \date 6 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RADIOBUTTON_HPP
#define WTL_RADIOBUTTON_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/Window.hpp>                                         //!< Window
#include <wtl/windows/controls/button/Button.hpp>                         //!< Button
#include <wtl/platform/Metrics.hpp>                                       //!< Metrics
#include <wtl/windows/controls/radiobutton/RadioButtonCheckProperty.h>    //!< RadioButtonCheckProperty

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct RadioButton - Encapsulates a standard radio button control 
  //! 
  //! \tparam ENC - Character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct RadioButton : Button<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = RadioButton<ENC>;
  
    //! \alias base - Define base type
    using base = Button<ENC>;
    
    //! \var encoding - Inherit character encoding
    static constexpr Encoding  encoding = base::encoding;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    // Properties
    RadioButtonCheckProperty<encoding>   Check;        //!< Check state

    // Events
    //RadioButtonCheckedEvent<encoding>    Checked;      //!< Check state changed

    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // RadioButton::RadioButton
    //! Creates the window object for a WTL radio button control without creating the window handle
    //! 
    //! \param[in] id - Control identifier
    //! \param[in] first - Identifier of first control in group
    //! \param[in] last - Identifier of last control in group
    /////////////////////////////////////////////////////////////////////////////////////////
    RadioButton(WindowId id, WindowId first, WindowId last) : base(id), 
                                                              Check(*this, first, last)
    {
      // Set properties
      this->Style = WindowStyle::ChildWindow|WindowStyle::TabStop | ButtonStyle::AutoRadioButton|ButtonStyle::Left|ButtonStyle::Notify;
      this->StyleEx = WindowStyleEx::Transparent;

      // Listen for clicks
      this->Click += new ButtonClickEventHandler<encoding>(this, &RadioButton::onClick);
    }
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    DISABLE_COPY(RadioButton);     //!< Cannot be copied
    ENABLE_MOVE(RadioButton);      //!< Can be moved
    ENABLE_POLY(RadioButton);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------
  
    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  private:
    ///////////////////////////////////////////////////////////////////////////////
    // RadioButton::onClick
    //! Raises the 'CheckChanged' event in response to clicks
    //! 
    //! \param[in] args - Message arguments
    //! \return wtl::LResult - Routing indicating message was handled
    ///////////////////////////////////////////////////////////////////////////////
    LResult  onClick(ButtonClickEventArgs<encoding> args) 
    { 
      // Raise 'Check Changed' event
      //Checked.raise(RadioButtonCheckedEventArgs<encoding>(args));
      Check = true;
    
      // Handled
      return {MsgRoute::Handled, 0};
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // RadioButton::onOwnerDraw
    //! Called in response to a reflected 'owner draw' message to draw the button
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    //!
    //! \throw wtl::platform_error - Unable to draw radio button
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onOwnerDraw(OwnerDrawCtrlEventArgs<encoding>& args) override
    { 
      // debug
      //cdebug << object_info(__func__, "Ident", args.Ident, 
      //                                "Action",args.Action, 
      //                                "Check",this->Check(), 
      //                                "BM_GETSTATE", enum_cast<ButtonState>( this->send(ButtonMessage::GetState).Result ),
      //                                "IsDlgButtonChecked", enum_cast<ButtonState>( ::IsDlgButtonChecked(*this->parent(), enum_cast(this->Ident())) ),
      //                                "State",args.State) << std::endl;
      
      // Draw control using current window skin
      SkinFactory<encoding>::get()->draw(*this, args.Graphics, args.Rect);

      // Handle message
      return {MsgRoute::Handled, 0};
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // RadioButton::onOwnerMeasure
    //! Called in response to a reflected 'owner measure' message to 
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    //!
    //! \throw wtl::platform_error - Unable to measure radio button
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onOwnerMeasure(OwnerMeasureCtrlEventArgs<encoding>& args) override
    { 
      // Measure control using current window skin
      args.Size = SkinFactory<encoding>::get()->measure(*this, args.Graphics);

      // Handle message
      return {MsgRoute::Handled, 0};
    }
  };
  
} // namespace wtl

#include <wtl/windows/controls/radiobutton/RadioButtonCheckProperty.hpp>   //!< RadioButtonCheckProperty

#endif // WTL_RADIOBUTTON_HPP
