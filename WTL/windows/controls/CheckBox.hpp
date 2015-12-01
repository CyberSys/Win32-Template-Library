//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\CheckBox.hpp
//! \brief Encapsulates the standard checkbox control
//! \date 17 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CHECKBOX_HPP
#define WTL_CHECKBOX_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/Window.hpp>                //!< Window
#include <wtl/windows/controls/Button.hpp>               //!< Button
#include <wtl/platform/Metrics.hpp>              //!< Metrics
#include <wtl/windows/controls/properties/CheckBoxCheckProperty.h>   //!< CheckBoxCheckProperty

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct CheckBox - Encapsulates a button control
  //! 
  //! \tparam ENC - Character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct CheckBox : Button<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = CheckBox<ENC>;
  
    //! \alias base - Define base type
    using base = Button<ENC>;
    
    //! \alias class_t - Inherit class type
    using class_t = typename base::class_t;
    
    //! \var encoding - Inherit character encoding
    static constexpr Encoding  encoding = base::encoding;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    // Properties
    CheckBoxCheckProperty<encoding>     Check;         //!< Check state

    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CheckBox::CheckBox
    //! Creates the window object for a WTL checkbox control without creating the window handle
    //! 
    //! \param[in] id - Control identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    CheckBox(WindowId id) : base(id), 
                            Check(*this)
    {
      // Set properties
      this->Style = WindowStyle::ChildWindow | ButtonStyle::AutoCheckBox|ButtonStyle::Left|ButtonStyle::Notify;
    }
    
    // -------------------------------- COPY, MOVE & DESTROY  -------------------------------
  public:
    DISABLE_COPY(CheckBox);     //!< Cannot be copied
    ENABLE_MOVE(CheckBox);      //!< Can be moved
    ENABLE_POLY(CheckBox);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------
  
    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  private:
    /////////////////////////////////////////////////////////////////////////////////////////
    // CheckBox::onOwnerDraw
    //! Called in response to a reflected 'owner draw' message to draw the button
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    //!
    //! \throw wtl::platform_error - Unable to draw checkbox
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onOwnerDraw(OwnerDrawCtrlEventArgs<encoding>& args) override
    { 
      // debug
      //cdebug << object_info(__func__, "Ident", args.Ident, 
      //                                "Action",args.Action, 
      //                                "Check",this->Check(), 
      //                                "BM_GETSTATE", enum_cast<ButtonState>( this->template send<ButtonMessage::GetState>().Result ),
      //                                "IsDlgButtonChecked", enum_cast<ButtonState>( ::IsDlgButtonChecked(*this->parent(), enum_cast(this->Ident())) ),
      //                                "State",args.State) << std::endl;
      
      // Draw control using current window skin
      SkinFactory<encoding>::get()->draw(*this, args.Graphics, args.Rect);

      // Handle message
      return {MsgRoute::Handled, 0};
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // CheckBox::onOwnerMeasure
    //! Called in response to a reflected 'owner measure' message to 
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    //!
    //! \throw wtl::platform_error - Unable to measure checkbox
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

#include <wtl/windows/controls/properties/CheckBoxCheckProperty.hpp>   //!< CheckBoxCheckProperty

#endif // WTL_CHECKBOX_HPP
