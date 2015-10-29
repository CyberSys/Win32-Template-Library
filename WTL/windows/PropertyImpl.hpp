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
  //! \enum PropertyAccess - Defines property access
  enum class PropertyAccess : int32_t 
  { 
    Read = 1,                   //!< Read access
    Write = 2,                  //!< Write access
    ReadWrite = Read|Write,     //!< Read & write access
  };

  //! Define traits: Contiguous Attribute
  template <> struct is_attribute<PropertyAccess>  : std::true_type {};
  template <> struct is_contiguous<PropertyAccess> : std::true_type {};


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct PropertyImpl - Encapsulates property value access & mutation. Typically used as a base class.
  //! 
  //! \tparam VALUE - Value type
  //! \tparam ACCESS - Access type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename VALUE, PropertyAccess ACCESS>
  struct PropertyImpl
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = PropertyImpl<VALUE,ACCESS>;
    
    //! \alias value_t - Define value type
    using value_t = VALUE;

    //! \var read - Define whether property supports read access
    static constexpr bool read = ACCESS && PropertyAccess::Read;

    //! \var write - Define whether property supports write access
    static constexpr bool write = ACCESS && PropertyAccess::Write;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    value_t   Value;      //!< Value storage

    // ------------------------------------- CONSTRUCTION -----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // PropertyImpl::PropertyImpl
    //! Create implementation, optionally with initial value
    //! 
    //! \param[in] &&... args - [optional] Value constructor arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    explicit PropertyImpl(ARGS&&... args) : Value(std::forward<ARGS>(args)...)
    {
    }

    // -------------------------------- COPY, MOVE & DESTROY  -------------------------------

    ENABLE_COPY_CTOR(PropertyImpl);       //!< Can be cloned
    DISABLE_COPY_ASSIGN(PropertyImpl);    //!< Value mutator cannot be circumvented
    ENABLE_POLY(PropertyImpl);            //!< Can be polymorphic
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // PropertyImpl::get const
    //! Value accessor
    //! 
    //! \return value_t - Current value
    //! 
    //! \remarks Using this method requires read access. It is not virtual by design, it may be excluded from the candidate pool by an SFINAE expression.
    //! \remarks Derived implementations should customize behaviour via custom methods (ie. template name-lookup) rather than simple sub-type polymorphism.
    /////////////////////////////////////////////////////////////////////////////////////////
    //template <typename = std::enable_if_t<read>>
    template <typename = void>
    value_t  get() const
    {
      static_assert(read, "Property does not support reading");

      return Value;
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // PropertyImpl::set 
    //! Value mutator
    //! 
    //! \param[in] val - New value 
    //! 
    //! \remarks Using this method requires write access. It is not virtual by design, it may be excluded from the candidate pool by an SFINAE expression.
    //! \remarks Derived implementations should customize behaviour via custom methods (ie. template name-lookup) rather than simple sub-type polymorphism.
    /////////////////////////////////////////////////////////////////////////////////////////
    //template <typename = std::enable_if_t<write>>
    template <typename = void>
    void  set(value_t val) 
    {
      static_assert(read, "Property does not support writing");

      Value = val;
    }
  };

      
} // namespace wtl

#endif // WTL_PROPERTY_IMPL_HPP
