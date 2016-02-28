//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\combobox\ComboBoxItemsCollection.hpp
//! \brief Abstracts the contents of an ComboBox control as an array of items
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
  //! \struct ComboBoxItemsCollection - Abstracts the text of an ComboBox control as an array of items
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
    
    //! \alias owner_t - Define owner control type
    using owner_t = ComboBox<ENC>;

    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct ItemProxy - Proxy for individual items
    /////////////////////////////////////////////////////////////////////////////////////////
    struct ItemProxy
    {
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
      //! \alias type - Define own type
      using type = ItemProxy;

      // ----------------------------------- REPRESENTATION -----------------------------------
    private:
      const owner_t&  Control;       //!< ComboBox control
      const uint32_t  ItemIndex;     //!< Zero-based item index
      
      // ---------------------------------- CREATE & DESTROY ----------------------------------
    public:
      /////////////////////////////////////////////////////////////////////////////////////////
      // ItemProxy::ItemProxy
      //! Create proxy for a particular item
      //! 
      //! \param[in] const& ctrl - ComboBox control
      //! \param[in] idx - Zero-based item index
      /////////////////////////////////////////////////////////////////////////////////////////
      ItemProxy(const owner_t& ctrl, uint32_t idx) : Control(ctrl), 
                                                     ItemIndex(idx)
      {}

      // ------------------------------------ COPY & MOVE -------------------------------------
    
      // ----------------------------------- STATIC METHODS -----------------------------------
    
      // ---------------------------------- ACCESSOR METHODS ----------------------------------
    public:
      /////////////////////////////////////////////////////////////////////////////////////////
      // ItemProxy::get() const &&
      //! Get item data
      //! 
      //! \return T* - Item data
      //! 
      //! \throw wtl::platform_error - Unable to retrieve item data
      /////////////////////////////////////////////////////////////////////////////////////////
      template <typename T>
      T* get() const &&
      {
        // Query item data
        switch (uintptr_t data = ComboBox_GetItemData(this->Control.handle(), index()))
        {
        case CB_ERR: throw platform_error(HERE, "Unable to query ComboBox item data");
        default:     return reinterpret_cast<T*>(data);
        }
      }
      
      /////////////////////////////////////////////////////////////////////////////////////////
      // ItemProxy::height() const
      //! Query item height
      //! 
      //! \return uint32_t - Height of item, in pixels
      //! 
      //! \throw wtl::platform_error - Unable to set item height 
      /////////////////////////////////////////////////////////////////////////////////////////
      uint32_t height() const &&
      {
        // Query item height
        switch (int32_t h = this->Control.send(ComboBoxMessage::GetItemHeight, index()))
        {
        case CB_ERR: throw platform_error(HERE, "Unable to query ComboBox item height");
        default:     return h;
        }
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      // ItemProxy::index() const
      //! Query item index
      //! 
      //! \return uint32_t - Zero-based item index
      /////////////////////////////////////////////////////////////////////////////////////////
      uint32_t index() const && noexcept  
      {
        return this->ItemIndex;
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      // ItemProxy::const char_t*() const &&
      //! Get item data as text
      //! 
      //! \return const char_t* - Item text
      //! 
      //! \throw wtl::platform_error - Unable to retrieve item data
      /////////////////////////////////////////////////////////////////////////////////////////
      operator const char_t*() const &&
      {
        return get<const char_t*>();
      }
      
      /////////////////////////////////////////////////////////////////////////////////////////
      // ItemProxy::operator->() const &&
      //! Get item data as text
      //! 
      //! \return const char_t* - Item text
      //! 
      //! \throw wtl::platform_error - Unable to retrieve item data
      /////////////////////////////////////////////////////////////////////////////////////////
      const char_t* operator->() const &&
      {
        return get<const char_t*>();
      }
      
      // ----------------------------------- MUTATOR METHODS ----------------------------------
      
      /////////////////////////////////////////////////////////////////////////////////////////
      // ItemProxy::height() 
      //! Adjust item height
      //! 
      //! \param[in] h - Height of item, in pixels
      //! 
      //! \throw wtl::platform_error - Unable to set item height
      /////////////////////////////////////////////////////////////////////////////////////////
      void height(uint32_t h) &&
      {
        if (this->Control.send(ComboBoxMessage::GetItemHeight, index(), h) == CB_ERR)
          throw platform_error(HERE, "Unable to set height of item ", index(), " to ", h);
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      // ItemProxy::operator=
      //! Set new item data
      //! 
      //! \param[in] const* data - Item data
      //! 
      //! \throw wtl::platform_error - Unable to set item data
      /////////////////////////////////////////////////////////////////////////////////////////
      template <typename ItemData>
      type& operator=(const ItemData* data) &&
      {
        if (ComboBox_SetItemData(this->Control.handle(), index(), data) == CB_ERR)
          throw platform_error(HERE, "Unable to set data for item ", index());
      }

      //! Prevent assignment: Cannot assign one proxy to another
      type& operator=(const type&) = delete;
    };

    //! \alias proxy_t - Define item proxy type
    using proxy_t = ItemProxy;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  private:
    owner_t&  Control;     //!< Associated edit window

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBoxItemsCollection::ComboBoxItemsCollection
    //! Create collection for ComboBox control
    //! 
    //! \param[in,out] &ctrl - ComboBox control
    /////////////////////////////////////////////////////////////////////////////////////////
    ComboBoxItemsCollection(owner_t& ctrl) : Control(ctrl)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBoxItemsCollection::at const
    //! Retrieve an item with bounds checking
    //! 
    //! \param[in] idx - Zero-based item index, or -1 for currently selected item
    //! \return proxy_t - Proxy for specified item
    //! 
    //! \throw wtl::logic_error - ComboBox control does not exist
    //! \throw wtl::out_of_range - Index out of range
    /////////////////////////////////////////////////////////////////////////////////////////
    proxy_t at(int32_t idx) const
    {
      // Verify index
      if (idx < -1 || idx >= static_cast<int32_t>(size()))
        throw out_of_range(HERE, "Index ", idx, " is out of range");

      // Create proxy for specified item
      return operator[](idx);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBoxItemsCollection::size() const
    //! Query whether collection is empty
    //! 
    //! \return bool - True iff empty
    //! 
    //! \throw wtl::logic_error - ComboBox control does not exist
    /////////////////////////////////////////////////////////////////////////////////////////
    bool empty() const 
    {
      return size() == 0;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBoxItemsCollection::size() const
    //! Query number of items
    //! 
    //! \return uint32_t - Number of items  
    //! 
    //! \throw wtl::logic_error - ComboBox control does not exist
    /////////////////////////////////////////////////////////////////////////////////////////
    uint32_t size() const 
    {
      //! Ensure exists
      if (!this->Control.exists())
        throw logic_error(HERE, "ComboBox control does not exist");

      return ComboBox_GetCount(this->Control.handle());
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBoxItemsCollection::operator[] const
    //! Retrieve an item without bounds checking
    //! 
    //! \param[in] idx - Zero-based item index, or -1 for currently selected item
    //! \return proxy_t - Proxy for specified item 
    //! 
    //! \throw wtl::logic_error - ComboBox control does not exist
    /////////////////////////////////////////////////////////////////////////////////////////
    proxy_t operator[](int32_t idx) const
    {
      //! Ensure exists
      if (!this->Control.exists())
        throw logic_error(HERE, "ComboBox control does not exist");

      // Create proxy for specified item, otherwise locate currently selected item
      return proxy_t(this->Control, idx >= 0 ? idx : ComboBox_GetCurSel(this->Control.handle()));
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBoxItemsCollection::append() 
    //! Appends an item to the collection
    //! 
    //! \tparam ItemData - Type of item data
    //!
    //! \param[in] const* data - Item text or data
    //!
    //! \throw wtl::logic_error - ComboBox control does not exist
    //! \throw wtl::platform_error - Unable to append item
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename ItemData>
    void append(const ItemData* data) 
    {
      insert(data, -1);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBoxItemsCollection::clear() 
    //! Clear all items
    //!
    //! \throw wtl::logic_error - ComboBox control does not exist
    /////////////////////////////////////////////////////////////////////////////////////////
    void clear() const 
    {
      //! Ensure exists
      if (!this->Control.exists())
        throw logic_error(HERE, "ComboBox control does not exist");

      // Clear items
      ComboBox_ResetContent(this->Control.handle());
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBoxItemsCollection::insert() 
    //! Inserts an item to the collection at a position
    //! 
    //! \tparam ItemData - Type of item data
    //!
    //! \param[in] const* data - Item text or data
    //! \param[in] idx - Zero-based item index at which to insert new item
    //!
    //! \throw wtl::logic_error - ComboBox control does not exist
    //! \throw wtl::platform_error - Unable to append item
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename ItemData>
    void insert(const ItemData* data, uint32_t idx) 
    {
      //! Ensure exists
      if (!this->Control.exists())
        throw logic_error(HERE, "ComboBox control does not exist");

      // Append item
      switch (ComboBox_InsertItemData(this->Control.handle(), idx, data))
      {
      case CB_ERR:      throw platform_error(HERE, "Unable to add ComboBox item");
      case CB_ERRSPACE: throw platform_error(HERE, "Insufficient space to add ComboBox item");
      }
    }

  };

      
} // namespace wtl

#endif // WTL_COMBOBOX_ITEMS_COLLECTION_HPP
