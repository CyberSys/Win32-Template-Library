//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\edit\EditModifiedProperty.hpp
//! \brief Separate class declaration for the Edit control 'Modified' property
//! \date 22 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EDIT_MODIFIED_H
#define WTL_EDIT_MODIFIED_H

#include <wtl/WTL.hpp>
#include <wtl/traits/EncodingTraits.hpp>              //!< Encoding
#include <wtl/windows/PropertyImpl.hpp>               //!< PropertyImpl

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  //! Forward declaration
  template <Encoding ENC>
  struct Edit;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct EditModifiedPropertyImpl - Provides the getters and setters for the edit control 'Modified' property
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EditModifiedPropertyImpl : PropertyImpl<ENC,bool,Edit<ENC>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = EditModifiedPropertyImpl;

    //! \alias base - Define base type
    using base = PropertyImpl<ENC,bool,Edit<ENC>>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;
    
    //! \alias window_t - Inherit window type
    using window_t = typename base::window_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // EditModifiedPropertyImpl::EditModifiedPropertyImpl
    //! Create window property 
    //! 
    //! \param[in,out] &wnd - Owner window
    /////////////////////////////////////////////////////////////////////////////////////////
    EditModifiedPropertyImpl(window_t& wnd) : base(wnd, false)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // EditModifiedPropertyImpl::get const
    //! Get the 'modified' state
    //! 
    //! \return value_t - Current 'modified' state if edit exists, otherwise false
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // EditModifiedPropertyImpl::set 
    //! Set the current 'Modified' state iff edit exists, otherwise this has no effect
    //! 
    //! \param[in] state - New 'Modified' state
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t state);

  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias EditModifiedProperty - Defines type of Edit control 'Modified' property
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using EditModifiedProperty = Property<EditModifiedPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_EDIT_MODIFIED_H
