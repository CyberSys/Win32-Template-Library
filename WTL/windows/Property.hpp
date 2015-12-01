//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\Property.hpp
//! \brief Property type
//! \date 25 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_PROPERTY_HPP
#define WTL_PROPERTY_HPP

#include <wtl/WTL.hpp>
#include <wtl/traits/EnumTraits.hpp>            //!< is_attribute
#include <utility>                              //!< std::forward
#include <type_traits>                          //!< std::enable_if, std::conditional

//! \namespace wtl - Windows template library
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \namespace concepts - Defines concepts used by property
  /////////////////////////////////////////////////////////////////////////////////////////
  namespace concepts
  {
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct PropertyGetter - Defines a concept requiring non-static method: 'value_type get()'
    //! 
    //! \tparam VAL - Value type
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename VAL>  
    struct PropertyGetter
    {
      template <typename IMPL, VAL (IMPL::*)() const = &IMPL::get> 
      static void* test( void* );
    };

    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct PropertySetter - Defines a concept requiring non-static method: 'void set(value_type)'
    //! 
    //! \tparam VAL - Value type
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename VAL>  
    struct PropertySetter
    {
      template <typename IMPL, void (IMPL::*)(VAL) = &IMPL::set> 
      static void* test( void* );
    };
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Property - Provides a wrapper for a high-level language style window property of any type.
  //! 
  //! \tparam IMPL - Type providing the implementation 
  //! 
  //! \remarks Provides the boilerplate operator overloads, such as equality, bitwise, and logical operations.
  //! \remarks Whether these are supported is determined by a separate implementation, which provides the get() and set() methods.
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename IMPL>
  struct Property : IMPL
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = Property<IMPL>;
    
    //! \alias base - Define base/implementation type
    using base = IMPL;
    
    //! \alias value_t - Inherit value type
    using value_t = typename IMPL::value_t;
    
    //! \alias window_t - Inherit window type
    using window_t = typename IMPL::window_t;
    
    //! \var read - Define whether property supports read access
    static constexpr bool read = requires<IMPL,concepts::PropertyGetter<value_t>>::value; 

    //! \var write - Define whether property supports write access
    static constexpr bool write = requires<IMPL,concepts::PropertySetter<value_t>>::value;
    
    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::Property
    //! Explicitly creates a property with an optional initial value
    //! 
    //! \param[in,out] &wnd - Owner window
    //! \param[in] &&... args - [optional] Property implementation constructor arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS> explicit
    Property(window_t& wnd, ARGS&&... args) : base(wnd, std::forward<ARGS>(args)...)
    {}

    // -------------------------------- COPY, MOVE & DESTROY  -------------------------------
    
    DISABLE_COPY_CTOR(Property);       //!< Cannot be cloned
    DISABLE_MOVE_CTOR(Property);       //!< Cannot be moved
    ENABLE_POLY(Property);             //!< Can be polymorphic
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::operator value_t const
    //! (Implicit) User conversion to value
    //! 
    //! \return value_t - Current value
    /////////////////////////////////////////////////////////////////////////////////////////
    operator value_t() const
    {
      return this->get();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::operator() const
    //! Another value accessor using function operator syntax
    //! 
    //! \return value_t - Current value
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t operator ()() const
    {
      return this->get();
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::operator value_t const
    //! (Implicit) User conversion to value
    //! 
    //! \return value_t - Current value
    /////////////////////////////////////////////////////////////////////////////////////////
    operator value_t() 
    {
      return this->get();
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
      this->set(r.get());
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
      this->set(std::forward<T>(val));     //!< Delegate to implementation
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
  //! \param[in] const &p - Property
  //! \param[in] && val - Value to combine
  //! \return Property::value_t - Result of 'p | val'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename IMPL, typename T>
  typename IMPL::value_t  operator | (const Property<IMPL>& p, T&& val)
  {
    return p.get() | val;
  }
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator & 
  //! Non-member bitwise-AND operator for Property types that support bitwise-AND
  //! 
  //! \tparam IMPL - Property implementation type
  //! \tparam T - Any type
  //! 
  //! \param[in] const &p - Property
  //! \param[in] && val - Value to combine
  //! \return Property::value_t - Result of 'p & val'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename IMPL, typename T>
  typename IMPL::value_t  operator & (const Property<IMPL>& p, T&& val)
  {
    return p.get() & val;
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

  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator & 
  //! Non-member logical-AND operator for Property types that support logical-AND
  //! 
  //! \tparam IMPL - Property implementation type
  //! \tparam T - Any type
  //! 
  //! \param[in] const &p - Property
  //! \param[in] && val - Value to combine
  //! \return bool - Result of 'p && val'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename IMPL, typename T>
  bool  operator && (const Property<IMPL>& p, T&& val)
  {
    return p.get() && val;
  }

      
} // namespace wtl

#endif // WTL_PROPERTY_HPP
