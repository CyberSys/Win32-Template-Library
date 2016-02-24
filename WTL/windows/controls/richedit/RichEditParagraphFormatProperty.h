//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\richedit\RichEditParagraphFormatProperty.hpp
//! \brief Separate class declaration for the RichEdit control 'ParagraphFormat' property
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RICH_EDIT_PARAGRAPH_FORMAT_PROPERTY_H
#define WTL_RICH_EDIT_PARAGRAPH_FORMAT_PROPERTY_H

#include <wtl/WTL.hpp>
#include <wtl/traits/EncodingTraits.hpp>              //!< Encoding
#include <wtl/traits/IconTraits.hpp>                  //!< HIcon
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
  //! \struct RichEditParagraphFormatPropertyImpl - Provides the getters and setters for the 'Icon' window property
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct RichEditParagraphFormatPropertyImpl : PropertyImpl<ENC,ParaFormat<ENC>,RichEdit<ENC>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = RichEditParagraphFormatPropertyImpl;

    //! \alias base - Define base type
    using base = PropertyImpl<ENC,ParaFormat<ENC>,RichEdit<ENC>>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;
    
    //! \alias window_t - Inherit window type
    using window_t = typename base::window_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // RichEditParagraphFormatPropertyImpl::RichEditParagraphFormatPropertyImpl
    //! Create window property 
    //! 
    //! \param[in,out] &wnd - Owner window
    /////////////////////////////////////////////////////////////////////////////////////////
    RichEditParagraphFormatPropertyImpl(window_t& wnd) : base(wnd)
    {
      // Register creation handler to set initial format
      wnd.Create += new CreateWindowEventHandler<base::encoding>(this, &type::onCreate);
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // RichEditParagraphFormatPropertyImpl::get const
    //! Get the current paragraph formatting
    //! 
    //! \return value_t - Current formatting if control exists, otherwise initial formatting
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // RichEditParagraphFormatPropertyImpl::set 
    //! Set the current paragraph formating iff control exists, otherwise sets the initial formatting
    //! 
    //! \param[in] format - Character formatting
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t icon);
  
  private:
    /////////////////////////////////////////////////////////////////////////////////////////
    // RichEditParagraphFormatPropertyImpl::onCreate 
    //! Called during control creation to set the initial paragraph formatting
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Returns 0 to accept button creation
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onCreate(CreateWindowEventArgs<ENC>& args);
  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias RichEditParagraphFormatProperty - Define button icon property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using RichEditParagraphFormatProperty = Property<RichEditParagraphFormatPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_RICH_EDIT_PARAGRAPH_FORMAT_PROPERTY_H
