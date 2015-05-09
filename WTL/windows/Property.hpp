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

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \enum PropertyType - Defines property types
  enum class PropertyType : int32 
  { 
    Mutable = 0,        //!< Read/Write
    Immutable = 1,      //!< Read only
    Reference = 2,      //!< Reference type
    Value = 4,          //!< Value type
    
    MutableRef = Mutable|Reference, 
    MutableValue = Mutable|Value,
    ImmutableValue = Immutable|Value,
    ImmutableReference = Immutable|Reference,
  };

  //! Define traits: Non-contiguous Attribute
  template <> struct is_attribute<PropertyType>  : std::true_type  {};
  template <> struct is_contiguous<PropertyType> : std::false_type {};

  //! Define limits traits
  template <> struct max_value<PropertyType>     : std::integral_constant<PropertyType,PropertyType::Value>   {};
  template <> struct min_value<PropertyType>     : std::integral_constant<PropertyType,PropertyType::Mutable> {};


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct PropertyImpl - Encapsulates a property value
  //! 
  //! \tparam VALUE - External value type
  //! \tparam TYPE - Accessibility and representation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename VALUE, PropertyType TYPE>
  struct PropertyImpl
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = PropertyImpl<VALUE,TYPE>;

    //! \alias reference_t - Define immutable reference type
    using reference_t = const VALUE&;

    //! \alias value_t - Define value type
    using value_t = VALUE;

    //! \var readonly - Define whether property is read-only or mutable
    static constexpr bool readonly = TYPE && PropertyType::Immutable;

    //! \var readonly - Define whether property is a reference or value type
    static constexpr bool reference = !(TYPE && PropertyType::Value);

    //! \alias argument_t - Define accessor/mutator argument type (Value vs Reference)
    using argument_t = std::conditional_t<reference, reference_t, value_t>;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    value_t   Value;      //!< Property value  

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // PropertyImpl::PropertyImpl
    //! Create initial value
    //! 
    //! \param[in] &&... args - [optional] Value constructor arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    explicit PropertyImpl(ARGS&&... args) : Value(std::forward<ARGS>(args)...)
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // PropertyImpl::~PropertyImpl
    //! Virtual d-tor
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~PropertyImpl() = default;

    DEFAULT_COPY(PropertyImpl);       //!< Copy semantics determined by value type
    DEFAULT_MOVE(PropertyImpl);       //!< Move semantics determined by value type

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // PropertyImpl::get const
    //! Value accessor
    //! 
    //! \return auto - Value  or  immutable reference to value
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual argument_t  get() const
    {
      return Value;
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // PropertyImpl::set 
    //! Value mutator
    //! 
    //! \param[in] auto value - New value  or  immutable reference to new value
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual void  set(argument_t value) 
    {
      Value = value;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::init
    //! Initialise value 
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual void  init(argument_t value) 
    {
      Value = value;
    }
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct PropertyFunctor - Encapsulates property mutation within a delegate 
  //! 
  //! \tparam VALUE - External value type
  //! \tparam TYPE - Property properties 
  /////////////////////////////////////////////////////////////////////////////////////////
  //template <typename VALUE, PropertyType TYPE>
  //struct PropertyFunctor : PropertyImpl<VALUE,TYPE>
  //{
  //  // ---------------------------------- TYPES & CONSTANTS ---------------------------------

  //  //! \alias base - Define base type
  //  using base = PropertyImpl<VALUE,TYPE>;

  //  //! \alias type - Define own type
  //  using type = PropertyFunctor<VALUE,TYPE>;

  //  //! \alias argument_t - Inherit accessor/mutator argument type (Value vs Reference)
  //  using argument_t = typename base::argument_t;

  //  //! \alias accessor_t - Define accessor delegate type
  //  using accessor_t = std::function<argument_t (void)>;

  //  //! \alias mutator_t - Define mutator delegate type
  //  using mutator_t = std::function<void (argument_t)>;

  //  // ----------------------------------- REPRESENTATION -----------------------------------
  //protected:
  //  accessor_t  Accessor;      //!< Accessor delegate
  //  mutator_t   Mutator;       //!< Mutator delegate

  //  // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
  //public:
  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  // PropertyFunctor::PropertyFunctor
  //  //! Create from accessor/mutator delegates and initialize value
  //  //! 
  //  //! \param[in] get - Accessor delegate
  //  //! \param[in] set - Mutator delegate
  //  //! \param[in] &&... args - [optional] Value constructor arguments
  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  template <typename... ARGS>
  //  PropertyFunctor(accessor_t get, mutator_t set, ARGS&&... args) : base(std::forward<ARGS>(args)...),
  //                                                                   Accessor(set), 
  //                                                                   Mutator(get)
  //  {}
  //  
  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  // PropertyFunctor::PropertyFunctor
  //  //! Create from mutator delegate and initialize value
  //  //! 
  //  //! \param[in] set - Mutator delegate
  //  //! \param[in] &&... args - [optional] Value constructor arguments
  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  template <typename... ARGS>
  //  PropertyFunctor(mutator_t set, ARGS&&... args) : base(std::forward<ARGS>(args)...),
  //                                                   Accessor(std::bind(&this::get, this)), 
  //                                                   Mutator(set)
  //  {}

  //  DISABLE_COPY(PropertyFunctor);       //!< Copy semantics determined by value type
  //  DISABLE_MOVE(PropertyFunctor);       //!< Move semantics determined by value type
  //  
  //  // ---------------------------------- ACCESSOR METHODS ----------------------------------			

  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  // PropertyFunctor::get const
  //  //! Value accessor
  //  //! 
  //  //! \return auto - Value  or  immutable reference to value
  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  argument_t  get() const override
  //  {
  //    return Accessor();
  //  }

  //  // ----------------------------------- MUTATOR METHODS ----------------------------------

  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  // PropertyFunctor::set 
  //  //! Value mutator
  //  //! 
  //  //! \param[in] auto value - New value  or  immutable reference to new value
  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  void  set(argument_t value)  override
  //  {
  //    // Mutator and update value
  //    Mutator(value);
  //    base::set(value);
  //  }
  //};

  /*template <typename VALUE, PropertyType TYPE, typename ACCESSOR, typename MUTATOR, typename... ARGS>
  PropertyFunctor<VALUE,TYPE>  make_property(ACCESSOR get, MUTATOR set, ARGS&&... args)
  {
    return PropertyFunctor<VALUE,TYPE>(get, set, std::forward<ARGS>(args)...);
  }*/


  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Property - Encapsulates any value with getter/setters. Provides update verification and change notification.
  //! 
  //! \tparam IMPL - Implementation type
  //! \tparam OWNER - [optional] Type permitted to perform internal mutation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename IMPL, typename OWNER = void>
  struct Property 
  {
    friend OWNER;     //!< Extended friend permits optional internal access

    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = Property<IMPL,OWNER>;

    //! \alias argument_t - Inherit argument type (Value vs Reference)
    using argument_t = typename IMPL::argument_t;

    //! \alias provider_t - Define implementation type
    using provider_t = IMPL;

    //! \alias reference_t - Inherit reference type
    using reference_t = typename IMPL::reference_t;

    //! \alias value_t - Inherit value type
    using value_t = typename IMPL::value_t;

    //! \alias ChangedEvent - Defines post-update event
    using ChangedEvent = Event<void>;

    //! \alias ChangingEvent - Defines pre-update event
    using ChangingEvent = Event<bool,argument_t,argument_t>;

    //! \var readonly - Define whether property is read-only or mutable
    static constexpr bool readonly = IMPL::readonly;

    //! \var reference - Define whether property is a reference or value type
    static constexpr bool reference = IMPL::reference;

    // ----------------------------------- REPRESENTATION -----------------------------------
  public:
    ChangedEvent   Changed;     //!< Raised after value changes
    ChangingEvent  Changing;    //!< Raised before value changes

  protected:
    provider_t     Impl;        //!< Implementation provider

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::Property
    //! Create property and implementation
    //! 
    //! \param[in] &&... args - [optional] Property implementation constructor arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    explicit Property(ARGS&&... args) : Impl(std::forward<ARGS>(args)...)
    {}

    DISABLE_COPY_CTOR(Property);       //!< Copy semantics determined by implementation
    DISABLE_MOVE_CTOR(Property);       //!< Move semantics determined by implementation

    /*DEFAULT_COPY_ASSIGN(Property);
    DEFAULT_MOVE_ASSIGN(Property);*/

    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::~Property
    //! Virtual d-tor
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~Property() 
    {}
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::get const
    //! Value accessor
    //! 
    //! \return auto - Value  or  immutable reference to value
    /////////////////////////////////////////////////////////////////////////////////////////
    argument_t  get() const
    {
      return Impl.get();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::operator argument_t const
    //! Implicit user conversion to value
    //! 
    //! \return auto - Value  or  immutable reference to value
    /////////////////////////////////////////////////////////////////////////////////////////
    operator argument_t() const
    {
      return get();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::operator() const
    //! Value accessor
    //! 
    //! \return auto - Value  or  immutable reference to value
    /////////////////////////////////////////////////////////////////////////////////////////
    argument_t operator ()() const
    {
      return get();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::operator-> const
    //! Value accessor
    //! 
    //! \return auto - Value  or  immutable reference to value
    /////////////////////////////////////////////////////////////////////////////////////////
    argument_t operator->() const
    {
      return get();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::operator == const
    //! Equality operator for values
    //! 
    //! \param[in] auto value - Value  
    //! \return bool - True iff equal
    /////////////////////////////////////////////////////////////////////////////////////////
    bool  operator == (argument_t value) const
    {
      return get() == value;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::operator == const
    //! Equality operator for properties
    //! 
    //! \param[in] auto r - Another property
    //! \return bool - True iff equal
    /////////////////////////////////////////////////////////////////////////////////////////
    bool  operator == (const type& r) const
    {
      return get() == r.get();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::operator != const
    //! Inequality operator for values
    //! 
    //! \param[in] auto value - Value  
    //! \return bool - True iff unequal
    /////////////////////////////////////////////////////////////////////////////////////////
    bool  operator != (argument_t value) const
    {
      return get() != value;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::operator != const
    //! Inequality operator for properties
    //! 
    //! \param[in] auto r - Another property
    //! \return bool - True iff unequal
    /////////////////////////////////////////////////////////////////////////////////////////
    bool  operator != (const type& r) const
    {
      return get() != r.get();
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::set 
    //! Value mutator
    //! 
    //! \param[in] auto value - New value  or  immutable reference to new value
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = std::enable_if_t<!readonly>>
    void  set(argument_t value) 
    {
      // Raise 'Changing'
      if (Changing.raise(Impl.get(), value))
      {
        // Set and raise 'Changed'
        Impl.set(value);
        Changed.raise();
      }
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::operator =
    //! Assign from value or reference
    //! 
    //! \param[in] auto value - New value  or  immutable reference to new value
    //! \return type& - Reference to self
    /////////////////////////////////////////////////////////////////////////////////////////
    type& operator = (argument_t value) 
    {
      set(value);
      return *this;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::operator =
    //! Assign from property
    //! 
    //! \param[in] const& r - Another property
    //! \return type& - Reference to self
    /////////////////////////////////////////////////////////////////////////////////////////
    type& operator = (const type& r) 
    {
      set(r.get());
      return *this;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::operator =
    //! Transfer from a property
    //! 
    //! \param[in] && r - Another property
    //! \return type& - Reference to self
    /////////////////////////////////////////////////////////////////////////////////////////
    type& operator = (type&& r) 
    {
      set(r.get());
      return *this;
    }

  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Property::init
    //! Initialise value without raising events
    //! 
    //! \param[in] &&... args - [optional] Value constructor arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    void  init(ARGS&&... args) 
    {
      // Initialise value
      Impl.init(std::forward<ARGS>(args)...);
    }

  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Property_t - Encapsulates any value with getter/setters. Provides update verification and change notification.
  //! 
  //! \tparam VALUE - Value type
  //! \tparam TYPE - Property type 
  //! \tparam INTERNAL - [optional] Internal representation type
  /////////////////////////////////////////////////////////////////////////////////////////
  /*template <typename VALUE, PropertyType TYPE, typename INTERNAL = VALUE>
  using Property_t = Property<PropertyImpl<VALUE,TYPE,INTERNAL>>;*/

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias PropertyChangedEventHandler - Define property 'Value Changed' event delegate type
  //! 
  //! \tparam VALUE - Value type
  //! \tparam PROVIDER - [optional] Property implementation provider type
  /////////////////////////////////////////////////////////////////////////////////////////
  /*template <typename PROPERTY>
  using PropertyChangedEventHandler = EventHandler< typename PROPERTY::ChangedEvent >;*/
    

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias PropertyChangingEventHandler - Define property 'Value Changing' event delegate type
  //! 
  //! \tparam VALUE - Value type
  //! \tparam PROVIDER - [optional] Property implementation provider type
  /////////////////////////////////////////////////////////////////////////////////////////
  /*template <typename PROPERTY>
  using PropertyChangingEventHandler = EventHandler< typename PROPERTY::ChangingEvent >;*/
  
    
}

#endif // WTL_PROPERTY_HPP
