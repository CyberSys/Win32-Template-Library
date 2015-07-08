//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\EnabledProperty.hpp
//! \brief Encapsulates the state of a window (enabled/disabled) in a boolean property
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_ENABLED_PROPERTY_HPP
#define WTL_WINDOW_ENABLED_PROPERTY_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EncodingTraits.hpp"                  //!< Encoding
#include "wtl/windows/properties/WindowProperty.hpp"      //!< WindowPropertyImpl

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct EnabledPropertyImpl - Encapsulates the state of a window (enabled/disabled) in a read/write boolean property.
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks When the window does not exist this provides the initial value used during window creation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EnabledPropertyImpl : WindowPropertyImpl<ENC,bool,PropertyAccess::ReadWrite>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = EnabledPropertyImpl;

    //! \alias base - Define base type
    using base = WindowPropertyImpl<ENC,bool,PropertyAccess::ReadWrite>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // EnabledPropertyImpl::EnabledPropertyImpl
    //! Create with initial value
    //! 
    //! \param[in,out] &wnd - Owner window
    //! \param[in] init - Initial enabled state
    /////////////////////////////////////////////////////////////////////////////////////////
    EnabledPropertyImpl(WindowBase<ENC>& wnd, bool init) : base(wnd, init)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // EnabledPropertyImpl::get const
    //! Get the window state
    //! 
    //! \return value_t - Current state if window exists, otherwise 'initial' state
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // EnabledPropertyImpl::set 
    //! Set the current window state iff window exists, otherwise 'initial' state
    //! 
    //! \param[in] state - Window state
    //! 
    //! \throw wtl::platform_error - Unable to set window state
    /////////////////////////////////////////////////////////////////////////////////////////
    void set(value_t state);
  };

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias EnabledProperty - Define window visibliity property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using EnabledProperty = Property<EnabledPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_WINDOW_ENABLED_PROPERTY_HPP
