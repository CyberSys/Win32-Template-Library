//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\resources\StringResource.hpp
//! \brief Encapsulates reading string resources
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_STRING_RESOURCES_HPP
#define WTL_STRING_RESOURCES_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct StringResource - Encapsulates loading a string from the resource table
  //! 
  //! \tparam ENC - Character encoding type
  //! \tparam LEN - Buffer capacity
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, unsigned LEN>
  struct StringResource : Resource
  {      
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias base - Define base type
    using base = Resource;

    //! \alias string_t - Define string type
    using string_t = CharArray<ENC,LEN>;
    
    //! \var encoding - Define character encoding
    static constexpr Encoding encoding = ENC;
    
  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct StringTableEntry - Variable length string table entry
    /////////////////////////////////////////////////////////////////////////////////////////
    struct StringTableEntry
    {
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------

      // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
    
      StringTableEntry() = delete;

      // ---------------------------------- ACCESSOR METHODS ----------------------------------			

      /////////////////////////////////////////////////////////////////////////////////////////
      // StringTableEntry::next
      //! Advance beyond this variable length entry to the next
      //! 
      //! \return const StringTableEntry* - Immutable pointer to next string in table
      /////////////////////////////////////////////////////////////////////////////////////////
      const StringTableEntry* next() const
      {
        return reinterpret_cast<const StringTableEntry*>(Text + Length);
      }
      
      // ----------------------------------- MUTATOR METHODS ----------------------------------

      // ----------------------------------- REPRESENTATION -----------------------------------

      //uint16         Dummy;        //!< Length of current entry, in characters
      uint16         Length;        //!< Length of current entry, in characters
      const wchar_t  Text[0xFFFF];  //!< String Text in UTF16
    };

    // ----------------------------------- REPRESENTATION -----------------------------------
  public:
    string_t   Text;     //!< String resource text

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // StringResource::StringResource
    //! Loads a string resource
    //! 
    //! \param[in] id - String identifier
    //! \param[in] lang - String language
    //! 
    //! \throw wtl::logic_error - Missing string -or- Insufficient buffer capacity to store string
    //! \throw wtl::platform_error - Unable to load resource
    /////////////////////////////////////////////////////////////////////////////////////////
    explicit StringResource(ResourceId<ENC> id, LanguageId lang = LanguageId::Neutral) : base(LoadedModules.findString(id,lang))
    {
      const int32  index = id.Value.Numeral % 16;                   //!< Index of desired string within table
      
      if (!this->Handle)
        throw platform_error(HERE, "String resource %d does not exist", id.Value.Numeral);

      // Load string table
      const StringTableEntry* item = get<StringTableEntry>();

      // Find desired string
      for (int32 idx = 1; item && idx < index; idx++)
        item = item->next();

      // [NOT-FOUND] Return false & empty string 
      if (!item)
        throw logic_error(HERE, "String resource %d does not exist", id.Value.Numeral);

      // [FOUND] Ensure sufficient space is available
      if (item->Length > LEN)
        throw logic_error(HERE, "String resource %d requires %d chars but only %d available", id.Value.Numeral, (int32)item->Length, (int32)LEN);

      // Convert from UTF16 if necessary
      Text.assign<Encoding::UTF16>(item->Text, item->Text+item->Length);
    }

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };

  
} //namespace wtl
#endif // WTL_STRING_RESOURCES_HPP

