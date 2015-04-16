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
  //! \struct Resource - Provides access to resource data
  ///////////////////////////////////////////////////////////////////////////////
  struct Resource
  {      
    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \var npos - Sentinel value for 'Resource not found'
    static const Resource npos;

    // --------------------- CONSTRUCTION ----------------------
  protected:
    ///////////////////////////////////////////////////////////////////////////////
    // Resource::Resource
    //! Create empty
    ///////////////////////////////////////////////////////////////////////////////
    Resource() : Module(HModule::npos),
                 Handle(HResource::npos),
                 Block(HGlobal::npos),
                 Data(nullptr)
    {
    }

  public:
    ///////////////////////////////////////////////////////////////////////////////
    // Resource::Resource
    //! Create from resource handle
    //!
    //! \param[in] const& module - Module containing resource
    //! \param[in] const& resource - Resource handle
    //! 
    //! \throw platform_error - Unable to load resource
    ///////////////////////////////////////////////////////////////////////////////
    Resource(const HModule& module, const HResource& resource) : Module(module),
                                                                 Handle(resource),
                                                                 Block(module,resource),
                                                                 Data(::LockResource(Block))
    {
      if (!Data)
        throw platform_error(HERE, "Unable to lock resource");
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Resource::~Resource
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~Resource()
    {}
    
    // ------------------------ STATIC -------------------------
  
    // ---------------------- ACCESSORS ------------------------			
    
    ///////////////////////////////////////////////////////////////////////////////
    // Resource::get const
    //! Access the resource data 
    //! 
    //! \tparam DATA - Resource storage format
    //! 
    //! \return const storage_t* - Immutable pointer to resource data 
    ///////////////////////////////////////////////////////////////////////////////
    template <typename DATA>
    const DATA* get() const
    {
      return reinterpret_cast<const DATA*>(Data);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Resource::size const
    //! Query the resource size
    //! 
    //! \return long32 - Resource size (Not sure of format)
    ///////////////////////////////////////////////////////////////////////////////
    long32 size() const
    {
      return ::SizeofResource(Module, Handle);
    }

    // ----------------------- MUTATORS ------------------------
    
    // -------------------- REPRESENTATION ---------------------
  protected:
    HModule     Module;     //!< Module containing resource
    HGlobal     Block;      //!< Resource block handle
    HResource   Handle;     //!< Resource data handle
  
  private:
    const void* Data;       //!< Resource data
  };


  ///////////////////////////////////////////////////////////////////////////////
  //! \struct Resources - Provides access to application resources
  //! 
  //! \tparam ENC - Character encoding type
  ///////////////////////////////////////////////////////////////////////////////
  //template <Encoding ENC = Encoding::UTF16>
  struct Resources
  {      
    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias char_t - Defines encoding character type
    //using char_t = encoding_char_t<ENC>;

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
    //! 
    //! \throw platform_error - Unable to load resource
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
      throw platform_error(HERE, "Cannot find resource");
    }

    // ---------------------- ACCESSORS ------------------------			

    // ----------------------- MUTATORS ------------------------

    // -------------------- REPRESENTATION ---------------------
  
  };

  
} //namespace wtl
#endif // WTL_RESOURCES_HPP

