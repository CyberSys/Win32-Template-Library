////////////////////////////////////////////////////////////////////////////////
//! \file wtl\resources\StringResource.hpp
//! \brief Encapsulates reading string resources
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_STRING_RESOURCES_HPP
#define WTL_STRING_RESOURCES_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct StringResource - Provides access to application resources
  //! 
  //! \tparam ENC - Character encoding type
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC = Encoding::UTF16>
  struct StringResource : Resources<ENC>
  {      
    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias base - Define base class
    using base = Resources<ENC>;

    //! \alias string_t - String buffer type
    using string_t = const typename base::char_t* const;

    ///////////////////////////////////////////////////////////////////////////////
    //! \struct StringTableEntry - Variable length string table entry
    ///////////////////////////////////////////////////////////////////////////////
    struct StringTableEntry
    {
      // Advance beyond this variable length entry to the next
      StringTableEntry* next(StringTableEntry* entry)
      {
        return reinterpret_cast<StringTableEntry*>(Text + Length);
      }

      uint16   Length;      //!< Length of current entry, in characters
      string_t Text;        //!< Text 
    };

    // --------------------- CONSTRUCTION ----------------------

    // ------------------------ STATIC -------------------------

    ///////////////////////////////////////////////////////////////////////////////
    // StringResource::load
    //! Loads a string resource
    //! 
    //! \tparam IdEncoding - Id encoding 
    //! \tparam OutEncoding- Output encoding
    //! \tparam OutLength - Output buffer length
    //! 
    //! \param[in] module - Module containing string
    //! \param[in] id - String identifier
    //! \param[in,out] &str - Output buffer
    //! \return bool - True iff was found 
    //! 
    //! \throw wtl::domain_error - Insufficient buffer capacity
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding IdEncoding, Encoding OutEncoding, unsigned OutLength>
    static bool load(HMODULE module, ResourceId<IdEncoding> id, CharArray<OutEncoding,OutLength>& str) 
    {
      decltype(id)  table(id.Value.Numeral / 16 + 1);  //!< String table resource
      int32         index = id.Value.Numeral % 16;     //!< Index of desired string within table
      
      // Lookup string table handle
      HResource resource(module, table, ResourceType::String, LanguageId());
      int32 size = ::SizeofResource(module, resource);

      // Load string table
      StringTableEntry* item = load<StringTableEntry>(module, resource);

      // Find desired string
      for (int32 idx = 0; item && idx < index; idx++)
        item = item->next();

      // Ensure sufficient space is available
      if (item && item->Length > OutLength)
        throw domain_error(HERE, "String resource %d requires %d chars but only %d available", id.Value.Numeral, (int32)item->Length, (int32)OutLength);

      // Extract + convert string if found, otherwise return false
      return item ? (str = CharArray<Encoding::UTF16,OutLength>(item->Text), true) : false;
    }
    
    // ---------------------- ACCESSORS ------------------------			

    // ----------------------- MUTATORS ------------------------

    // -------------------- REPRESENTATION ---------------------

  };

  
} //namespace wtl
#endif // WTL_STRING_RESOURCES_HPP

