//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\PopupMeun.hpp
//! \brief Provides window menus and popup menus
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_POPUP_MENU_HPP
#define WTL_POPUP_MENU_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/Handle.hpp"                             //!< Handle
#include "wtl/traits/EncodingTraits.hpp"                    //!< Encoding
#include "wtl/traits/MenuTraits.hpp"                        //!< HMenu
#include "wtl/gdi/StockObjects.hpp"                         //!< StockBrush
#include "wtl/windows/MenuItemInfo.hpp"                     //!< MenuItemInfo
#include "wtl/windows/Command.hpp"                          //!< Command
#include "wtl/windows/CommandGroup.hpp"                     //!< CommandGroup
#include "wtl/windows/events/OwnerDrawMenuEvent.hpp"        //!< OwnerDrawEvent
#include "wtl/windows/events/OwnerMeasureMenuEvent.hpp"     //!< OwnerMeasureEvent

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct PopupMenu - Provides a pop-up menu containing items representing the Commands of a GUI Command Group
  //! 
  //! \tparam ENC - Menu text character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct PopupMenu 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = PopupMenu<ENC>;

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias iteminfo_t - Define item info type
    using iteminfo_t = MenuItemInfo<ENC>;
    
    //! \var encoding - Define item text character encoding
    static constexpr Encoding  encoding = ENC;

  protected:
    //! \alias collection_t - Define collection type
    using collection_t = List<CommandPtr<ENC>>;

    //! \alias const_iterator - Define collection iterator type
    using const_iterator = typename collection_t::const_iterator;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  public:
    OwnerDrawMenuEvent<encoding>      OwnerDraw;        //!< 'Owner draw' event 
    OwnerMeasureMenuEvent<encoding>   OwnerMeasure;     //!< 'Owner Measure' event 
    CommandGroupPtr<encoding>         Group;            //!< Shared Command group

  protected:
    HMenu         Handle;     //!< Menu handle
    collection_t  Items;      //!< Menu command items
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // PopupMenu::PopupMenu
    //! Populate from an Command Group
    //!
    //! \param[in] const& group - Command group
    /////////////////////////////////////////////////////////////////////////////////////////
    PopupMenu(const CommandGroupPtr<encoding>& group) : Handle(MenuType::Popup), 
                                                        Group(group)
    {
      // Owner draw handler
      /*OwnerDraw += new OwnerDrawMenuEventHandler<encoding>(this, &PopupMenu::onOwnerDraw);
      OwnerMeasure += new OwnerMeasureMenuEventHandler<encoding>(this, &PopupMenu::onOwnerMeasure);*/

      // Insert items from group
      for (auto& cmd : *Group)
        *this += cmd.second;
    }

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // PopupMenu::begin const
    //! Get read-only position of first command
    //! 
    //! \return const_iterator - Position of first command
    /////////////////////////////////////////////////////////////////////////////////////////
    const_iterator  begin() const
    {
      return Items.begin();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // PopupMenu::end const
    //! Get read-only position of last command
    //! 
    //! \return const_iterator - Position of last command
    /////////////////////////////////////////////////////////////////////////////////////////
    const_iterator  end() const
    {
      return Items.end();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // PopupMenu::handle const
    //! Get the shared menu handle 
    //! 
    //! \return const HMenu& - Shared menu handle
    /////////////////////////////////////////////////////////////////////////////////////////
    const HMenu& handle() const
    {
      return Handle;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // PopupMenu::size const
    //! Get the number of items
    //! 
    //! \return int32_t - Number of items
    //! 
    //! \throw wtl::platform_error - Unable to query item count
    /////////////////////////////////////////////////////////////////////////////////////////
    int32_t  size() const
    {
      // Query item count
      int32_t num = GetMenuItemCount(Handle);
      if (num != -1)
        return num;

      // Error: failed
      throw platform_error(HERE, "Unable to query menu item count");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // PopupMenu::find const
    //! Searches for an Command 
    //! 
    //! \param[in] id - Command Id
    //! \return CommandPtr<encoding> - Shared command pointer, possibly empty
    /////////////////////////////////////////////////////////////////////////////////////////
    CommandPtr<encoding> find(CommandId id) const
    {
      // Lookup item
      auto pos = Items.find_if([id] (const CommandPtr<encoding>& command) { return command->ident() == id; });
      if (pos != end())
        return *pos;

      // [NOT FOUND] Return empty pointer
      return CommandPtr<encoding>(nullptr);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // PopupMenu::operator ::HMENU const
    //! Implicit user conversion to native menu handle
    //! 
    //! \return ::HMENU - Native handle
    /////////////////////////////////////////////////////////////////////////////////////////
    operator ::HMENU() const
    {
      return Handle;
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // PopupMenu::insert
    //! Inserts a Gui Command menu item at a position
    //! 
    //! \param[in] idx - Zero-based position 
    //! \param[in] const& cmd - Gui Command
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing command
    //! \throw wtl::platform_error - Unable to insert menu item
    /////////////////////////////////////////////////////////////////////////////////////////
    void  insert(int32_t idx, const CommandPtr<encoding>& cmd)
    {
      REQUIRED_PARAM(cmd);

      MenuItemInfo<encoding> item(*cmd);     //!< Generate Gui Command menu item

      // Insert item into menu
      if (!WinAPI<encoding>::insertMenuItem(Handle, idx, True, &item))
        throw platform_error(HERE, "Unable to insert menu item");

      // Insert Command into collection 
      Items.insert(Items.at(idx), cmd);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // PopupMenu::operator += 
    //! Appends a Gui Command menu item 
    //! 
    //! \param[in] const& cmd - Gui command
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing command
    //! \throw wtl::platform_error - Unable to insert menu item
    /////////////////////////////////////////////////////////////////////////////////////////
    PopupMenu&  operator += (const CommandPtr<encoding>& cmd)
    {
      REQUIRED_PARAM(cmd);

      // Append command to menu
      insert(size(), cmd);
      return *this;
    }

  protected:
    
  };

} // namespace wtl

#endif  // WTL_POPUP_MENU_HPP
