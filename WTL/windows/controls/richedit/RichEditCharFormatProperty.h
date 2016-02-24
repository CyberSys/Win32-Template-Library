//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\richedit\RichEditCharFormatProperty.hpp
//! \brief Separate class declaration for the RichEdit control 'CharFormat' property
//! \date 6 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RICH_EDIT_CHAR_FORMAT_PROPERTY_H
#define WTL_RICH_EDIT_CHAR_FORMAT_PROPERTY_H

#include <wtl/WTL.hpp>
#include <wtl/casts/EnumCast.hpp>                     //!< enum_cast
#include <wtl/traits/EncodingTraits.hpp>              //!< Encoding
#include <wtl/windows/PropertyImpl.hpp>               //!< PropertyImpl
#include <wtl/windows/events/CreateWindowEvent.hpp>   //!< CreateWindowEventArgs

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  //! Forward declaration
  template <Encoding ENC>
  struct RichEdit;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct CharFormat - Encapsulates rich-edit control character formatting
  //! 
  //! \tparam ENC - Character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct CharFormat : choose_t<ENC,::CHARFORMATA,::CHARFORMATW>
  {
    //! \alias type - Define own type
    using type = CharFormat<ENC>;

    //! \alias base - Define base type
    using base = choose_t<ENC,::CHARFORMATA,::CHARFORMATW>;
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharFormat::CharFormat
    //! Construct character formatting with mask for requesting text colour and effects
    /////////////////////////////////////////////////////////////////////////////////////////
    CharFormat() : base{sizeof(base)}
    {
      this->dwMask = enum_cast(CharFormatMask::Effects | CharFormatMask::Colour);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // CharFormat::CharFormat
    //! Construct character formatting for text colour and effects
    //! 
    //! \param[in] fx - Text effects
    //! \param[in] col - Text colour
    /////////////////////////////////////////////////////////////////////////////////////////
    CharFormat(CharFormatEffect fx, Colour col) : base{sizeof(base)}
    {
      this->crTextColor = enum_cast(col);
      this->dwEffects = enum_cast(fx);
      this->dwMask = enum_cast(CharFormatMask::Effects | CharFormatMask::Colour);
    }
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct RichEditCharFormatPropertyImpl - Provides the getters and setters for the 'Icon' window property
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct RichEditCharFormatPropertyImpl : PropertyImpl<ENC,CharFormat<ENC>,RichEdit<ENC>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = RichEditCharFormatPropertyImpl;

    //! \alias base - Define base type
    using base = PropertyImpl<ENC,CharFormat<ENC>,RichEdit<ENC>>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;
    
    //! \alias window_t - Inherit window type
    using window_t = typename base::window_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // RichEditCharFormatPropertyImpl::RichEditCharFormatPropertyImpl
    //! Create window property 
    //! 
    //! \param[in,out] &wnd - Owner window
    /////////////////////////////////////////////////////////////////////////////////////////
    RichEditCharFormatPropertyImpl(window_t& wnd) : base(wnd)
    {
      // Register creation handler to set initial format
      wnd.Create += new CreateWindowEventHandler<base::encoding>(this, &type::onCreate);
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // RichEditCharFormatPropertyImpl::get const
    //! Get the current character formatting
    //! 
    //! \return value_t - Current formatting if control exists, otherwise initial formatting
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // RichEditCharFormatPropertyImpl::set 
    //! Set the current character formating iff control exists, otherwise sets the initial formatting
    //! 
    //! \param[in] format - Character formatting
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(const value_t& format);
  
  private:
    /////////////////////////////////////////////////////////////////////////////////////////
    // RichEditCharFormatPropertyImpl::onCreate 
    //! Called during control creation to set the initial character formatting
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Returns 0 to accept control creation
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onCreate(CreateWindowEventArgs<ENC>& args);
  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias RichEditCharFormatProperty - Define control format property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using RichEditCharFormatProperty = Property<RichEditCharFormatPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_RICH_EDIT_CHAR_FORMAT_PROPERTY_H
