//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\PropertyImpl.hpp
//! \brief Property implementation base
//! \date 25 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_PROPERTY_IMPL_HPP
#define WTL_PROPERTY_IMPL_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EnumTraits.hpp"            //!< is_attribute
#include <utility>                              //!< std::forward
#include <type_traits>                          //!< std::enable_if, std::conditional

//! \namespace wtl - Windows template library
namespace wtl 
{
  // Forward declaration
  template <Encoding ENC>
  struct WindowBase;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct PropertyImpl - Acts as a base class for classes that provides the getter/setters for a property
  //! 
  //! \tparam ENC - Window encoding
  //! \tparam VALUE - Value type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename VALUE>
  struct PropertyImpl
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = PropertyImpl<ENC, VALUE>;
    
    //! \alias value_t - Define value type
    using value_t = VALUE;

    //! \alias window_t - Define window type
    using window_t = WindowBase<ENC>;
    
    //! \var encoding - Define window character encoding
    static constexpr Encoding encoding = ENC;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    value_t    Value;       //!< Value storage
    window_t&  Window;      //!< Owner window

    // ------------------------------------- CONSTRUCTION -----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // PropertyImpl::PropertyImpl
    //! Stores the owner window and optionally sets the initial property value
    //! 
    //! \param[in,out] &wnd - Owner window
    //! \param[in] &&... args - [optional] Initial property value (or constructor arguments)
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS> explicit 
    PropertyImpl(window_t& wnd, ARGS&&... args) : Value(std::forward<ARGS>(args)...),
                                                  Window(wnd)
    {
    }

    // -------------------------------- COPY, MOVE & DESTROY  -------------------------------

    ENABLE_COPY_CTOR(PropertyImpl);       //!< Can be cloned
    DISABLE_COPY_ASSIGN(PropertyImpl);    //!< Contains a reference type
    ENABLE_POLY(PropertyImpl);            //!< Can be polymorphic
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // PropertyImpl::get const
    //! Fallback accessor for the property value.
    //!
    //! \return value_t - Current value
    //! 
    //! \remarks Since value_t may not be a copyable type, this method must be declared as a template
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = void>
    value_t  get() const
    {
      return Value;
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // PropertyImpl::set 
    //! Fallback mutator for the property value. 
    //! 
    //! \param[in] val - New value 
    //!
    //! \remarks Since this operation may not be supported by value_t, this method must be declared as a template
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = void>
    void  set(value_t val) 
    {
      Value = val;
    }
  };

      
} // namespace wtl

#endif // WTL_PROPERTY_IMPL_HPP
