//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\richedit\RichEditSelectedTextProperty.hpp
//! \brief Separate implementation for 'SelectedRange' window property (resolves circular dependency)
//! \date 6 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RICH_EDIT_SELECTED_TEXT_PROPERTY_HPP
#define WTL_RICH_EDIT_SELECTED_TEXT_PROPERTY_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/OpaqueCast.hpp>                                         //!< opaque_cast
#include <wtl/windows/controls/richedit/RichEditSelectedTextProperty.h>     //!< RichEditSelectedTextProperty
#include <wtl/windows/controls/richedit/RichEdit.hpp>                       //!< RichEdit

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // RichEditSelectedTextPropertyImpl::get const
  //! Get the currently selected text, if any
  //! 
  //! \return value_t - Currently selected text
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename RichEditSelectedTextPropertyImpl<ENC>::value_t  RichEditSelectedTextPropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Query window text
    if (this->Window.exists())
    {
      // Query length of selection
      GETTEXTLENGTHEX gtl {GTL_DEFAULT, UINT(ENC)};
      uint32_t len = this->Window.send(RichEditMessage::GetTextLengthEx, opaque_cast(&gtl)).Result;

      // Allocate buffer
      GETTEXTEX gt {len*sizeof(encoding_char_t<ENC>), GT_SELECTION|GT_USECRLF};
      std::vector<encoding_char_t<ENC>> txt(len+1);
      
      // Query selection text
      this->Window.send(RichEditMessage::GetTextEx, opaque_cast(&gt), opaque_cast(txt.data()));
      txt.back() = '\0';

      // Return selection text
      return txt.data();
    }
        
    // [OFFLINE] Return empty
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // RichEditSelectedTextPropertyImpl::set 
  //! Replaces the currently selected text
  //! 
  //! \param[in] const& txt - New text
  //! 
  //! \throw wtl::platform_error - Unable to set selection text 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  RichEditSelectedTextPropertyImpl<ENC>::set(const value_t& txt) 
  {
    // [EXISTS] Set text selection range
    if (this->Window.exists())
    {
      SETTEXTEX st {ST_SELECTION|ST_KEEPUNDO, UINT(ENC)};
      /*if (ENC == Encoding::UTF16)
        st.flags |= ST_UNICODE;*/

      // Set text
      this->Window.send(RichEditMessage::SetTextEx, opaque_cast(&st), opaque_cast(txt.c_str()));
      /*if (!this->Window.send(RichEditMessage::SetTextEx, opaque_cast(&st), opaque_cast(txt.c_str())).Result)
        throw platform_error(HERE, "Unable to set selection text");*/
    }

    // Store value
    //base::set(txt);
  }

      
} // namespace wtl

#endif  // WTL_RICH_EDIT_SELECTED_TEXT_PROPERTY_HPP

