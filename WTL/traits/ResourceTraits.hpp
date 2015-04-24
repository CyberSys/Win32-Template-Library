////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\ResourceTraits.hpp
//! \brief Defines enumeration traits + helper global operators
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RESOURCE_TRAITS_HPP
#define WTL_RESOURCE_TRAITS_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/ModuleTraits.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \enum ResourceType - Defines resource types
  enum class ResourceType : uint32
  {
    Cursor = 1,
    Bitmap = 2,
    Icon = 3,
    Menu = 4,
    Dialog = 5,
    String = 6,
    FontDir = 7,
    Font = 8,
    Accelerator = 9,
    RcData = 10,
    MessageTable = 11,
    GroupCursor  = 12,
    GroupIcon    = 14,
    Version = 16,
    DlgInclude = 17,
#if(winver >= 0x0400)
    PlugPlay = 19,
    Vxd = 20,
    AniCursor = 21,
    AniIcon = 22,
#endif 
    Html = 23,
    Manifest = 24,
  };

  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<ResourceType>  : std::false_type {};
  template <> struct is_contiguous<ResourceType> : std::false_type {};

  //! Define limits traits
  template <> struct max_value<ResourceType>     : std::integral_constant<ResourceType,ResourceType::Cursor>     {};
  template <> struct min_value<ResourceType>     : std::integral_constant<ResourceType,ResourceType::Manifest>   {};
  

  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct ResourceId - Resource identifier
  //! \tparam ENC - String encoding
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ResourceId final
  {
    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias char_t - Character type
    using char_t = encoding_char_t<ENC>;

    //! \union Ident - Identifier
    union Ident
    {
      const char_t* Name;       //!< Resource name
      uint32        Numeral;    //!< Resource ID
    };
    
    // -------------------- REPRESENTATION ---------------------

    Ident  Value;     //!< Resource identifier value

    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // ResourceId::ResourceId
    //! Create from string representation
    //! 
    //! \param[in] const* name - Resource name
    ///////////////////////////////////////////////////////////////////////////////
    ResourceId(const char_t* name) 
    {
      Value.Name = name;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // ResourceId::ResourceId
    //! Create from 16-bit ordinal representation
    //! 
    //! \param[in] id - 16-bit ordinal representation
    ///////////////////////////////////////////////////////////////////////////////
    ResourceId(uint16 id) 
    {
      Value.Numeral = id;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // ResourceId::ResourceId
    //! Encode resource type using ordinal representation
    //! 
    //! \param[in] type - Resource type
    ///////////////////////////////////////////////////////////////////////////////
    ResourceId(ResourceType type) 
    {
      Value.Numeral = enum_cast(type);
    }

    // ------------------------ STATIC -------------------------

    //! \var npos - 'No resource' sentinel value
    static const ResourceId npos;

    // ---------------------- ACCESSORS ------------------------			

    ///////////////////////////////////////////////////////////////////////////////
    // ResourceId::isOrdinal const
    //! Queries whether the Id is numeric 
    //! 
    //! \return bool - True iff ordinal representation
    ///////////////////////////////////////////////////////////////////////////////
    bool isOrdinal() const
    {
      return IS_INTRESOURCE(Value.Numeral);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // ResourceId::isString const
    //! Queries whether the Id is a string 
    //! 
    //! \return bool - True iff string representation
    ///////////////////////////////////////////////////////////////////////////////
    bool isString() const
    {
      return !IS_INTRESOURCE(Value.Numeral);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // ResourceId::toOrdinal const
    //! Get the Ordinal representation
    //! 
    //! \return uint16 - Ordinal representation
    ///////////////////////////////////////////////////////////////////////////////
    uint16 toOrdinal() const
    {
      return static_cast<uint16>(Value.Numeral);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // ResourceId::toString const
    //! Get the string representation
    //! 
    //! \return const char_t* - String representation
    ///////////////////////////////////////////////////////////////////////////////
    const char_t* toString() const
    {
      return Value.Name;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // ResourceId::operator char_t* const
    //! Get string representation of resource identifier 
    //! 
    //! \return char_t* - String representation 
    ///////////////////////////////////////////////////////////////////////////////
    operator const char_t*() const
    {
      return Value.Name;
    }

    // ----------------------- MUTATORS ------------------------

  };

  //! \alias ResourceIdA - ANSI Resource identifier
  using ResourceIdA = ResourceId<Encoding::ANSI>;

  //! \alias ResourceIdW - UTF16 Resource identifier
  using ResourceIdW = ResourceId<Encoding::UTF16>;
  
  //! \var npos - Define 'No resource' sentinel value
  template <Encoding ENC>
  const ResourceId<ENC>  ResourceId<ENC>::npos = ResourceId<ENC>(zero<uint16>::value);
  
  
  ///////////////////////////////////////////////////////////////////////////////
  //! wtl::resource_id
  //! Creates a numeric resource identifier
  //!
  //! \tparam ENC - Encoding type
  //! \tparam ENUM - Identifier type
  //! 
  //! \param[in] id - Identifier
  //! \return ResourceId - Resource Id of appropriate type
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC = wtl::Encoding::UTF16, typename ENUM>
  ResourceId<ENC> resource_id(ENUM id)
  {
    return ResourceId<ENC>(enum_cast(id));
  }


  ///////////////////////////////////////////////////////////////////////////////
  //! wtl::resource_name
  //! Creates a resource string identifier
  //!
  //! \tparam CHAR - Character type
  //! 
  //! \param[in] const* name - Resource name
  //! \return ResourceId - Resource Id of appropriate type
  ///////////////////////////////////////////////////////////////////////////////
  template <typename CHAR>
  ResourceId<default_encoding<CHAR>::value> resource_name(const CHAR* name)
  {
    return ResourceId<default_encoding<CHAR>::value>(name);
  }



  ///////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<::HRSRC> - Encapsulates allocating resource handles
  ///////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<::HRSRC>
  {
    //! \var npos - Invalid handle sentinel value
    static const ::HRSRC npos; 

    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HRSRC>::create
    //! Create resource handle
    //! 
    //! \tparam ENC - Resource name character encoding 
    //!
    //! \param[in] const& module - Module containing resource
    //! \param[in] name - Resource identifier
    //! \param[in] type - Resource type
    //! \param[in] lang - Resource language
    //! \return HAlloc<::HRSRC> - Accquired handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    static HAlloc<::HRSRC> create(const HModule& module, ResourceId<ENC> name, ResourceType type, LanguageId language) 
    { 
      // Load resource handle
      if (::HRSRC res = getFunc<ENC>(::FindResourceExA,::FindResourceExW)(module, ResourceId(type), name, language))
        return { res, AllocType::Accquire };

      // Error: Failed  
      throw platform_error(HERE, "Unable to find resource");
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HRSRC>::clone
    //! Clone handle
    //! 
    //! \param[in] addr - Handle
    //! \return HAlloc<::HRSRC> - Duplicate of handle
    //! 
    //! \throw wtl::platform_error - Failed to clone handle
    ///////////////////////////////////////////////////////////////////////////////
    static HAlloc<::HRSRC> clone(HAlloc<::HRSRC> addr);

    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HRSRC>::destroy noexcept
    //! Release handle 
    //! 
    //! \param[in] addr - Handle
    //! \return bool - Always true
    ///////////////////////////////////////////////////////////////////////////////
    static bool destroy(HAlloc<::HRSRC> addr) noexcept
    {
      // Resource handles are not released by module under Win32
      return true;
    }
  };
  
  //! \alias HResource - Resource handle
  using HResource = Handle<::HRSRC>;

} //namespace wtl
#endif // WTL_RESOURCE_TRAITS_HPP

