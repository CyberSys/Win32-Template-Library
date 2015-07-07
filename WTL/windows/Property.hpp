//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\Property.hpp
//! \brief Property type
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_PROPERTY_HPP
#define WTL_PROPERTY_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EnumTraits.hpp"            //!< is_attribute
#include "wtl/windows/Event.hpp"                //!< Event
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
    {}

    // -------------------------------- COPYING & DESTRUCTION -------------------------------

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
    template <typename = std::enable_if_t<read>>
    value_t  get() const
    {
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
    template <typename = std::enable_if_t<write>>
    void  set(value_t val) 
    {
      Value = val;
    }
  };

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Property - Provides a HLL style property for values of any type, and overloads convenient operators.
  //! 
  //! \tparam IMPL - Type providing the implementation of the property value
  //! 
  //! \remarks Equality, comparison, relational, logical operations etc. are implemented as non-member operators
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename IMPL>
  struct Property 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = Property<IMPL>;
    
    //! \alias implementation_t - Define implementation type
    using implementation_t = IMPL;

    //! \alias value_t - Inherit value type
    using value_t = typename IMPL::value_t;
    
    //! \var read - Inherit whether property supports read access
    static constexpr bool read = IMPL::read;

    //! \var write - Inherit whether property supports write access
    static constexpr bool write = IMPL::write;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    implementation_t   Impl;     //!< Implementation provider

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::Property
    //! Explicitly creates a property with an optional initial value
    //! 
    //! \param[in] &&... args - [optional] Property implementation constructor arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    explicit Property(ARGS&&... args) : Impl(std::forward<ARGS>(args)...)
    {}

    // -------------------------------- COPYING & DESTRUCTION -------------------------------
    
    DISABLE_COPY_CTOR(Property);       //!< Cannot be cloned
    DISABLE_MOVE_CTOR(Property);       //!< Cannot be moved
    ENABLE_POLY(Property);             //!< Can be polymorphic
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::get const
    //! Value accessor
    //! 
    //! \return value_t - Current value
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = std::enable_if_t<read>>
    value_t  get() const
    {
      return Impl.get();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::operator value_t const
    //! (Implicit) User conversion to value
    //! 
    //! \return value_t - Current value
    /////////////////////////////////////////////////////////////////////////////////////////
    operator value_t() const
    {
      return Impl.get();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::operator() const
    //! Another value accessor using function operator syntax
    //! 
    //! \return value_t - Current value
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t operator ()() const
    {
      return Impl.get();
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::set 
    //! Value mutator
    //! 
    //! \param[in] value - New value
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = std::enable_if_t<write>>
    void  set(value_t value) 
    {
      // Set value
      Impl.set(value);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::operator =
    //! Assignment operator
    //! 
    //! \param[in] const& r - Another property of equal type
    //! \return type& - Reference to self
    /////////////////////////////////////////////////////////////////////////////////////////
    type&  operator = (const type& r) 
    {
      Impl.set(r.get());
      return *this;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::operator =
    //! Delegating assignment operator - delegates assignment logic to the implementation (for all types except self)
    //! 
    //! \tparam T - Any type
    //! 
    //! \param[in] && val - (Forwarding-reference) New value 
    //! \return type& - Reference to self
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    type&  operator = (T&& val) 
    {
      Impl.set(std::forward<T>(val));     //!< Delegate to implementation
      return *this;
    }
    
  };

  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator ==
  //! Non-member equality operator for Property types
  //! 
  //! \tparam IMPL - Property implementation type
  //! \tparam T - Any type
  //! 
  //! \param[in] const& p - Property
  //! \param[in] && val - Value to compare against
  //! \return bool - True iff equal
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename IMPL, typename T>
  bool  operator == (Property<IMPL>& p, T&& val)
  {
    return p.get() == val;
  }
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator !=
  //! Non-member inequality operator for Property types
  //! 
  //! \tparam IMPL - Property implementation type
  //! \tparam T - Any type
  //! 
  //! \param[in] const& p - Property
  //! \param[in] && val - Value to compare against
  //! \return bool - True iff unequal
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename IMPL, typename T>
  bool  operator != (Property<IMPL>& p, T&& val)
  {
    return p.get() != val;
  }

  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator | 
  //! Non-member bitwise-OR operator for Property types that support bitwise-OR
  //! 
  //! \tparam IMPL - Property implementation type
  //! \tparam T - Any type
  //! 
  //! \param[in] &p - Property
  //! \param[in] && val - Value to combine
  //! \return Property::value_t - Combination of 'v' and value of 'p'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename IMPL, typename T>
  typename IMPL::value_t  operator | (const Property<IMPL>& p, T&& val)
  {
    return p.get() | val;
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::operator |=
  //! Non-member bitwise-OR assignment operator for Property types that support bitwise-OR assignment
  //! 
  //! \tparam IMPL - Property implementation type
  //! \tparam T - Any type
  //! 
  //! \param[in,out] &p - Property
  //! \param[in] && val - Value to combine
  //! \return Property& - Reference 'p' whose value is now combined with 'val'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename IMPL, typename T>
  Property<IMPL>&  operator |= (Property<IMPL>& p, T&& val)
  {
    return p = p.get() | val;
  }

  
      
} // namespace wtl

#endif // WTL_PROPERTY_HPP
