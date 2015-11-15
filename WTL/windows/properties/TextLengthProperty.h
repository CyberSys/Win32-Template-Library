//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\TextLengthProperty.hpp
//! \brief Separate class declaration for the 'TextLength' window property
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_TEXT_LENGTH_PROPERTY_H
#define WTL_WINDOW_TEXT_LENGTH_PROPERTY_H

#include <wtl/WTL.hpp>
#include <wtl/traits/EncodingTraits.hpp>     //!< Encoding
#include <wtl/windows/PropertyImpl.hpp>      //!< PropertyImpl

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct TextLengthPropertyImpl - Encapsulates the window-text length in a read-only property.
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks When the window does not exist this is zero
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct TextLengthPropertyImpl : PropertyImpl<ENC,uint32_t>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = TextLengthPropertyImpl;

    //! \alias base - Define base type
    using base = PropertyImpl<ENC,uint32_t>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;
    
    //! \alias window_t - Inherit window type
    using window_t = typename base::window_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // TextLengthPropertyImpl::TextLengthPropertyImpl
    //! Create window property
    //! 
    //! \param[in,out] &wnd - Owner window
    /////////////////////////////////////////////////////////////////////////////////////////
    TextLengthPropertyImpl(window_t& wnd) : base(wnd, zero<value_t>())
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // TextLengthPropertyImpl::get const
    //! Get length of text, in characters
    //! 
    //! \return value_t - Length of current window text, in characters.  (Always zero when window doesn't exist)
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias TextLengthProperty - Define window-text length property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using TextLengthProperty = Property<TextLengthPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_WINDOW_TEXT_LENGTH_PROPERTY_H
