//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\combobox\ComboBoxItemsCollection.hpp
//! \brief Abstracts the contents of an edit control as an array of items
//! \date 27 February 2016
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_COMBOBOX_ITEMS_COLLECTION_HPP
#define WTL_COMBOBOX_ITEMS_COLLECTION_HPP

#include <wtl/WTL.hpp>
#include <wtl/traits/EncodingTraits.hpp>     //!< Encoding
//#include <wtl/utils/String.hpp>              //!< String

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  //! Forward declaration
  template <Encoding ENC>
  struct ComboBox;

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ComboBoxItem - 
  //! 
  //! \tparam ENC - Character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ComboBoxItem 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = ComboBoxItem<ENC>;
    
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;

    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    //! \var encoding - Define character encoding
    static constexpr Encoding encoding = ENC;

    // ----------------------------------- REPRESENTATION -----------------------------------
  private:
    String<encoding>   Text;      //!< Item text
    HIcon              Icon;      //!< Item icon
    uint32_t           Height;    //!< Item height
    void*              Data;      //!< Custom data
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBoxItem::ComboBoxItem
    //! Create item with icon and text
    //!
    //! \param[in] const& txt - Item text
    //! \param[in] const& icon - Item icon
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding InputEncoding>
    ComboBoxItem(const String<InputEncoding>& txt, const HIcon& icon) : Text(txt), Icon(icon)
    {}

    DISABLE_COPY(ComboBoxItem);     //!< Prevent removal from collection
    DISABLE_MOVE(ComboBoxItem);     //!< Prevent removal from collection
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ComboBoxItemsCollection - Abstracts the contents of an edit control as an array of items
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ComboBoxItemsCollection 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = ComboBoxItemsCollection<ENC>;

    //! \alias char_t - Define window character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias window_t - Define owner window type
    using window_t = ComboBox<ENC>;

    //! \alias value_t - Define item type
    using value_t = ComboBoxItem<ENC>;
    
    //! \alias reference_t - Define mutable reference type
    using reference_t = value_t&;
    
    //! \alias reference_t - Define immutable reference type
    using const_reference_t = const reference_t;
    
    //! \var encoding - Inherit character encoding
    static constexpr Encoding  encoding = base::encoding;
    
  private:
    //! \alias storage_t - Define item storage type
    using storage_t = std::vector<value_t>;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  private:
    window_t&  Control;     //!< Associated combobox window
    storage_t  Items;       //!< Item storage

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBoxItemsCollection::ComboBoxItemsCollection
    //! Create empty collection and associate with combobox control
    //! 
    //! \param[in,out] &ctrl - ComboBox control
    /////////////////////////////////////////////////////////////////////////////////////////
    ComboBoxItemsCollection(window_t& ctrl) : Control(ctrl)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBoxItemsCollection::at const
    //! Retrieve an element with bounds checking
    //! 
    //! \param[in] idx - Zero-based line index
    //! \return const_reference_t - Immutable reference to desired item
    //! 
    //! \throw wtl::logic_error - Control does not exist
    //! \throw wtl::out_of_range - Index out of range
    /////////////////////////////////////////////////////////////////////////////////////////
    const_reference_t at(uint32_t idx) const
    {
      //! Ensure exists
      if (!this->Control.exists())
        throw logic_error(HERE, "ComboBox control does not exist");

      // Verify index
      if (idx >= size())
        throw out_of_range(HERE, "Index ", idx, " is out of range");

      // Lookup item
      return this->Items[idx];
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBoxItemsCollection::append
    //! Append an item to the collection
    //! 
    //! \param[in] idx - Zero-based line index, or -1 for current line
    //! \return value_t - Specified line text 
    //! 
    //! \throw wtl::logic_error - Edit control does not exist
    //! \throw wtl::out_of_range - Index out of range
    //! \throw wtl::platform_error - Unable to retrieve window text
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    void append(uint32_t idx, const T* obj)
    {
      switch (ComboBox_InsertString(this->Control.handle(), idx, obj))
      {
      }
      int i  = CB_ERR, CB_ERRSPACE;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBoxItemsCollection::bottom const
    //! Retrieve the last line of text 
    //! 
    //! \return proxy_t - Proxy providing line text
    //! 
    //! \throw wtl::logic_error - Edit control does not exist
    //! \throw wtl::platform_error - Unable to retrieve window text
    /////////////////////////////////////////////////////////////////////////////////////////
    proxy_t bottom() const
    {
      // Retrieve bottom line
      return this->operator[](size()-1);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBoxItemsCollection::size() const
    //! Query whether collection is empty
    //! 
    //! \return bool - True iff empty
    //! 
    //! \throw wtl::logic_error - Control does not exist
    /////////////////////////////////////////////////////////////////////////////////////////
    bool empty() const 
    {
      return size() == 0;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBoxItemsCollection::size() const
    //! Query number of items
    //! 
    //! \return uint32_t - Number of items, if any
    //! 
    //! \throw wtl::logic_error - Control does not exist
    /////////////////////////////////////////////////////////////////////////////////////////
    uint32_t size() const 
    {
      //! Ensure exists
      if (!this->Control.exists())
        throw logic_error(HERE, "ComboBox does not exist");

      return this->Items.size();
      //return ComboBox_GetCount(this->Control.handle());
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBoxItemsCollection::operator[] const
    //! Retrieve an item without bounds checking
    //! 
    //! \param[in] idx - Zero-based item index
    //! \return proxy_t - Proxy providing line text
    //! 
    //! \throw wtl::logic_error - Edit control does not exist
    //! \throw wtl::platform_error - Unable to retrieve window text
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t operator[](uint32_t idx) const
    {
      //! Ensure exists
      if (!this->Control.exists())
        throw logic_error(HERE, "Edit control does not exist");

      // Return item
      return this->Items[idx];
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBoxItemsCollection::at 
    //! Retrieve an element with bounds checking
    //! 
    //! \param[in] idx - Zero-based line index
    //! \return reference_t - Mutable reference to desired item
    //! 
    //! \throw wtl::logic_error - Control does not exist
    //! \throw wtl::out_of_range - Index out of range
    /////////////////////////////////////////////////////////////////////////////////////////
    reference_t at(uint32_t idx) 
    {
      // Delegate to const implementation
      return const_cast<reference_t>( static_cast<const type*>(this)->at(idx) );
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBoxItemsCollection::clear() 
    //! Clear all items
    /////////////////////////////////////////////////////////////////////////////////////////
    void clear() const noexcept
    {
      //! Clear all items
      ComboBox_ResetContent(this->Control.handle());
    }
  };

      
} // namespace wtl

#endif // WTL_COMBOBOX_ITEMS_COLLECTION_HPP
