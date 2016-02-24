//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\EditSelectionProperty.h
//! \brief Separate class declaration for the 'SelectedRange' window property
//! \date 6 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EDIT_SELECTION_PROPERTY_H
#define WTL_EDIT_SELECTION_PROPERTY_H

#include <wtl/WTL.hpp>
#include <wtl/traits/EncodingTraits.hpp>     //!< Encoding
#include <wtl/windows/PropertyImpl.hpp>      //!< PropertyImpl
#include <wtl/utils/String.hpp>              //!< String

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  //! \struct SelectionRange - Defines position of text selection range
  struct SelectionRange
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    ulong32_t  Start,         //!< Character index of first character
               Finish;        //!< Character index of final charater

    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // SelectionRange::SelectionRange
    //! Create empty range
    /////////////////////////////////////////////////////////////////////////////////////////
    SelectionRange() : Start(0), Finish(0)
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // EditSelectionPropertyImpl::EditSelectionPropertyImpl
    //! Create from positions 
    //! 
    //! \param[in] s - Start
    //! \param[in] f - Finish
    /////////////////////////////////////////////////////////////////////////////////////////
    SelectionRange(ulong32_t s, ulong32_t f) : Start(s), Finish(f)
    {}

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
    // ----------------------------------- STATIC METHODS -----------------------------------
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct EditSelectionPropertyImpl - Provides the getters and setters for the 'SelectedRange' window property
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EditSelectionPropertyImpl : PropertyImpl<ENC,SelectionRange>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = EditSelectionPropertyImpl;

    //! \alias base - Define base type
    using base = PropertyImpl<ENC,SelectionRange>;
      
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
    // EditSelectionPropertyImpl::EditSelectionPropertyImpl
    //! Create property 
    //! 
    //! \param[in,out] &wnd - Owner window
    /////////////////////////////////////////////////////////////////////////////////////////
    EditSelectionPropertyImpl(window_t& wnd) : base(wnd, defvalue<value_t>())
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // EditSelectionPropertyImpl::get const
    //! Get the current text within a dynamic string
    //! 
    //! \return value_t - Current selection range if window exists, otherwise empty range
    //! 
    //! \throw wtl::platform_error - Unable to retrieve selection range
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // EditSelectionPropertyImpl::set 
    //! Set the current selection range iff window exists, otherwise ignored
    //! 
    //! \param[in] format - Text selection range
    //! 
    //! \throw wtl::platform_error - Unable to set selection range
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t format);
  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias EditSelectionProperty - Define selection range property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using EditSelectionProperty = Property<EditSelectionPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_EDIT_SELECTION_PROPERTY_H
