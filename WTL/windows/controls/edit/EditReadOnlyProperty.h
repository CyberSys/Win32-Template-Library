//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\checkbox\EditReadOnlyProperty.hpp
//! \brief Separate class declaration for the Edit control 'ReadOnly' property
//! \date 22 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EDIT_READONLY_PROPERTY_H
#define WTL_EDIT_READONLY_PROPERTY_H

#include <wtl/WTL.hpp>
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
  struct Edit;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct EditReadOnlyPropertyImpl - Provides the getters and setters for the edit control 'ReadOnly' property
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EditReadOnlyPropertyImpl : PropertyImpl<ENC,bool,Edit<ENC>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = EditReadOnlyPropertyImpl;

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
    // EditReadOnlyPropertyImpl::EditReadOnlyPropertyImpl
    //! Create window property 
    //! 
    //! \param[in,out] &wnd - Owner window
    /////////////////////////////////////////////////////////////////////////////////////////
    EditReadOnlyPropertyImpl(window_t& wnd) : base(wnd, bool())
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // EditReadOnlyPropertyImpl::get const
    //! Query whether the text is read-only
    //! 
    //! \return value_t - Current 'readonly' state if edit exists, otherwise initial state
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // EditReadOnlyPropertyImpl::set 
    //! Set the 'ReadOnly' state iff edit exists, otherwise sets the initial state
    //! 
    //! \param[in] state - New 'ReadOnly' state
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t state);
  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias EditReadOnlyProperty - Defines type of Edit control 'ReadOnly' property
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using EditReadOnlyProperty = Property<EditReadOnlyPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_EDIT_READONLY_PROPERTY_H
