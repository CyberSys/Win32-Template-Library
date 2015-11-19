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
#include <wtl/windows/Window.hpp>                     //!< Window
#include <wtl/controls/Button.hpp>                        //!< Button
//#include <wtl/controls/events/ButtonEvents.hpp>           //!< ButtonClickEvent
//#include <wtl/controls/properties/ButtonIconProperty.h>   //!< ButtonIconProperty
//#include <wtl/controls/properties/ButtonStateProperty.h>  //!< ButtonStateProperty
//#include <wtl/gdi/Theme.hpp>                              //!< Theme

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
    //ButtonIconProperty<encoding>      Icon;          //!< Icon
    //ButtonStateProperty<encoding>     State;         //!< State

    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CheckBox::CheckBox
    //! Creates the window object for a WTL checkbox control without creating the window handle
    //! 
    //! \param[in] id - Control identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    CheckBox(WindowId id) : base(id)
    {
      // Set properties
      this->Style |= ButtonStyle::CheckBox;
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
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onOwnerDraw(OwnerDrawCtrlEventArgs<encoding>& args) override
    { 
      // debug
      cdebug << object_info(__func__, "Ident", args.Ident, "Action",args.Action, "State",args.State) << endl;

      try
      {        
        Theme theme(this->handle(), L"Button");
        RectL rc = args.Rect;

        // Determine state
        CHECKBOXSTATES state = (!this->Enabled                          ? CBS_UNCHECKEDDISABLED 
                               : args.State && OwnerDrawState::Selected ? CBS_UNCHECKEDPRESSED 
                               : this->isMouseOver()                    ? CBS_UNCHECKEDHOT : CBS_UNCHECKEDNORMAL);
        
        // Draw background
        theme.fill(args.Graphics, BP_CHECKBOX, state, args.Rect);

        // Pressed: Offset drawing rect
        if (state == PBS_PRESSED)
          rc += PointL(1,1);

        // Draw text
        DrawTextFlags flags = DrawTextFlags::SingleLine|DrawTextFlags::VCentre|DrawTextFlags::Centre;
        theme.write(args.Graphics, BP_CHECKBOX, state, this->Text(), rc, flags);
      }
      catch (const std::exception&)
      {
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
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onOwnerMeasure(OwnerMeasureCtrlEventArgs<encoding>& args) override
    { 
      // Measure button text
      args.Size = args.Graphics.measure(this->Text());

      // Handle message
      return {MsgRoute::Handled, 0};
    }
  };
} // namespace wtl

//#include <wtl/controls/properties/CheckBoxIconProperty.hpp>      //!< IconProperty
//#include <wtl/controls/properties/CheckBoxStateProperty.hpp>     //!< StateProperty

#endif // WTL_CHECKBOX_HPP
