//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\resources\Resources.hpp
//! \brief Defines enumeration traits + helper global operators
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RESOURCES_HPP
#define WTL_RESOURCES_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/GlobalTraits.hpp"
#include "wtl/traits/ResourceTraits.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ResourceBlob - Provides access to resource data
  /////////////////////////////////////////////////////////////////////////////////////////
  struct ResourceBlob
  {      
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    HModule     Module;     //!< Module containing resource
    HGlobal     Block;      //!< ResourceBlob block handle
    HResource   Handle;     //!< ResourceBlob data handle
  
  private:
    const void* Data;       //!< ResourceBlob data

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ResourceBlob::ResourceBlob
    //! Create empty
    /////////////////////////////////////////////////////////////////////////////////////////
    ResourceBlob() : Module(default<HModule>()),
                 Handle(default<HResource>()),
                 Block(default<HGlobal>()),
                 Data(nullptr)
    {
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ResourceBlob::ResourceBlob
    //! Copy create
    /////////////////////////////////////////////////////////////////////////////////////////
    ResourceBlob(const ResourceBlob& r) : Module(r.Module),
                                  Handle(r.Handle),
                                  Block(r.Block),
                                  Data(r.Data)
    {
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ResourceBlob::ResourceBlob
    //! Create from resource handle
    //!
    //! \param[in] const& module - Module containing resource
    //! \param[in] const& resource - ResourceBlob handle
    //! 
    //! \throw platform_error - Unable to load resource
    /////////////////////////////////////////////////////////////////////////////////////////
    ResourceBlob(const HModule& module, const HResource& resource) : Module(module),
                                                                 Handle(resource),
                                                                 Block(module,resource),
                                                                 Data(::LockResource(Block))
    {
      if (!Data)
        throw platform_error(HERE, "Unable to lock resource");
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ResourceBlob::~ResourceBlob
    //! Virtual d-tor
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~ResourceBlob()
    {}

    //DEFAULT_COPY(ResourceBlob);
    //DEFAULT_COPY_ASSIGN(ResourceBlob);
    
    // ----------------------------------- STATIC METHODS -----------------------------------
  
    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ResourceBlob::exists const
    //! Query whether resource is valid
    //! 
    //! \return bool - True iff resource is valid
    /////////////////////////////////////////////////////////////////////////////////////////
    bool exists() const
    {
      return Handle.exists();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ResourceBlob::get const
    //! Access the resource data 
    //! 
    //! \tparam DATA - ResourceBlob storage format
    //! 
    //! \return const storage_t* - Immutable pointer to resource data 
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename DATA>
    const DATA* get() const
    {
      return reinterpret_cast<const DATA*>(Data);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ResourceBlob::size const
    //! Query the resource size
    //! 
    //! \return long32 - ResourceBlob size (Not sure of format)
    /////////////////////////////////////////////////////////////////////////////////////////
    long32 size() const
    {
      return ::SizeofResource(Module, Handle);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ResourceBlob::operator == const
    //! Equality operator 
    //! 
    //! \param[in] const &r - Another handle
    //! \return bool - True iff handles are to the same resource
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator == (const ResourceBlob& r) const
    {
      return this->Handle == r.Handle
          && this->Module == r.Module
          && this->Block == r.Block
          && this->Data == r.Data;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ResourceBlob::operator != const
    //! Inequality operator 
    //! 
    //! \param[in] const &r - Another resource
    //! \return bool - False iff handles are to the same resource
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator != (const ResourceBlob& r) const
    {
      return !operator==(r);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ResourceBlob::operator bool const
    //! Query whether resource is valid
    //! 
    //! \return bool - True iff resource is valid
    /////////////////////////////////////////////////////////////////////////////////////////
    operator bool () const
    {
      return exists();
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };



  
} //namespace wtl
#endif // WTL_RESOURCES_HPP

