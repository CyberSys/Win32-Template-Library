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
  //! \tparam ACCESS - Access type(s)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename VALUE, PropertyAccess ACCESS>
  struct PropertyImpl
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = PropertyImpl<VALUE,ACCESS>;
    
    //! \alias reference_t - Define lvalue reference type
    using reference_t = VALUE;  //const VALUE&;

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
    //! \remarks Using this method requires read access
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
    //! \remarks Using this method requires write access
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
  //! \tparam INTERNAL1 - [optional] Type permitted to perform internal mutation
  //! \tparam INTERNAL2 - [optional] Type permitted to perform internal mutation
  //! 
  //! \remarks Equality, comparison, and logical operations are implemented as non-member operators
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename IMPL, typename INTERNAL1 = void, typename INTERNAL2 = void>
  struct Property 
  {
    friend INTERNAL1;     //!< Extended friend permits optional internal access
    friend INTERNAL2;     //!< Extended friend permits optional internal access

    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = Property<IMPL,INTERNAL1,INTERNAL2>;
    
    //! \alias impl_t - Define implementation type
    using impl_t = IMPL;

    //! \alias reference_t - Inherit reference type
    using reference_t = typename IMPL::reference_t;

    //! \alias value_t - Inherit value type
    using value_t = typename IMPL::value_t;
    
    //! \var read - Inherit whether property supports read access
    static constexpr bool read = IMPL::read;

    //! \var write - Inherit whether property supports write access
    static constexpr bool write = IMPL::write;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    impl_t   Impl;     //!< Implementation provider

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

    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::operator == const
    //! 
    //! \tparam T - Any type
    //! 
    //! \param[in] && val - Value to compare against
    //! \return bool - True iff equal
    /////////////////////////////////////////////////////////////////////////////////////////
    /*template <typename T>
    bool  operator == (T&& val)
    {
      return Impl.get() == val;
    }*/
    
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
    type&  operator = (T&& val) //-> enable_if_is_not_t<type,T,type&>
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
  //! \param[in] val - Value to compare against
  //! \return bool - True iff equal
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename IMPL, typename F1, typename F2, typename T>
  bool  operator == (Property<IMPL,F1,F2>& p, T&& val)
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
  //! \param[in] val - Value to compare against
  //! \return bool - True iff unequal
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename IMPL, typename F1, typename F2, typename T>
  bool  operator != (Property<IMPL,F1,F2>& p, T&& val)
  {
    return p.get() != val;
  }

  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator | 
  //! Provides bitwise-OR operations on properties that support it
  //! 
  //! \tparam IMPL - Property implementation type
  //! \tparam F1 - [optional] First property friend
  //! \tparam F2 - [optional] Second property friend
  //! 
  //! \param[in,out] &p - Property
  //! \param[in] v - Value to combine
  //! \return value_t - Combination of 'v' and value of 'p'
  /////////////////////////////////////////////////////////////////////////////////////////
  /*template <typename IMPL, typename F1, typename F2, typename T>
  typename IMPL::value_t  operator | (Property<IMPL,F1,F2>& p, T&& val)
  {
    return p.get() | v;
  }*/

  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::operator |=
  //! Provides bitwise-OR assignment upon properties that support bitwise-OR operations
  //! 
  //! \tparam IMPL - Property implementation type
  //! \tparam F1 - [optional] First property friend
  //! \tparam F2 - [optional] Second property friend
  //! 
  //! \param[in,out] &p - Property
  //! \param[in] v - Value to combine
  //! \return Property& - Reference to updated 'p' 
  /////////////////////////////////////////////////////////////////////////////////////////
  /*template <typename IMPL, typename F1, typename F2>
  Property<IMPL,F1,F2>&  operator |= (Property<IMPL,F1,F2>& p, typename IMPL::value_t v)
  {
    return p = p.get() | v;
  }*/

  
      
} // namespace wtl

#endif // WTL_PROPERTY_HPP
