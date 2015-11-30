//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\controls\CheckBox.hpp
//! \brief Encapsulates the standard checkbox control
//! \date 17 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CHECKBOX_HPP
#define WTL_CHECKBOX_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/Window.hpp>                //!< Window
#include <wtl/controls/Button.hpp>               //!< Button
#include <wtl/platform/Metrics.hpp>              //!< Metrics
#include <wtl/controls/properties/CheckBoxCheckedProperty.h>   //!< CheckBoxCheckedProperty

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
    CheckBoxCheckedProperty<encoding>     Checked;         //!< Checked

    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CheckBox::CheckBox
    //! Creates the window object for a WTL checkbox control without creating the window handle
    //! 
    //! \param[in] id - Control identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    CheckBox(WindowId id) : base(id), 
                            Checked(*this)
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
      cdebug << object_info(__func__, "Ident", args.Ident, 
                                      "Action",args.Action, 
                                      "Checked",this->Checked(), 
                                      "BM_GETSTATE", enum_cast<ButtonState>( this->template send<ButtonMessage::GetState>().Result ),
                                      "IsDlgButtonChecked", enum_cast<ButtonState>( ::IsDlgButtonChecked(*this->parent(), enum_cast(this->Ident())) ),
                                      "State",args.State) << endl;

      Theme theme(this->handle(), L"Button");
        
      // Determine state
      CHECKBOXSTATES state = CBS_UNCHECKEDNORMAL;
      if (!this->Enabled)
        state = CBS_UNCHECKEDDISABLED;
      else if (args.State && OwnerDrawState::Selected)
        state = CBS_UNCHECKEDPRESSED;
      else if (this->isMouseOver())
        state = CBS_UNCHECKEDHOT;
      
      //if (this->Checked == ButtonState::Checked)  //if (args.State && OwnerDrawState::Checked)
      //  state += (CBS_CHECKEDNORMAL-1);
      //if (this->State && ButtonState::Checked)  
      //  state += (CBS_CHECKEDNORMAL-1);
      //else if (this->Checked == ButtonState::Indeterminate)
      //  state += (CBS_MIXEDNORMAL-1);

      // Query content rect
      RectL rcContent = theme.content(args.Graphics, BP_CHECKBOX, state, args.Rect);

      // Draw background
      args.Graphics.fill(args.Rect, StockBrush::BtnFace);

      // Calculate checkbox / text rectangles
      SizeL szCheckBox = theme.measure(args.Graphics, BP_CHECKBOX, state);
      RectL rcCheckBox = rcContent.arrange(szCheckBox, {RectL::FromLeft,Metrics::WindowEdge.Width}, RectL::Centre);
      
      // Draw checkbox
      theme.fill(args.Graphics, BP_CHECKBOX, state, rcCheckBox);

      // Draw text
      rcContent.Left = rcCheckBox.Right + Metrics::WindowEdge.Width;
      theme.write(args.Graphics, BP_CHECKBOX, state, this->Text(), rcContent, DrawTextFlags::Left|DrawTextFlags::VCentre|DrawTextFlags::SingleLine);
      
      // [FOCUS] Draw focus rectangle
      if (args.State && OwnerDrawState::Focus)
      {
        RectL rcText = theme.measure(args.Graphics, BP_CHECKBOX, state, this->Text(), rcContent, DrawTextFlags::Left|DrawTextFlags::VCentre|DrawTextFlags::SingleLine);
        args.Graphics.focus(rcText);
      }

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
      Theme theme(this->handle(), L"Button");

      // Measure checkbox + text + edges
      args.Size = theme.measure(args.Graphics, BP_CHECKBOX, CBS_UNCHECKEDNORMAL) 
                + args.Graphics.measure(this->Text()) 
                + SizeL(3*Metrics::WindowEdge.Width, 0);

      // Handle message
      return {MsgRoute::Handled, 0};
    }
  };
} // namespace wtl

#include <wtl/controls/properties/CheckBoxCheckedProperty.hpp>   //!< CheckBoxCheckedProperty

#endif // WTL_CHECKBOX_HPP
