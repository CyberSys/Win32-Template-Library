//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\richedit\RichEditParagraphFormatProperty.hpp
//! \brief Separate implementation for the RichEdit control 'ParagraphFormat' property (resolves circular dependency)
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RICH_EDIT_PARAGRAPH_FORMAT_PROPERTY_HPP
#define WTL_RICH_EDIT_PARAGRAPH_FORMAT_PROPERTY_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/controls/richedit/RichEditParagraphFormatProperty.h>    //!< RichEditParagraphFormatPropertyImpl
#include <wtl/windows/controls/richedit/RichEdit.hpp>                         //!< RichEdit

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // RichEditParagraphFormatPropertyImpl::get const
  //! Get the current paragraph formatting
  //! 
  //! \return value_t - Current formatting if control exists, otherwise initial formatting
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename RichEditParagraphFormatPropertyImpl<ENC>::value_t  RichEditParagraphFormatPropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Get current formatting iff window exists
    if (this->Window.exists())
    {
      value_t cf;
      // TODO: this->Window.send(RichEditMessage::SetParagraphFormat, SCF_ALL, opaque_cast(cf));
      return cf;
    }

    // Return initial formatting
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // RichEditParagraphFormatPropertyImpl::onCreate 
  //! Called during control creation to set the initial paragraph formatting
  //! 
  //! \param[in,out] &args - Message arguments 
  //! \return LResult - Returns 0 to accept window creation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  LResult  RichEditParagraphFormatPropertyImpl<ENC>::onCreate(CreateWindowEventArgs<ENC>& args)
  {
    // Set formatting iff window exists
    // TODO: this->Window.send(RichEditMessage::SetParagraphFormat, SCF_ALL, opaque_cast(base::get())); 

    // Accept window creation
    return 0;
  }
    
  /////////////////////////////////////////////////////////////////////////////////////////
  // RichEditParagraphFormatPropertyImpl::set 
  //! Set the current paragraph formating iff control exists, otherwise sets the initial formatting
  //! 
  //! \param[in] format - Paragraphacter formatting
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  RichEditParagraphFormatPropertyImpl<ENC>::set(const value_t& format) 
  {
    // [EXISTS] Set formatting iff window exists
    if (this->Window.exists())
      // TODO: this->Window.send(RichEditMessage::SetParagraphFormat, SCF_SELECTION, opaque_cast(format)); 
    
    // Update 'initial' value
    base::set(format);
  }
  
    
} // namespace wtl

#endif // WTL_RICH_EDIT_PARAGRAPH_FORMAT_PROPERTY_HPP
