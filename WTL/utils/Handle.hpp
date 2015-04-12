////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Handle.hpp
//! \brief Encapsulate any handle type
//! \date 8 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_HANDLE_HPP
#define WTL_HANDLE_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct handle_traits - Defines handle traits
  //!
  //! \tparam T - Any handle type
  ///////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct handle_traits
  {
    static constexpr bool cloneable = false;
  };

  ///////////////////////////////////////////////////////////////////////////////
  //! \enum AllocType - Handle allocation type
  ///////////////////////////////////////////////////////////////////////////////
  enum class AllocType
  {
    Accquire,   //!< Handle was accquired
    Create,     //!< Handle was created
    WeakRef,    //!< Handle is not owned
  };

  ///////////////////////////////////////////////////////////////////////////////
  //! \struct HAlloc - Associates handle and allocation method
  ///////////////////////////////////////////////////////////////////////////////
  template <typename handle_t>
  struct HAlloc
  {
    HAlloc(handle_t h, AllocType at) : Handle(h), Method(at)
    {}

    handle_t   Handle;      //!< Handle
    AllocType  Method;      //!< Allocation method
  };

  ///////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc - Provides allocation/release for any handle type
  //!
  //! \tparam T - Any handle type
  ///////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct handle_alloc
  {
    static HAlloc<T> create();
    static HAlloc<T> clone(HAlloc<T>);
    static void      destroy(HAlloc<T>);
  };


  ///////////////////////////////////////////////////////////////////////////////
  //! \struct Handle - Encapsulates any handle
  //!
  //! \tparam T - Any handle type that can be stored as a pointer
  ///////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct Handle
  {
    //static_assert(std::is_convertible<T,HANDLE>::value, "Only Win32 handles are supported");
    static_assert(sizeof(T) <= sizeof(HANDLE), "Unsupported handle size");

    // ---------------------- TYPES & CONSTANTS -------------------
    
    //! \typedef alloc_t - Define handle allocator type
    using alloc_t = handle_alloc<T>;

    //! \typedef handle_t - Defines handle type
    using handle_t = T;

    //! \typedef halloc_t - Define allocation handle type
    using halloc_t = HAlloc<T>;

    //! \typedef pointer_t - Defines internal representation used to store handle
    using pointer_t = handle_t*;

    //! \typedef shared_ptr_t - Define shared pointer type
    using shared_ptr_t = std::shared_ptr<handle_t>;
    
    //! \typedef traits_t - Define handle traits type
    using traits_t = handle_traits<handle_t>; 

    //! \var npos - 'No handle' sentinel value
    static const Handle<T> npos;

    // ---------------- CONSTRUCTION / DESTRUCTION ----------------

    ///////////////////////////////////////////////////////////////////////////////
    // Handle::Handle
    //! Create shared handle using appropriate allocator
    //! 
    //! \param[in] &&... args - [optional] Strongly typed variadic arguments
    //! 
    //! \throw wtl::platform_error - Unable to create handle
    ///////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    Handle(ARGS&&... args) : Allocation(alloc_t::create(std::forward<ARGS>(args)...)), 
                             Storage(toPointer(Allocation.Handle), [this](pointer_t ptr) { safeDelete(ptr); })
    {
      // Ensure created successfully
      if (!exists())
        throw platform_error(HERE, "Unable to create handle");
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Handle::Handle
    //! Create from a pre-existing native handle
    //! 
    //! \param[in] h - Native handle
    //! \param[in] t - Allocation type
    ///////////////////////////////////////////////////////////////////////////////
    Handle(handle_t h, AllocType t) : Allocation(h, t),
                                      Storage(toPointer(h), [this](pointer_t ptr) { safeDelete(ptr); } )
    {}
    
    ///////////////////////////////////////////////////////////////////////////////
    // Handle::Handle
    //! Copy-create from another shared handle
    //! 
    //! \param[in] const& r - Another shared handle
    ///////////////////////////////////////////////////////////////////////////////
    Handle(const Handle&) = default;
    
    ///////////////////////////////////////////////////////////////////////////////
    // Handle::Handle
    //! Move-create from another shared handle
    //! 
    //! \param[in] && r - Another shared handle
    ///////////////////////////////////////////////////////////////////////////////
    Handle(Handle&&) = default;
    
    ///////////////////////////////////////////////////////////////////////////////
    // Handle::operator=
    //! Copy-assign from another shared handle
    //! 
    //! \param[in] const& r - Another shared handle
    ///////////////////////////////////////////////////////////////////////////////
    Handle& operator=(const Handle&) = default;
    
    ///////////////////////////////////////////////////////////////////////////////
    // Handle::operator=
    //! Move-assign from another shared handle
    //! 
    //! \param[in] && r - Another shared handle
    ///////////////////////////////////////////////////////////////////////////////
    Handle& operator=(Handle&&) = default;

    // ------------------------- STATIC ---------------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // Handle::npos
    //! Get the 'Invalid handle' sentinel value
    //! 
    //! \return handle_t - 'Invalid handle' sentinel value
    ///////////////////////////////////////////////////////////////////////////////
    /*static handle_t npos()
    {
      return alloc_t::npos;
    }*/

  protected:
    ///////////////////////////////////////////////////////////////////////////////
    // Handle::toHandle
    //! Convert pointer to handle
    //! 
    //! \param[in] ptr - Pointer
    //! \return handle_t - 'ptr' as handle
    ///////////////////////////////////////////////////////////////////////////////
    static handle_t toHandle(pointer_t ptr)
    {
      return (handle_t)(long32)ptr;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Handle::toPointer
    //! Convert handle to pointer
    //! 
    //! \param[in] h - Handle
    //! \return pointer_t - 'h' as pointer
    ///////////////////////////////////////////////////////////////////////////////
    static pointer_t toPointer(handle_t h)
    {
      return (pointer_t)(long32)h;
    }

    // ------------------------ ACCESSORS -------------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // Handle::exists const
    //! Query whether handle is valid
    //! 
    //! \return bool - True iff handle is valid
    ///////////////////////////////////////////////////////////////////////////////
    bool exists() const
    {
      return (bool)Storage != false && get() != alloc_t::npos;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Handle::get const
    //! Get the handle value
    //! 
    //! \return handle_t - Handle
    ///////////////////////////////////////////////////////////////////////////////
    handle_t get() const
    {
      return toHandle(Storage.get());
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Handle::operator bool const
    //! Query whether handle is valid
    //! 
    //! \return bool - True iff handle is valid
    ///////////////////////////////////////////////////////////////////////////////
    operator bool() const
    {
      return exists();
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Handle::operator handle_t const
    //! Implicit user conversion to underlying handle
    //! 
    //! \return handle_t - Handle
    ///////////////////////////////////////////////////////////////////////////////
    operator handle_t() const
    {
      return get();
    }

    // ------------------------- MUTATORS -------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // Handle::release
    //! Releases the handle
    //! 
    //! \throw wtl::platform_error - Unable to release handle
    ///////////////////////////////////////////////////////////////////////////////
    void release() 
    {
      bool success(true);

      // Use custom deleter to access result
      if (exists())
        Storage.reset(nullptr, [this,&success](pointer_t  ptr) { success = safeDelete(ptr); });

      // [FAILED] Throw platform_error
      if (!success)
        throw platform_error(HERE, "Unable to release handle");
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Handle::safeDelete
    //! Destroys a handle
    //! 
    //! \param[in] ptr - Handle
    //! \return bool - Return true if handle was destroyed or already invalid, false if valid but failed to destroy
    ///////////////////////////////////////////////////////////////////////////////
    bool safeDelete(pointer_t ptr) 
    { 
      // Ensure both valid pointer and handle
      if (ptr != nullptr && toHandle(ptr) != alloc_t::npos)
        // Pass handle and method to factory destructor
        return alloc_t::destroy(Allocation);
      
      // Succeed if invalid
      return true;
    }

    // ---------------------- REPRESENTATION -----------------------
  protected:
    halloc_t      Allocation;  //!< Allocated handle & method storage
    shared_ptr_t  Storage;     //!< Handle
  };
  
} // GDK namespace



#endif // WTL_HANDLE_HPP

#pragma once
