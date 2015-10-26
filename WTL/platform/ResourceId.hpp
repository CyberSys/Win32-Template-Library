//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\ResourceId.hpp
//! \brief Provides utility types encapsulating points, sizes, rectangles, etc.
//! \date 26 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RESOURCE_ID_HPP
#define WTL_RESOURCE_ID_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EncodingTraits.hpp"      //!< encoding_char_t
#include "wtl/utils/DebugInfo.hpp"            //!< DebugInfo
#include "wtl/platform/SystemFlags.hpp"       //!< ResourceType

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ResourceId - Resource identifier
  //! 
  //! \tparam ENC - String encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ResourceId final
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias char_t - Character type
    using char_t = encoding_char_t<ENC>;

    //! \union Ident - Identifier
    union Ident
    {
      const char_t*  Name;       //!< Resource name
      uint32_t       Numeral;    //!< Resource ID
    };
    
    // ----------------------------------- REPRESENTATION -----------------------------------

    Ident  Value;     //!< Resource identifier value

    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ResourceId::ResourceId
    //! Create empty
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    ResourceId() 
    {
      Value.Numeral = 0;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ResourceId::ResourceId
    //! Create from string representation
    //! 
    //! \param[in] const* name - Resource name
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    ResourceId(const char_t* name) 
    {
      Value.Name = name;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ResourceId::ResourceId
    //! Create from 16-bit ordinal representation
    //! 
    //! \param[in] id - 16-bit ordinal representation
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    ResourceId(uint16_t id) 
    {
      Value.Numeral = id;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ResourceId::ResourceId
    //! Encode resource type using ordinal representation
    //! 
    //! \param[in] type - Resource type
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    ResourceId(ResourceType type) 
    {
      Value.Numeral = enum_cast(type);
    }

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    CONSTEXPR_COPY_CTOR(ResourceId);      //!< Can be deep copied at compile-time
    CONSTEXPR_MOVE_CTOR(ResourceId);      //!< Can be moved at compile-time
    ENABLE_COPY_ASSIGN(ResourceId);       //!< Can be assigned
    ENABLE_MOVE_ASSIGN(ResourceId);       //!< Can be move-assigned
    DISABLE_POLY(ResourceId);             //!< Cannot be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    /////////////////////////////////////////////////////////////////////////////////////////
    // ResourceId::isOrdinal constexpr 
    //! Queries whether the Id is numeric 
    //! 
    //! \return bool - True iff ordinal representation
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr bool isOrdinal() const noexcept 
    {
      return IS_INTRESOURCE(Value.Numeral);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ResourceId::isString constexpr 
    //! Queries whether the Id is a string 
    //! 
    //! \return bool - True iff string representation
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr bool isString() const noexcept 
    {
      return !IS_INTRESOURCE(Value.Numeral);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ResourceId::toOrdinal constexpr 
    //! Get the Ordinal representation
    //! 
    //! \return uint16_t - Ordinal representation
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr uint16_t toOrdinal() const noexcept 
    {
      return static_cast<uint16_t>(Value.Numeral);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ResourceId::toString constexpr 
    //! Get the string representation
    //! 
    //! \return const char_t* - String representation
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr const char_t* toString() const noexcept 
    {
      return Value.Name;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ResourceId::operator const char_t* constexpr 
    //! Get string representation of resource identifier 
    //! 
    //! \return char_t* - String representation 
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr operator const char_t*() const noexcept
    {
      return Value.Name;
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };

  //! \alias ResourceIdA - ANSI Resource identifier
  using ResourceIdA = ResourceId<Encoding::ANSI>;

  //! \alias ResourceIdW - UTF16 Resource identifier
  using ResourceIdW = ResourceId<Encoding::UTF16>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct default_t<ResourceId> - Define default values for 'ResourceId' types
  /////////////////////////////////////////////////////////////////////////////////////////
  /*template <> struct default_t<ResourceIdA> : literal_constant<ResourceIdA> { CONSTEXPR_CTOR(default_t); };
  template <> struct default_t<ResourceIdW> : literal_constant<ResourceIdW> { CONSTEXPR_CTOR(default_t); };*/

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::resource_id
  //! Creates a numeric resource identifier
  //!
  //! \tparam ENC - Encoding type
  //! \tparam ENUM - Identifier type
  //! 
  //! \param[in] id - Identifier
  //! \return ResourceId - Resource Id of appropriate type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC = wtl::Encoding::UTF16, typename ENUM>
  ResourceId<ENC> resource_id(ENUM id)
  {
    return ResourceId<ENC>(enum_cast(id));
  }


  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::resource_name
  //! Creates a resource string identifier
  //!
  //! \tparam CHR - Character type
  //! 
  //! \param[in] const* name - Resource name
  //! \return ResourceId - Resource Id of appropriate type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename CHR>
  ResourceId<default_encoding<CHR>::value> resource_name(const CHR* name)
  {
    return ResourceId<default_encoding<CHR>::value>(name);
  }

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Prints a Size to the debug console
  //!
  //! \tparam T - Size field type
  //! 
  //! \param[in,out] &c - Debugging console
  //! \param[in] const& sz - Size
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  Console& operator << (Console& c, const ResourceId<ENC>& id)
  {
    if (id.isOrdinal())
      return c << id.toOrdinal();
    else
      return c << id.toString();
  };


} // WTL namespace



#endif // WTL_RESOURCE_ID_HPP


