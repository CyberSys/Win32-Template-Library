//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\TextProperty.h
//! \brief Separate class declaration for the 'Text' window property
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_TEXT_PROPERTY_H
#define WTL_WINDOW_TEXT_PROPERTY_H

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
  //! \struct TextPropertyImpl - Provides the getters and setters for the 'Text' window property
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks Provides initial window text during window creation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct TextPropertyImpl : WindowPropertyImpl<ENC,String<ENC>,PropertyAccess::ReadWrite>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = TextPropertyImpl;

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
    // TextPropertyImpl::TextPropertyImpl
    //! Create with initial value
    //! 
    //! \param[in,out] &wnd - Owner window
    //! \param[in] init - Initial window text
    /////////////////////////////////////////////////////////////////////////////////////////
    TextPropertyImpl(WindowBase<ENC>& wnd, value_t init = defvalue<value_t>()) : base(wnd, init)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // TextPropertyImpl::get const
    //! Get the current text if window exists, otherwise 'initial' text
    //! 
    //! \return value_t - Dynamic string containing current Window text (using window character encoding)
    //! 
    //! \throw wtl::platform_error - Unable to retrieve window text
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // TextPropertyImpl::set 
    //! Set the current window text iff window exists, otherwise 'initial' text
    //! 
    //! \param[in] text - Window text
    //! 
    //! \throw wtl::platform_error - Unable to set window text
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t text);
  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias TextProperty - Define window text property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using TextProperty = Property<TextPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_WINDOW_TEXT_PROPERTY_H
