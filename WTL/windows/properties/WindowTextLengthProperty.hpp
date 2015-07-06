//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\WindowTextLengthProperty.hpp
//! \brief Encapsulates a window's text length in an integer property
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_TEXT_LEN_PROPERTY_HPP
#define WTL_WINDOW_TEXT_LEN_PROPERTY_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EncodingTraits.hpp"                  //!< Encoding
#include "wtl/windows/properties/WindowProperty.hpp"      //!< WindowPropertyImpl

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct WindowTextLengthPropertyImpl - Encapsulates the window-text length in a read-only property.
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks When the window does not exist this is zero
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct WindowTextLengthPropertyImpl : WindowPropertyImpl<ENC,uint32_t,PropertyAccess::Read>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = WindowTextLengthPropertyImpl;

    //! \alias base - Define base type
    using base = WindowPropertyImpl<ENC,uint32_t,PropertyAccess::Read>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowTextLengthPropertyImpl::WindowTextLengthPropertyImpl
    //! Create with initial value of zero
    //! 
    //! \param[in,out] &wnd - Owner window
    /////////////////////////////////////////////////////////////////////////////////////////
    WindowTextLengthPropertyImpl(WindowBase<ENC>& wnd) : base(wnd, zero<value_t>())
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowTextLengthPropertyImpl::get const
    //! Get length of text, in characters
    //! 
    //! \return value_t - Length of current window text, in characters.  (Always zero when window doesn't exist)
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias WindowTextLengthProperty - Define window-text length property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using WindowTextLengthProperty = Property<WindowTextLengthPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_WINDOW_TEXT_LEN_PROPERTY_HPP
