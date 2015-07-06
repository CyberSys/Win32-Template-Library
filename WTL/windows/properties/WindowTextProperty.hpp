//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\WindowTextProperty.hpp
//! \brief Encapsulates the window text in a read/write dynamic-string property
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_TEXT_PROPERTY_HPP
#define WTL_WINDOW_TEXT_PROPERTY_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EncodingTraits.hpp"                  //!< Encoding
#include "wtl/windows/properties/WindowProperty.hpp"      //!< WindowPropertyImpl
#include "wtl/utils/String.hpp"                           //!< String

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct WindowTextPropertyImpl - Encapsulates the window-text in a read/write dynamic-string property.
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks When the window does not exist this provides the initial value used during window creation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct WindowTextPropertyImpl : WindowPropertyImpl<ENC,String<ENC>,PropertyAccess::ReadWrite>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = WindowTextPropertyImpl;

    //! \alias base - Define base type
    using base = WindowPropertyImpl<ENC,String<ENC>,PropertyAccess::ReadWrite>;
      
    //! \alias char_t - Define window character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowTextPropertyImpl::WindowTextPropertyImpl
    //! Create with initial value
    //! 
    //! \param[in,out] &wnd - Owner window
    //! \param[in] init - Initial window text
    /////////////////////////////////////////////////////////////////////////////////////////
    WindowTextPropertyImpl(WindowBase<ENC>& wnd, value_t init = default<value_t>()) : base(wnd, init)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowTextPropertyImpl::get const
    //! Get the current text if window exists, otherwise 'initial' text
    //! 
    //! \return value_t - Dynamic string containing current Window text (using window character encoding)
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowTextPropertyImpl::set 
    //! Set the current window text iff window exists, otherwise 'initial' text
    //! 
    //! \param[in] text - Window text
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t text);
  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias WindowTextProperty - Define window text property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using WindowTextProperty = Property<WindowTextPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_WINDOW_TEXT_PROPERTY_HPP
