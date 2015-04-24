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
    
    // -------------------- REPRESENTATION ---------------------
  protected:
    HModule     Module;     //!< Module containing resource
    HGlobal     Block;      //!< Resource block handle
    HResource   Handle;     //!< Resource data handle
  
  private:
    const void* Data;       //!< Resource data

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

    DEFAULT_COPY(Resource);
    //DEFAULT_COPY_ASSIGN(Resource);
    
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
    
    ///////////////////////////////////////////////////////////////////////////////
    // Resource::operator == const
    //! Equality operator 
    //! 
    //! \param[in] const &r - Another handle
    //! \return bool - True iff handles are to the same resource
    ///////////////////////////////////////////////////////////////////////////////
    bool operator == (const Resource& r) const
    {
      return this->Handle == r.Handle
          && this->Module == r.Module;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Resource::operator != const
    //! Inequality operator 
    //! 
    //! \param[in] const &r - Another resource
    //! \return bool - False iff handles are to the same resource
    ///////////////////////////////////////////////////////////////////////////////
    bool operator != (const Resource& r) const
    {
      return !operator==(r);
    }

    // ----------------------- MUTATORS ------------------------
  };



  
} //namespace wtl
#endif // WTL_RESOURCES_HPP

