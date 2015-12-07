//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\RichEditSelectedTextProperty.h
//! \brief Separate class declaration for the 'SelectedRange' window property
//! \date 6 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RICH_EDIT_SELECTED_TEXT_PROPERTY_H
#define WTL_RICH_EDIT_SELECTED_TEXT_PROPERTY_H

#include <wtl/WTL.hpp>
#include <wtl/traits/EncodingTraits.hpp>     //!< Encoding
#include <wtl/windows/PropertyImpl.hpp>      //!< PropertyImpl
#include <wtl/utils/String.hpp>              //!< String

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct RichEditSelectedTextPropertyImpl - Provides the getters and setters for the 'TextSelection' richedit property
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct RichEditSelectedTextPropertyImpl : PropertyImpl<ENC,String<ENC>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = RichEditSelectedTextPropertyImpl;

    //! \alias base - Define base type
    using base = PropertyImpl<ENC,String<ENC>>;
      
    //! \alias char_t - Define window character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;
    
    //! \alias window_t - Inherit window type
    using window_t = typename base::window_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // RichEditSelectedTextPropertyImpl::RichEditSelectedTextPropertyImpl
    //! Create property 
    //! 
    //! \param[in,out] &wnd - Owner window
    /////////////////////////////////////////////////////////////////////////////////////////
    RichEditSelectedTextPropertyImpl(window_t& wnd) : base(wnd, defvalue<value_t>())
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // RichEditSelectedTextPropertyImpl::get const
    //! Get the currently selected text, if any
    //! 
    //! \return value_t - Currently selected text
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // RichEditSelectedTextPropertyImpl::set 
    //! Replaces the currently selected text
    //! 
    //! \param[in] const& txt - New text
    //! 
    //! \throw wtl::platform_error - Unable to set text 
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(const value_t& txt);
  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias RichEditSelectedTextProperty - Define selection range property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using RichEditSelectedTextProperty = Property<RichEditSelectedTextPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_RICH_EDIT_SELECTED_TEXT_PROPERTY_H
