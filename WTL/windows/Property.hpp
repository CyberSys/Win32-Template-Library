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
    
    //! \alias type - Define our type
    using type = PropertyData<VALUE>;
    
    //! \alias reference_t - Define immutable reference type
    using reference_t = const VALUE&;

    //! \alias value_t - Define value type
    using value_t = VALUE;

    // ----------------------- REPRESENTATION ------------------------
  protected:
    value_t   Value;      //!< Property value

    // --------------------- CONSTRUCTION ----------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // PropertyData::PropertyData
    //! Create a property with an initial value
    //! 
    //! \param[in] const& value - Initial value
    ///////////////////////////////////////////////////////////////////////////////
    explicit PropertyData(reference_t value) : Value(value)
    {}

    // ---------------------- ACCESSORS ------------------------			
    
    ///////////////////////////////////////////////////////////////////////////////
    // PropertyData::get const
    //! Value accessor
    //! 
    //! \return reference_t - Immutable reference to value
    ///////////////////////////////////////////////////////////////////////////////
    virtual reference_t  get() const
    {
      return Value;
    }

    // ----------------------- MUTATORS ------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // PropertyData::set 
    //! Value mutator
    //! 
    //! \param[in] const& value - New value
    ///////////////////////////////////////////////////////////////////////////////
    virtual void set(reference_t value) 
    {
      Value = value;
    }
  };

  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct Property - Encapsulates any value with getter/setters and provides change notification
  //! 
  //! \tparam VALUE - Value type
  //! \tparam MUTABLE - [optional] Whether property can be changed (Default is true)
  //! \tparam PROVIDER - [optional] Property data provider type
  ///////////////////////////////////////////////////////////////////////////////
  template <typename VALUE, bool MUTABLE = true, typename PROVIDER = PropertyData<VALUE>>
  struct Property 
  {
    // ------------------- TYPES & CONSTANTS -------------------
    
    //! \alias type - Define our type
    using type = Property<VALUE,MUTABLE,PROVIDER>;

    //! \alias ChangedEvent - Defines post-update event
    using ChangedEvent = Event<void>;

    //! \alias ChangingEvent - Defines pre-update event
    using ChangingEvent = Event<bool,const VALUE&,const VALUE&>;
    
    //! \alias reference_t - Define immutable reference type
    using reference_t = const VALUE&;

    //! \alias value_t - Define value type
    using value_t = VALUE;

    //! \var readonly - Define whether property is read-only
    static constexpr bool readonly = !MUTABLE;

  protected:
    //! \alias provider_t - Define data provider type
    using provider_t = PROVIDER;

    // ----------------------- REPRESENTATION ------------------------
  public:
    ChangedEvent   Changed;     //!< Raised after value changes
    ChangingEvent  Changing;    //!< Raised before value changes

  protected:
    provider_t     Data;        //!< Value provider

    // --------------------- CONSTRUCTION ----------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // Property::Property
    //! Create a property with an initial value
    //! 
    //! \param[in] value - Initial value
    ///////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    explicit Property(ARGS&&... args) : Data(std::forward<ARGS>(args)...)
    {}

    DISABLE_COPY(Property);       //!< Cannot be copied
    DISABLE_MOVE(Property);       //!< Cannot be moved

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
    //! \return reference_t - Immutable reference to value
    ///////////////////////////////////////////////////////////////////////////////
    reference_t  get() const
    {
      return Data.get();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Property::operator reference_t const
    //! Implicit user conversion to reference_t
    //! 
    //! \return reference_t - Immutable reference to value
    ///////////////////////////////////////////////////////////////////////////////
    operator reference_t() const
    {
      return get();
    }

    // ----------------------- MUTATORS ------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // Property::set 
    //! Property mutator
    //! 
    //! \param[in] const& value - New value
    ///////////////////////////////////////////////////////////////////////////////
    std::enable_if_t<!readonly>  set(reference_t value) 
    {
      // Raise 'Changing'
      if (Changing(Data.get(), value))
      {
        // Set and raise 'Changed'
        Data.set(value);
        Changed();
      }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Property::operator =
    //! Assign from value
    //! 
    //! \param[in] const& value - New value
    //! \return type& - Reference to self
    ///////////////////////////////////////////////////////////////////////////////
    type& operator = (reference_t value) 
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
    /*type& operator = (type&& r) 
    {
      set(r.Value);
      return *this;
    }*/
  };
  

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias PropertyChangedEventHandler - Define 'Property Value Changed' event delegate type
  //! 
  //! \tparam VALUE - Value type
  //! \tparam MUTABLE - [optional] Whether property can be changed (Default is true)
  //! \tparam PROVIDER - [optional] Property data provider type
  ///////////////////////////////////////////////////////////////////////////////
  template <typename VALUE, bool MUTABLE = true, typename PROVIDER = PropertyData<VALUE>>
  using PropertyChangedEventHandler = EventHandler< typename Property<VALUE,MUTABLE,PROVIDER>::ChangedEvent >;
    

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias PropertyChangingEventHandler - Define 'Property Value Changing' event delegate type
  //! 
  //! \tparam VALUE - Value type
  //! \tparam MUTABLE - [optional] Whether property can be changed (Default is true)
  //! \tparam PROVIDER - [optional] Property data provider type
  ///////////////////////////////////////////////////////////////////////////////
  template <typename VALUE, bool MUTABLE = true, typename PROVIDER = PropertyData<VALUE>>
  using PropertyChangingEventHandler = EventHandler< typename Property<VALUE,MUTABLE,PROVIDER>::ChangingEvent >;
  
    
}

#endif // WTL_PROPERTY_HPP
