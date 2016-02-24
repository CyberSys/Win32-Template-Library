//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\properties\RadioButtonCheckProperty.hpp
//! \brief Separate class declaration for the RadioButton control 'Check' property
//! \date 6 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RADIOBUTTON_CHECKED_H
#define WTL_RADIOBUTTON_CHECKED_H

#include <wtl/WTL.hpp>
#include <wtl/traits/EncodingTraits.hpp>              //!< Encoding
#include <wtl/platform/ControlStyles.hpp>             //!< RadioButtonCheck
#include <wtl/windows/PropertyImpl.hpp>               //!< PropertyImpl
#include <wtl/windows/events/CreateWindowEvent.hpp>   //!< CreateWindowEventArgs

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  //! Forward declaration
  template <Encoding ENC>
  struct RadioButton;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct RadioButtonCheckPropertyImpl - Provides the getters and setters for the button control 'Check' property
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct RadioButtonCheckPropertyImpl : PropertyImpl<ENC,bool,RadioButton<ENC>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = RadioButtonCheckPropertyImpl;

    //! \alias base - Define base type
    using base = PropertyImpl<ENC,bool,RadioButton<ENC>>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;
    
    //! \alias window_t - Inherit window type
    using window_t = typename base::window_t;

    // ----------------------------------- REPRESENTATION -----------------------------------
  private:
    WindowId    First, Last;

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // RadioButtonCheckPropertyImpl::RadioButtonCheckPropertyImpl
    //! Create window property 
    //! 
    //! \param[in,out] &wnd - Owner window
    //! \param[in] first - Identifier of first control in group
    //! \param[in] last - Identifier of last control in group
    /////////////////////////////////////////////////////////////////////////////////////////
    RadioButtonCheckPropertyImpl(window_t& wnd, WindowId first, WindowId last) : base(wnd, false), 
                                                                                 First(first), 
                                                                                 Last(last)
    {
      wnd.Create += new CreateWindowEventHandler<base::encoding>(this, &type::onCreate);
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // RadioButtonCheckPropertyImpl::get const
    //! Get the button state
    //! 
    //! \return value_t - Current state if button exists, otherwise initial state
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // RadioButtonCheckPropertyImpl::set 
    //! Set the state iff button exists, otherwise sets the initial state
    //! 
    //! \param[in] state - RadioButton state
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t state);

  private:
    /////////////////////////////////////////////////////////////////////////////////////////
    // RadioButtonCheckPropertyImpl::onCreate 
    //! Called during button creation to set the initial state
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Routing indicating message was handled
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onCreate(CreateWindowEventArgs<ENC>& args);
  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias RadioButtonCheckProperty - Define button state property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using RadioButtonCheckProperty = Property<RadioButtonCheckPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_RADIOBUTTON_CHECKED_H
