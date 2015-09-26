//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\IdentProperty.hpp
//! \brief Encapsulates the child-window-id in an appropriate enumeration property
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_ID_PROPERTY_HPP
#define WTL_WINDOW_ID_PROPERTY_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EncodingTraits.hpp"                  //!< Encoding
#include "wtl/platform/WindowFlags.hpp"                   //!< WindowId
#include "wtl/windows/properties/WindowProperty.hpp"      //!< WindowPropertyImpl

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct IdentPropertyImpl - Encapsulates the window-id in a read/write property.
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks When the window does not exist this provides the initial value used during window creation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct IdentPropertyImpl : WindowPropertyImpl<ENC,WindowId,PropertyAccess::ReadWrite>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = IdentPropertyImpl;

    //! \alias base - Define base type
    using base = WindowPropertyImpl<ENC,WindowId,PropertyAccess::ReadWrite>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // IdentPropertyImpl::IdentPropertyImpl
    //! Create with initial window-id
    //! 
    //! \param[in,out] &wnd - Owner window
    //! \param[in] init - [optional] Initial window id  (Default is zero)
    /////////////////////////////////////////////////////////////////////////////////////////
    IdentPropertyImpl(WindowBase<ENC>& wnd, WindowId init = zero<WindowId>()) : base(wnd, init)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // IdentPropertyImpl::get const
    //! Get the window id
    //! 
    //! \return value_t - Current window-id if window exists, otherwise 'initial' window-id
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const; // { return defvalue<WindowId>(); }

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // IdentPropertyImpl::set 
    //! Set the current window id iff window exists, otherwise 'initial' window-id
    //! 
    //! \param[in] id - Window id
    //! 
    //! \throw wtl::platform_error - Unable to set window-id
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t id);
  };

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias IdentProperty - Define window id property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using IdentProperty = Property<IdentPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_WINDOW_ID_PROPERTY_HPP
