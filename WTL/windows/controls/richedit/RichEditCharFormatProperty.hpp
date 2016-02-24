//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\richedit\RichEditCharFormatProperty.hpp
//! \brief Separate implementation for the RichEdit control 'CharFormat' property (resolves circular dependency)
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RICH_EDIT_CHAR_FORMAT_PROPERTY_HPP
#define WTL_RICH_EDIT_CHAR_FORMAT_PROPERTY_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/controls/richedit/RichEditCharFormatProperty.h>    //!< RichEditCharFormatPropertyImpl
#include <wtl/windows/controls/richedit/RichEdit.hpp>                    //!< RichEdit

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // RichEditCharFormatPropertyImpl::get const
  //! Get the current character formatting
  //! 
  //! \return value_t - Current formatting if control exists, otherwise initial formatting
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename RichEditCharFormatPropertyImpl<ENC>::value_t  RichEditCharFormatPropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Get current formatting iff window exists
    if (this->Window.exists())
    {
      value_t cf;
      this->Window.send(RichEditMessage::SetCharFormat, SCF_ALL, opaque_cast(cf));
      return cf;
    }

    // Return initial formatting
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // RichEditCharFormatPropertyImpl::onCreate 
  //! Called during control creation to set the initial character formatting
  //! 
  //! \param[in,out] &args - Message arguments 
  //! \return LResult - Returns 0 to accept window creation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  LResult  RichEditCharFormatPropertyImpl<ENC>::onCreate(CreateWindowEventArgs<ENC>& args)
  {
    // Set formatting iff window exists
    this->Window.send(RichEditMessage::SetCharFormat, SCF_ALL, opaque_cast(base::get())); 

    // Accept window creation
    return 0;
  }
    
  /////////////////////////////////////////////////////////////////////////////////////////
  // RichEditCharFormatPropertyImpl::set 
  //! Set the current character formating iff control exists, otherwise sets the initial formatting
  //! 
  //! \param[in] format - Character formatting
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  RichEditCharFormatPropertyImpl<ENC>::set(const value_t& format) 
  {
    // [EXISTS] Set formatting iff window exists
    if (this->Window.exists())
      this->Window.send(RichEditMessage::SetCharFormat, SCF_SELECTION, opaque_cast(format)); 
    
    // Update 'initial' value
    base::set(format);
  }
  
    
} // namespace wtl

#endif // WTL_RICH_EDIT_CHAR_FORMAT_PROPERTY_HPP
