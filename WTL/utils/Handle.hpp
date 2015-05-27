//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Handle.hpp
//! \brief Encapsulate any handle type
//! \date 8 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_HANDLE_HPP
#define WTL_HANDLE_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/Exception.hpp"            //!< Exceptions
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
  //! \struct HAlloc - Associates handle and allocation method
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename native_t>
  struct HAlloc
  {
    HAlloc(native_t h, AllocType at) : Handle(h), Method(at)
    {}

    native_t   Handle;      //!< Handle
    AllocType  Method;      //!< Allocation method
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc - Provides allocation/release for any handle type
  //!
  //! \tparam T - Any handle type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct handle_alloc;  /* Undefined */
  //{
  //  static constexpr T  npos = ¬T;
  //
  //  handle_alloc() = delete;
  //
  //  static HAlloc<T>  create();
  //  static HAlloc<T>  clone(HAlloc<T>);
  //  static void       destroy(HAlloc<T>);
  //};


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
  //! \struct Handle - Encapsulates any handle
  //!
  //! \tparam T - Any handle type that can be stored as a pointer
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct Handle
  {
    //static_assert(std::is_convertible<T,HANDLE>::value, "Only Win32 handles are supported");
    static_assert(sizeof(T) <= sizeof(HANDLE), "Unsupported handle size");

    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \typedef type - Define own type
    using type = Handle<T>;

    //! \typedef alloc_t - Define handle allocator type
    using alloc_t = handle_alloc<T>;

    //! \typedef native_t - Defines handle type
    using native_t = T;

    //! \typedef halloc_t - Define allocation handle type
    using halloc_t = HAlloc<T>;

    //! \typedef pointer_t - Defines internal representation used to store handle
    using pointer_t = native_t*;

    //! \typedef shared_ptr_t - Define shared pointer type
    using shared_ptr_t = std::shared_ptr<native_t>;
    
    //! \typedef traits_t - Define handle traits type
    using traits_t = handle_traits<native_t>; 

    // ----------------------------------- REPRESENTATION ------------------------------------
  protected:
    halloc_t      Allocation;  //!< Allocated handle & method storage
    shared_ptr_t  Storage;     //!< Handle

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Handle::Handle
    //! Create weak reference to appropriate 'Invalid Handle' sentinel value
    /////////////////////////////////////////////////////////////////////////////////////////
    Handle() : Handle(alloc_t::npos, AllocType::WeakRef)
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // Handle::Handle
    //! Create shared handle using appropriate allocator
    //! 
    //! \param[in] && arg - First strongly typed variadic argument
    //! \param[in] &&... args - [optional] Remaining strongly typed variadic arguments
    //! 
    //! \throw wtl::platform_error - Unable to create handle
    //! 
    //! \remarks This constructor syntax enforces an invariant upon allocators; they cannot provide a parameterless factory method
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename ARG, typename... ARGS>
    Handle(ARG&& arg, ARGS&&... args) : Allocation( alloc_t::create(std::forward<ARG>(arg), std::forward<ARGS>(args)...) ), 
                                        Storage( toPointer(Allocation.Handle), [this](pointer_t ptr) { safeDelete(ptr); } )
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
    Handle(native_t h, AllocType t) : Allocation(h, t),
                                      Storage(toPointer(h), [this](pointer_t ptr) { safeDelete(ptr); } )
    {}
    
	  // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(Handle);      //!< Can be deep copied
    ENABLE_MOVE(Handle);      //!< Can be moved 
    ENABLE_POLY(Handle);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------
  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Handle::toHandle
    //! Convert pointer to handle
    //! 
    //! \param[in] ptr - Pointer
    //! \return native_t - 'ptr' as handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static native_t toHandle(pointer_t ptr)
    {
      return (native_t)(long32_t)ptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Handle::toPointer
    //! Convert handle to pointer
    //! 
    //! \param[in] h - Handle
    //! \return pointer_t - 'h' as pointer
    /////////////////////////////////////////////////////////////////////////////////////////
    static pointer_t toPointer(native_t h)
    {
      return (pointer_t)(long32_t)h;
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
      return (bool)Storage != false && get() != alloc_t::npos;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Handle::get const
    //! Get the handle value
    //! 
    //! \return native_t - Handle
    /////////////////////////////////////////////////////////////////////////////////////////
    native_t get() const
    {
      return toHandle(Storage.get());
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
    //! Equality operator 
    //! 
    //! \param[in] const &r - Another handle
    //! \return bool - True iff handle & method are equal
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator == (const Handle<T>& r) const
    {
      return Allocation.Handle == r.Allocation.Handle
          && Allocation.Method == r.Allocation.Method;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Handle::operator != const
    //! Inequality operator 
    //! 
    //! \param[in] const &r - Another handle
    //! \return bool - True iff handle or method are different
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator != (const Handle<T>& r) const
    {
      return !operator==(r);
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
      auto unsafeDelete = [this,&success] (pointer_t  ptr) { success = safeDelete(ptr); };

      // Use custom deleter to access result
      if (exists())
        Storage.reset<native_t,decltype(unsafeDelete)>(nullptr, unsafeDelete);

      // [FAILED] Throw platform_error
      if (!success)
        throw platform_error(HERE, "Unable to release handle");
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Handle::safeDelete
    //! Destroys a handle
    //! 
    //! \param[in] ptr - Handle
    //! \return bool - Return true if handle was destroyed or already invalid, false if valid but failed to destroy
    /////////////////////////////////////////////////////////////////////////////////////////
    bool safeDelete(pointer_t ptr) 
    { 
      // Ensure both valid pointer and handle
      if (ptr != nullptr && toHandle(ptr) != alloc_t::npos)
        // Pass handle and method to factory destructor
        return alloc_t::destroy(Allocation);
      
      // Succeed if invalid
      return true;
    }
  };



} // WTL namespace



#endif // WTL_HANDLE_HPP


