//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Lazy.hpp
//! \brief Provides lazy initialization for any type
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_LAZY_HPP
#define WTL_LAZY_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/DynamicArray.hpp>

//! \namespace wtl - Windows template library
namespace wtl
{

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Lazy - Provides lazy initialization for any type
  //!
  //! \tparam DATA - Any type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename DATA>
  struct Lazy : protected DynamicArray<DATA,1>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias base - Base class alias
    using base = DynamicArray<DATA,1>;
    
    //! \alias object_t - Object type
    using object_t = DATA;
    
    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Lazy::Lazy
    //! Does nothing
    /////////////////////////////////////////////////////////////////////////////////////////
    Lazy()
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    ENABLE_COPY(Lazy);     //!< Move semantics inherited from enclosed type
    ENABLE_MOVE(Lazy);     //!< Move semantics inherited from enclosed type
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Lazy::~Lazy
    //! Ensures the object is destroyed
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~Lazy()
    {
      destroy();
    }

	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Lazy::exists const
    //! Query whether object exists
    //!
    //! \return bool - True iff object exists
    /////////////////////////////////////////////////////////////////////////////////////////
    bool exists() const
    {
      return !this->empty();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Lazy::operator * const
    //! Access the object
    //!
    //! \return const_reference - Immutable reference to object 
    //! 
    //! \throw wtl::logic_error - [Debug only] Object does not exist
    /////////////////////////////////////////////////////////////////////////////////////////
    typename base::const_reference operator*() const
    {
      LOGIC_INVARIANT(this->empty() == false);

      return this->Data[0];
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Lazy::operator -> const
    //! Access the object
    //!
    //! \return const_pointer - Immutable pointer to object 
    //!
    //! \throw wtl::logic_error - [Debug only] Object does not exist
    /////////////////////////////////////////////////////////////////////////////////////////
    typename base::const_pointer operator->() const
    {
      LOGIC_INVARIANT(this->empty() == false);

      return this->Data;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Lazy::operator bool const
    //! Query whether object exists
    //!
    //! \return bool - True iff object exists
    /////////////////////////////////////////////////////////////////////////////////////////
    operator bool () const
    {
      return exists();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Lazy::operator ! const
    //! Query whether object does not exist
    //!
    //! \return bool - True iff object is non-existant
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator! () const
    {
      return !exists();
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Lazy::create
    //! Creates the object
    //! 
    //! \tparam ...PARAMS - [optional] Constructor argument types
    //!
    //! \param[in,out] && args - [optional] Constructor arguments
    //!
    //! \throw wtl::logic_error - [Debug only] Object already exists
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... PARAMS>
    void create(PARAMS&&... args)
    {
      LOGIC_INVARIANT(this->empty() == true);

      // variadic construct in place
      this->emplace_back(std::forward<PARAMS>(args)...);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Lazy::destroy
    //! Destroys the object
    /////////////////////////////////////////////////////////////////////////////////////////
    void destroy()
    {
      this->clear();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Lazy::operator *
    //! Access the object
    //!
    //! \return reference - Mutable reference to object 
    //!
    //! \throw wtl::logic_error - [Debug only] Object does not exist
    /////////////////////////////////////////////////////////////////////////////////////////
    typename base::reference operator*()
    {
      LOGIC_INVARIANT(exists());

      return this->Data[0];
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Lazy::operator ->
    //! Access the object
    //!
    //! \throw wtl::logic_error - [Debug only] Object does not exist
    /////////////////////////////////////////////////////////////////////////////////////////
    typename base::pointer operator->()
    {
      LOGIC_INVARIANT(exists());

      return this->Data;
    }
  };
  

  

} // WTL namespace



#endif // WTL_LAZY_HPP


