////////////////////////////////////////////////////////////////////////////////
//! \file wtl\resources\Resources.hpp
//! \brief Defines enumeration traits + helper global operators
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RESOURCES_HPP
#define WTL_RESOURCES_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct Resources - Provides access to application resources
  //! 
  //! \tparam ENC - Character encoding type
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC = Encoding::UTF16>
  struct Resources
  {      
    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias char_t - Defines encoding character type
    using char_t = encoding_char_t<ENC>;

    // --------------------- CONSTRUCTION ----------------------

    // ------------------------ STATIC -------------------------
  protected:
    ///////////////////////////////////////////////////////////////////////////////
    // Resources::load
    //! Find the resource data block associated with a resource handle
    //! 
    //! \tparam DATA - Resource storage format
    //! 
    //! \param[in] module - Module containing resource
    //! \param[in] res - Resource handle
    //! \return DATA* - Resource data block
    ///////////////////////////////////////////////////////////////////////////////
    template <typename DATA = uint8>
    static DATA* load(HMODULE module, const HResource& res)
    {
      // Load resource
      if (HGLOBAL block = ::LoadResource(module, res))
      {
        // Retrieve data
        if (DATA* data = reinterpret_cast<DATA*>(::LockResource(block)))
          return data;

        // Failed
        throw platform_error(HERE, "Unable to lock resource");
      }
      
      // Not found
      throw platform_error(HERE, "Missing resource");
    }

    // ---------------------- ACCESSORS ------------------------			

    // ----------------------- MUTATORS ------------------------

    // -------------------- REPRESENTATION ---------------------
  
  };

  
} //namespace wtl
#endif // WTL_RESOURCES_HPP

