//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\properties\CheckBoxCheckProperty.hpp
//! \brief Separate class declaration for the CheckBox control 'Check' property
//! \date 22 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CHECKBOX_CHECKED_H
#define WTL_CHECKBOX_CHECKED_H

#include <wtl/WTL.hpp>
#include <wtl/traits/EncodingTraits.hpp>              //!< Encoding
#include <wtl/platform/ControlStyles.hpp>             //!< CheckBoxCheck
#include <wtl/windows/PropertyImpl.hpp>               //!< PropertyImpl
#include <wtl/windows/events/CreateWindowEvent.hpp>   //!< CreateWindowEventArgs

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  //! Forward declaration
  template <Encoding ENC>
  struct CheckBox;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct CheckBoxCheckPropertyImpl - Provides the getters and setters for the button control 'Check' property
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct CheckBoxCheckPropertyImpl : PropertyImpl<ENC,ButtonState,CheckBox<ENC>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = CheckBoxCheckPropertyImpl;

    //! \alias base - Define base type
    using base = PropertyImpl<ENC,ButtonState,CheckBox<ENC>>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;
    
    //! \alias window_t - Inherit window type
    using window_t = typename base::window_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // CheckBoxCheckPropertyImpl::CheckBoxCheckPropertyImpl
    //! Create window property 
    //! 
    //! \param[in,out] &wnd - Owner window
    /////////////////////////////////////////////////////////////////////////////////////////
    CheckBoxCheckPropertyImpl(window_t& wnd) : base(wnd, ButtonState::Unchecked)
    {
      wnd.Create += new CreateWindowEventHandler<base::encoding>(this, &type::onCreate);
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CheckBoxCheckPropertyImpl::get const
    //! Get the button state
    //! 
    //! \return value_t - Current state if button exists, otherwise initial state
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // CheckBoxCheckPropertyImpl::set 
    //! Set the state iff button exists, otherwise sets the initial state
    //! 
    //! \param[in] state - CheckBox state
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t state);

  private:
    /////////////////////////////////////////////////////////////////////////////////////////
    // CheckBoxCheckPropertyImpl::onCreate 
    //! Called during button creation to set the initial state
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onCreate(CreateWindowEventArgs<ENC>& args);
  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CheckBoxCheckProperty - Define button state property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CheckBoxCheckProperty = Property<CheckBoxCheckPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_CHECKBOX_CHECKED_H
