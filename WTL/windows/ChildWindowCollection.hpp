//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\ChildWindowCollection.hpp
//! \brief Collection encapsulating the creation, destruction, and lookup of child windows
//! \date 25 February 2016
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CHILD_WINDOW_COLLECTION_HPP
#define WTL_CHILD_WINDOW_COLLECTION_HPP

#include <wtl/WTL.hpp>
#include <wtl/traits/EncodingTraits.hpp>                          //!< Encoding
#include <wtl/traits/WindowTraits.hpp>                            //!< HWnd
#include <wtl/utils/Exception.hpp>                                //!< exception
#include <wtl/utils/Handle.hpp>                                   //!< Handle
#include <wtl/windows/WindowId.hpp>                               //!< WindowId

//! \namespace wtl - Windows template library
namespace wtl 
{
 
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ChildWindowCollection - Define child window collection type
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ChildWindowCollection 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = ChildWindowCollection;
    
    //! \alias ident_t - Define identifier type
    using ident_t = WindowId;

    //! \alias window_t - Define child window type
    using window_t = Window<ENC>;

  protected:
    //! \alias collection_t - Define collection type
    using collection_t = WindowIdCollection<ENC>;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    collection_t  Collection;   //!< Maps window Ids to window objects
    Window<ENC>&  Owner;        //!< Window containing the collection
      
    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ChildWindowCollection::ChildWindowCollection
    //! Create empty collection
    //! 
    //! \param[in] &parent - Window containing the collection
    /////////////////////////////////////////////////////////////////////////////////////////
    ChildWindowCollection(Window<ENC>& owner) : Owner(owner)
    {}
      
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ChildWindowCollection::contains const
    //! Query whether collection contains a child window
    //! 
    //! \tparam IDENT - Window id type 
    //! 
    //! \param[in] id - Child window Id
    //! \return bool - True iff identifier is present
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename IDENT>
    bool contains(IDENT id) const
    {
      // Lookup child window
      return this->Collection.find(static_cast<ident_t>(id)) != this->Collection.end();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ChildWindowCollection::find const
    //! Find a child window by ID and cast to its dynamic type
    //! 
    //! \tparam CTRL - [optional] Child window type (Default is Window<encoding>)
    //! \tparam IDENT - Window id type 
    //! 
    //! \param[in] id - Child window Id
    //! \return CTRL& - Reference to child window
    //! 
    //! \throw wtl::domain_error - Unable to convert child window to specified type
    //! \throw wtl::logic_error - Child window not found
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename CTRL = window_t, typename IDENT>
    CTRL& find(IDENT id) const
    {
      // Lookup child window
      auto pos = this->Collection.find(static_cast<ident_t>(id));
      if (pos != this->Collection.end())
      {
        // [FOUND] Convert & return
        if (CTRL* ctrl = dynamic_cast<CTRL*>(pos->second))
          return *ctrl;

        // [ERROR] Incorrect window type
        throw domain_error(HERE, "Unable to convert child window to specified type");
      }

      // [ERROR] Unable to find child window
      throw logic_error(HERE, "Child window not found");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ChildWindowCollection::operator[] const
    //! Find a child window by ID
    //!
    //! \tparam IDENT - Window id type 
    //!
    //! \param[in] id - Child window id
    //! \return window_t* - Pointer to child window
    //! 
    //! \throw wtl::logic_error - Child window not found
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename IDENT>
    window_t* operator[](IDENT id) const
    {
      // Lookup child window
      auto pos = this->Collection.find(static_cast<ident_t>(id));
      if (pos != this->Collection.end())
        return pos->second;

      // [ERROR] Unable to find child window
      throw logic_error(HERE, "Child window not found");
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ChildWindowCollection::add
    //! Creates and inserts a child window into the collection
    //!
    //! \param[in,out] &child - Window object representing child window to be created
    //!
    //! \throw wtl::logic_error - Window already exists
    //! \throw wtl::platform_error - Unable to create window
    /////////////////////////////////////////////////////////////////////////////////////////
    void add(window_t& child)
    {
      // Ensure child window does not exist
      if (child.exists())
        throw logic_error(HERE, "Child window already exists");

      // Ensure identifier is unique
      else if (contains(child.Ident()))
        throw logic_error(HERE, "Identifier already in use");

      // Create as child of 'Owner' window
      child.Handle = HWnd(child.wndclass(), &child, Owner.handle(), child.Ident, child.Style, child.StyleEx, child.Text(), child.Position, child.Size);

      // Insert into collection iff successful
      this->Collection[child.Ident] = &child;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ChildWindowCollection::clear
    //! Clears and destroy all child windows from the collection
    //!
    //! \throw wtl::platform_error - Unable to destroy window
    /////////////////////////////////////////////////////////////////////////////////////////
    void clear()
    {
      // Lookup first/last
      typename collection_t::iterator pos = this->Collection.begin(), 
                                      last = this->Collection.end();

      // Remove and destroy each child window individually
      while (pos != last)
      {
        window_t* wnd = pos->second;

        // Remove from collection before destroying
        this->Collection.erase(pos++);
        wnd->destroy();
      }
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ChildWindowCollection::remove
    //! Removes and destroys a child window from the collection
    //!
    //! \param[in,out] &child - Child window object  (Handle must exist)
    //! 
    //! \throw wtl::logic_error - Child window not found
    /////////////////////////////////////////////////////////////////////////////////////////
    void remove(window_t& child)
    {
      // Remove from collection and then destroy
      if (this->Collection.erase(child.Ident) > 0)
        child.destroy();
      else
        // [ERROR] Unable to find child window
        throw logic_error(HERE, "Child window not found");
    }
  };

  
} // namespace wtl


#endif // WTL_CHILD_WINDOW_COLLECTION_HPP
