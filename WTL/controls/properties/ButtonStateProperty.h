//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\ButtonStateProperty.hpp
//! \brief Separate class declaration for the Button control 'State' property
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BUTTON_STATE_PROPERTY_H
#define WTL_BUTTON_STATE_PROPERTY_H

#include <wtl/WTL.hpp>
#include <wtl/traits/EncodingTraits.hpp>              //!< Encoding
#include <wtl/platform/ControlStyles.hpp>             //!< ButtonState
#include <wtl/windows/PropertyImpl.hpp>               //!< PropertyImpl
#include <wtl/windows/events/CreateWindowEvent.hpp>   //!< CreateWindowEventArgs

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  //! Forward declaration
  template <Encoding ENC>
  struct Button;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ButtonStatePropertyImpl - Provides the getters and setters for the button control 'State' property
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ButtonStatePropertyImpl : PropertyImpl<ENC,ButtonState,Button<ENC>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = ButtonStatePropertyImpl;

    //! \alias base - Define base type
    using base = PropertyImpl<ENC,ButtonState,Button<ENC>>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;
    
    //! \alias window_t - Inherit window type
    using window_t = typename base::window_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ButtonStatePropertyImpl::ButtonStatePropertyImpl
    //! Create window property 
    //! 
    //! \param[in,out] &wnd - Owner window
    /////////////////////////////////////////////////////////////////////////////////////////
    ButtonStatePropertyImpl(window_t& wnd) : base(wnd, ButtonState::Unchecked)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ButtonStatePropertyImpl::get const
    //! Get the button state
    //! 
    //! \return value_t - Current state if button exists, otherwise initial state
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // ButtonStatePropertyImpl::set 
    //! Set the state iff button exists, otherwise sets the initial state
    //! 
    //! \param[in] state - Button state
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t state);
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ButtonStatePropertyImpl::onCreate 
    //! Called during button creation to set the initial state
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Returns 0 to accept button creation
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onCreate(CreateWindowEventArgs<ENC>& args);
  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ButtonStateProperty - Define button state property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using ButtonStateProperty = Property<ButtonStatePropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_BUTTON_STATE_PROPERTY_H
