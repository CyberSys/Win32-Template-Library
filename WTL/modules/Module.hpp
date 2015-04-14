////////////////////////////////////////////////////////////////////////////////
//! \file wtl\modules\Module.hpp
//! \brief Encapsulates a module instance
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MODULE_HPP
#define WTL_MODULE_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/ModuleTraits.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \struct Module - Encapsulates a module (.dll, .exe)
  struct Module
  {
    // ------------------- TYPES & CONSTANTS -------------------
  
    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // Module::Module
    //! Create from native module handle
    //!
    //! \param[in] m - Native module handle
    ///////////////////////////////////////////////////////////////////////////////
    Module(::HMODULE m) : Handle(m, AllocType::WeakRef)
    {}
    
    ///////////////////////////////////////////////////////////////////////////////
    // Module::Module
    //! Create from shared module handle
    //!
    //! \param[in] m - Shared module handle
    ///////////////////////////////////////////////////////////////////////////////
    Module(const HModule& m) : Handle(m)
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // Module::Module
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~Module()
    {}
    
    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			
    
    // TODO: Execute functor

    ///////////////////////////////////////////////////////////////////////////////
    // Module::load
    //! Loads a string resource from the module
    //! 
    //! \tparam LEN - Output buffer capacity
    //! \tparam ENC - Output string character encoding (Also resource id encoding)
    //! 
    //! \param[in] id - String identifier
    //! \param[in,out] &str - Output buffer
    //! \return bool - True iff was found 
    //! 
    //! \throw wtl::domain_error - Insufficient buffer capacity to store string
    ///////////////////////////////////////////////////////////////////////////////
    template <unsigned LEN, Encoding ENC = Encoding::UTF16>
    bool load(ResourceId<ENC> id, CharArray<ENC,LEN>& str) 
    {
      return StringResource::load(Handle, id, str);
    }

    // ----------------------- MUTATORS ------------------------

    // -------------------- REPRESENTATION ---------------------
  protected:
    HModule   Handle;       //!< Module handle
  };
}

#endif // WTL_MODULE_HPP
