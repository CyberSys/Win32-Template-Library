////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\Property.hpp
//! \brief Property type
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_PROPERTY_HPP
#define WTL_PROPERTY_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct PropertyData - Encapsulates property value storage
  //! 
  //! \tparam VALUE - Value type
  ///////////////////////////////////////////////////////////////////////////////
  template <typename VALUE>
  struct PropertyData
  {
    // ------------------- TYPES & CONSTANTS -------------------
    
    //! \alias value_type - Define value type
    using value_type = VALUE;

    //! \alias type - Define our type
    using type = PropertyData<VALUE>;

    // ----------------------- REPRESENTATION ------------------------
  protected:
    value_type   Value;      //!< Value

    // --------------------- CONSTRUCTION ----------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // PropertyData::PropertyData
    //! Create a property with a default value
    ///////////////////////////////////////////////////////////////////////////////
    PropertyData() : Value(zero<value_type>::value)
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // PropertyData::PropertyData
    //! Create a property with an initial value
    //! 
    //! \param[in] value - Initial value
    ///////////////////////////////////////////////////////////////////////////////
    explicit PropertyData(value_type value) : Value(value)
    {}

    // ---------------------- ACCESSORS ------------------------			
    
    ///////////////////////////////////////////////////////////////////////////////
    // PropertyData::get const
    //! PropertyData accessor
    //! 
    //! \return value_type - Value
    ///////////////////////////////////////////////////////////////////////////////
    value_type  get() const
    {
      return Value;
    }

    // ----------------------- MUTATORS ------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // PropertyData::set 
    //! PropertyData mutator
    //! 
    //! \param[in] const& value - New value
    ///////////////////////////////////////////////////////////////////////////////
    void set(value_type value) 
    {
      Value = value;
    }
  };

  ///////////////////////////////////////////////////////////////////////////////
  //! \struct Property - Encapsulates a value and provides change notification
  //! 
  //! \tparam VALUE - Value type
  //! \tparam PROVIDER - [optional] Data provider type
  ///////////////////////////////////////////////////////////////////////////////
  template <typename VALUE, typename PROVIDER = PropertyData<VALUE>>
  struct Property 
  {
    // ------------------- TYPES & CONSTANTS -------------------
    
    //! \alias ChangeEvent - Define 'ValueChanged' event
    using ChangeEvent = Event<void>;

    //! \alias delegate_t - Define delegate type
    using delegate_t = typename ChangeEvent::delegate_t;

    //! \alias value_type - Define value type
    using value_type = VALUE;
    
    //! \alias type - Define our type
    using type = Property<VALUE>;

  protected:
    //! \alias provider_t - Define data provider type
    using provider_t = PROVIDER;

    // ----------------------- REPRESENTATION ------------------------
  public:
    ChangeEvent  Changed;     //!< Raised after value changes

  protected:
    provider_t   Data;        //!< Value

    // --------------------- CONSTRUCTION ----------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // Property::Property
    //! Create a property with a default value
    ///////////////////////////////////////////////////////////////////////////////
    Property() 
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // Property::Property
    //! Create a property with an initial value
    //! 
    //! \param[in] value - Initial value
    ///////////////////////////////////////////////////////////////////////////////
    explicit Property(value_type value) : Data(value)
    {}

    DEFAULT_COPY_CTOR(Property);    //!< Shallow reference copy
    DEFAULT_MOVE_CTOR(Property);    //!< Shallow reference copy

    ///////////////////////////////////////////////////////////////////////////////
    // Property::~Property
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~Property() 
    {}
    
    // ---------------------- ACCESSORS ------------------------			
    
    ///////////////////////////////////////////////////////////////////////////////
    // Property::get const
    //! Property accessor
    //! 
    //! \return value_type - Value
    ///////////////////////////////////////////////////////////////////////////////
    value_type  get() const
    {
      return Data.get();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Property::operator value_type const
    //! Implicit user conversion to value_type
    //! 
    //! \return value_type - Value
    ///////////////////////////////////////////////////////////////////////////////
    operator value_type() const
    {
      return get();
    }

    // ----------------------- MUTATORS ------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // Property::set 
    //! Property mutator
    //! 
    //! \param[in] value - New value
    ///////////////////////////////////////////////////////////////////////////////
    void set(value_type value) 
    {
      Data.set(value);
      Changed();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Property::operator =
    //! Assign from value
    //! 
    //! \param[in] value - New value
    //! \return type& - Reference to self
    ///////////////////////////////////////////////////////////////////////////////
    type& operator = (value_type value) 
    {
      set(value);
      return *this;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Property::operator =
    //! Assign from property
    //! 
    //! \param[in] const& r - Another property
    //! \return type& - Reference to self
    ///////////////////////////////////////////////////////////////////////////////
    type& operator = (const type& r) 
    {
      set(r.Value);
      return *this;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Property::operator =
    //! Assign from property
    //! 
    //! \param[in] && r - Another property
    //! \return type& - Reference to self
    ///////////////////////////////////////////////////////////////////////////////
    type& operator = (type&& r) 
    {
      set(r.Value);
      return *this;
    }
  };
  
}

#endif // WTL_PROPERTY_HPP
