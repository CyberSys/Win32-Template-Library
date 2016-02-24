//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Handle.hpp
//! \brief Encapsulate any handle type
//! \date 8 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_HANDLE_HPP
#define WTL_HANDLE_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/Exception.hpp>            //!< Exceptions
#include <wtl/utils/Requires.hpp>             //!< requires
#include <utility>                            //!< std::forward
#include <memory>                             //!< std::shared_ptr

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \enum AllocType - Handle allocation type
  /////////////////////////////////////////////////////////////////////////////////////////
  enum class AllocType
  {
    Accquire,   //!< Handle was accquired
    Create,     //!< Handle was created
    WeakRef,    //!< Handle is not owned
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct NativeHandle - Associates handle and allocation method
  //!
  //! \tparam T - Any handle type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct NativeHandle
  {
    //! \typedef handle_t - Native handle type
    using handle_t = T;

    /////////////////////////////////////////////////////////////////////////////////////////
    // NativeHandle::NativeHandle
    //! Create and store native handle
    //! 
    //! \param[in] h - Native handle
    //! \param[in] at - Allocation method
    /////////////////////////////////////////////////////////////////////////////////////////
    NativeHandle(handle_t h, AllocType at) : Handle(h), Method(at)
    {}

    handle_t   Handle;      //!< Handle
    AllocType  Method;      //!< Allocation method
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc - Provides allocation/release for any handle type
  //!
  //! \tparam T - Any handle type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct handle_alloc;  /* Undefined */
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<::HANDLE> - Blocks specialization by types that are only typedefs for ::HANDLE 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<::HANDLE>
  {};

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct handle_traits - Defines handle traits
  //!
  //! \tparam T - Any handle type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct handle_traits
  {
    static constexpr bool cloneable = false;
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \namespace <anon> - Utility namespace
  /////////////////////////////////////////////////////////////////////////////////////////
  namespace
  {
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct converter - Handles the conversion between pointer/numeric native handle types and their stored form, [native_t*]
    //! 
    //! \tparam native_t - Handle type
    //! \tparam <anon> - SFINAE
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename native_t, typename = void>
    struct converter;

    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct converter - Converts between integral handle types and pointers
    //! 
    //! \tparam native_t - Integral Handle type
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename native_t>
    struct converter<native_t, enable_if_integral_t<native_t>>
    {
      /////////////////////////////////////////////////////////////////////////////////////////
      // Handle::toHandle
      //! Cast a pointer to a handle (of underlying integral type)
      //!
      //! \param[in] ptr - Handle previously cast to a pointer
      //! \return native_t - Actual handle
      /////////////////////////////////////////////////////////////////////////////////////////
      static native_t toHandle(native_t* ptr)
      {
        return static_cast<native_t>( reinterpret_cast<std::uintptr_t>(ptr) );
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      // Handle::toPointer
      //! Cast a handle (of underlying integral type) to a pointer
      //!
      //! \param[in] h - Actual handle
      //! \return native_t* - Handle cast to a pointer
      /////////////////////////////////////////////////////////////////////////////////////////
      static native_t* toPointer(native_t h)
      {
        return reinterpret_cast<native_t*>( static_cast<std::uintptr_t>(h) );
      }
    };

    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct converter - Converts between pointer handle types and pointers
    //! 
    //! \tparam native_t - Pointer Handle type
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename native_t>
    struct converter<native_t, enable_if_pointer_t<native_t>>
    {
      /////////////////////////////////////////////////////////////////////////////////////////
      // Handle::toHandle
      //! Cast a pointer to a handle (of underlying pointer type)
      //!
      //! \param[in] ptr - Handle previously cast to a pointer
      //! \return native_t - Actual handle
      /////////////////////////////////////////////////////////////////////////////////////////
      static native_t toHandle(native_t* ptr)
      {
        return reinterpret_cast<native_t>(ptr);
      }
      
      /////////////////////////////////////////////////////////////////////////////////////////
      // Handle::toPointer
      //! Cast a handle (of underlying pointer type) to a pointer
      //!
      //! \param[in] h - Actual handle
      //! \return native_t* - Handle cast to a pointer
      /////////////////////////////////////////////////////////////////////////////////////////
      static native_t* toPointer(native_t h)
      {
        return reinterpret_cast<native_t*>(h);
      }
    };
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \namespace concepts - Defines concepts used by handle
  /////////////////////////////////////////////////////////////////////////////////////////
  namespace concepts
  {
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct CloneableHandle - Defines a concept requiring static method: 'NativeHandle<T> clone(NativeHandle<T>)'
    //! 
    //! \tparam T - Native handle type
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct CloneableHandle
    {
      template <typename U, NativeHandle<T> (*)(NativeHandle<T>) = &U::clone> 
      static void* test( void* );
    };
    
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct ConstructibleHandle - Defines a concept requiring static 'create' method: 'NativeHandle<T> create(...)'
    //! 
    //! \tparam T - Native handle type
    //! \tparam ARGS... - Constructor argument types 
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename T, typename... ARGS>  
    struct ConstructibleHandle
    {
      template <typename U, typename = std::is_same<NativeHandle<T>,decltype(U::create( std::declval<ARGS>()... ))>>   //!< NB: Test whether U::create(..) is callable with ARGS...
      static void* test( void* );    
    };

    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct DestroyableHandle - Defines a concept requiring static method: 'bool destroy(NativeHandle<T>)'
    //! 
    //! \tparam T - Native handle type
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>  
    struct DestroyableHandle
    {
      template <typename U, bool (*)(NativeHandle<T>) noexcept = &U::destroy> 
      static void* test( void* );
    };

    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct StoreableHandle - Defines a concept requiring handle size lesser-or-equal in size to ::HANDLE
    //! 
    //! \tparam T - Native handle type
    /////////////////////////////////////////////////////////////////////////////////////////
    struct StoreableHandle
    {
      template <typename U, typename = std::enable_if_t<(sizeof(U) <= sizeof(::HANDLE))>> 
      static void* test( void* );
    };
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Handle - Provides a shared smart-pointer type for any handle type
  //!
  //! \tparam NATIVE - Native handle type which models the StoreableHandle concept
  //! \tparam ALLOCATOR - [optional] Handle allocator type that models the ConstructibleHandle, CloneableHandle, DestroyableHandle concepts
  //! \tparam TRAITS - [optional] Type providing handle traits
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename NATIVE, typename ALLOCATOR = handle_alloc<NATIVE>, typename TRAITS = handle_traits<NATIVE>>
  struct Handle
  {
    concept_check(NATIVE,StoreableHandle);
    //concept_check(ALLOCATOR,CloneableHandle<NATIVE>);
    //concept_check(ALLOCATOR,DestroyableHandle<NATIVE>);
    
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \typedef type - Define own type
    using type = Handle<NATIVE,ALLOCATOR,TRAITS>;

    //! \typedef native_t - Defines handle type
    using native_t = NATIVE;

    //! \typedef traits_t - Define handle traits type
    using traits_t = TRAITS;
    
    //! \var npos - Invalid handle sentinel value
    static constexpr native_t npos = traits_t::npos;
    
  protected:    
    //! \typedef alloc_t - Define handle allocator type
    using alloc_t = ALLOCATOR;

    //! \typedef converter_t - Define handle-pointer converter type
    using converter_t = converter<native_t>;
    
    //! \typedef value_t - Define allocation handle type
    using value_t = NativeHandle<native_t>;

    //! \typedef storage_t - Define smart-pointer type used to manage ownership and release
    using storage_t = std::shared_ptr<native_t>;

    // ----------------------------------- REPRESENTATION ------------------------------------
  protected:
    value_t    Value;      //!< Handle value and method of allocation
    storage_t  Storage;    //!< Shared pointer used to manage ownership

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Handle::Handle
    //! Create an empty handle (A weak reference to the appropriate 'Invalid Handle' sentinel value)
    /////////////////////////////////////////////////////////////////////////////////////////
    Handle() : Handle(npos, AllocType::WeakRef)
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // Handle::Handle
    //! Create shared handle using appropriate allocator
    //!
    //! \param[in] && arg - First argument to allocator 'create' function
    //! \param[in] &&... args - [optional] Remaining arguments
    //!
    //! \throw wtl::platform_error - Unable to create handle
    //!
    //! \remarks This constructor syntax enforces an invariant upon allocators - they cannot provide a parameterless factory method
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename ARG, typename... ARGS> explicit 
    Handle(ARG&& arg, ARGS&&... args) : Value( allocate(std::forward<ARG>(arg), std::forward<ARGS>(args)...) ),
                                        Storage( converter_t::toPointer(Value.Handle), [this](native_t* ptr) { safeDelete(ptr); } )
    {
      // Ensure created successfully
      if (!exists())
        throw platform_error(HERE, "Unable to create handle");
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Handle::Handle
    //! Create from a pre-existing native handle
    //!
    //! \param[in] h - Native handle
    //! \param[in] t - Allocation type
    /////////////////////////////////////////////////////////////////////////////////////////
    Handle(native_t h, AllocType t) : Value(h, t),
                                      Storage(converter_t::toPointer(h), [this](native_t* ptr) { safeDelete(ptr); } )
    {}

	  // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(Handle);      //!< Can be deep copied
    ENABLE_MOVE(Handle);      //!< Can be moved
    ENABLE_POLY(Handle);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------
  private:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Handle::allocate
    //! Allocates the handle using appropriate allocator. Also ensures handle models 'ConstructibleHandle' concept.
    //!
    //! \param[in] &&... args - Arguments to allocator 'create' function
    //! \return value_t - Newly allocated handle
    //!
    //! \throw wtl::platform_error - Unable to create handle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    static value_t allocate(ARGS&&... args) 
    {
      static_assert(requires<alloc_t,concepts::ConstructibleHandle<native_t,ARGS...>>::value, "Incorrect handle constructor arguments");
      concept_check(alloc_t,ConstructibleHandle<native_t,ARGS...>);

      return alloc_t::create(std::forward<ARGS>(args)...);
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Handle::exists const
    //! Query whether handle is valid
    //!
    //! \return bool - True iff handle is valid
    /////////////////////////////////////////////////////////////////////////////////////////
    bool exists() const
    {
      return (bool)Storage != false && get() != npos;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Handle::get const
    //! Get the handle value
    //!
    //! \return native_t - Handle
    /////////////////////////////////////////////////////////////////////////////////////////
    native_t get() const
    {
      return converter_t::toHandle(Storage.get());
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Handle::operator bool const
    //! Query whether handle is valid
    //!
    //! \return bool - True iff handle is valid
    /////////////////////////////////////////////////////////////////////////////////////////
    operator bool() const
    {
      return exists();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Handle::operator native_t const
    //! Implicit user conversion to underlying handle
    //!
    //! \return native_t - Handle
    /////////////////////////////////////////////////////////////////////////////////////////
    operator native_t() const
    {
      return get();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Handle::operator == const
    //! Shared handle equality operator
    //!
    //! \param[in] const &r - Another handle
    //! \return bool - True iff handle & method are equal
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator == (const type& r) const
    {
      return Value.Handle == r.Value.Handle
          && Value.Method == r.Value.Method;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Handle::operator == const
    //! Native handle equality operator
    //!
    //! \param[in] h - Native handle
    //! \return bool - True iff handles are equal
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator == (const native_t h) const
    {
      return Value.Handle == h;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Handle::operator != const
    //! Shared handle inequality operator
    //!
    //! \param[in] const &r - Another handle
    //! \return bool - True iff handle or method are different
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator != (const type& r) const
    {
      return Value.Handle != r.Value.Handle
          || Value.Method != r.Value.Method;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Handle::operator != const
    //! Native handle inequality operator
    //!
    //! \param[in] h - Native handle
    //! \return bool - True iff handles are different
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator != (const native_t h) const
    {
      return Value.Handle != h;
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Handle::release
    //! Releases the handle
    //!
    //! \throw wtl::platform_error - Unable to release handle
    /////////////////////////////////////////////////////////////////////////////////////////
    void release()
    {
      bool success(true);
      auto unsafeDelete = [this,&success] (native_t*  ptr) { success = safeDelete(ptr); };

      // Use custom deleter to access result
      if (exists())
        Storage.reset(static_cast<native_t*>(nullptr), unsafeDelete);

      // [FAILED] Throw platform_error
      if (!success)
        throw platform_error(HERE, "Unable to release handle");
    }

   protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Handle::safeDelete
    //! Destroys a handle
    //!
    //! \param[in] ptr - Handle
    //! \return bool - Return true if handle was destroyed or already invalid, false if valid but failed to destroy
    /////////////////////////////////////////////////////////////////////////////////////////
    bool safeDelete(native_t* ptr)
    {
      // Ensure both valid pointer and handle
      if (ptr != nullptr && converter_t::toHandle(ptr) != npos)
        // Pass handle and method to factory destructor
        return alloc_t::destroy(Value);

      // Succeed if invalid
      return true;
    }
  };


  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator == 
  //! Non-member native handle equality operator
  //!
  //! \tparam NATIVE - Native handle type which models the StoreableHandle concept
  //! \tparam ALLOCATOR - [optional] Handle allocator type that models the ConstructibleHandle, CloneableHandle, DestroyableHandle concepts
  //! \tparam TRAITS - [optional] Type providing handle traits
  //!
  //! \param[in] nh - Native handle
  //! \param[in] const& sh - Shared handle
  //! \return bool - True iff handles are equal
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename NATIVE, typename ALLOCATOR, typename TRAITS>
  bool operator == (NATIVE nh, const Handle<NATIVE,ALLOCATOR,TRAITS>& sh) 
  {
    return sh == nh;    // Delegate to member operator
  }
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator != 
  //! Non-member native handle inequality operator
  //!
  //! \tparam NATIVE - Native handle type which models the StoreableHandle concept
  //! \tparam ALLOCATOR - [optional] Handle allocator type that models the ConstructibleHandle, CloneableHandle, DestroyableHandle concepts
  //! \tparam TRAITS - [optional] Type providing handle traits
  //!
  //! \param[in] nh - Native handle
  //! \param[in] const& sh - Shared handle
  //! \return bool - True iff handles are unequal
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename NATIVE, typename ALLOCATOR, typename TRAITS>
  bool operator != (NATIVE nh, const Handle<NATIVE,ALLOCATOR,TRAITS>& sh) 
  {
    return sh != nh;    // Delegate to member operator
  }


} // WTL namespace



#endif // WTL_HANDLE_HPP


