//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\WindowMenu.hpp
//! \brief Provides window menu
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_MENU_HPP
#define WTL_WINDOW_MENU_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/Handle.hpp"                             //!< Handle
#include "wtl/utils/DebugInfo.hpp"                          //!< object_info
#include "wtl/traits/EncodingTraits.hpp"                    //!< Encoding
#include "wtl/traits/MenuTraits.hpp"                        //!< HMenu
#include "wtl/platform/CommandId.hpp"                       //!< command_id,command_group_id
#include "wtl/platform/DrawingFlags.hpp"                    //!< DrawTextFlags
#include "wtl/gdi/StockObjects.hpp"                         //!< StockBrush
#include "wtl/windows/PopupMenu.hpp"                        //!< PopupMenu
#include "wtl/windows/Command.hpp"                          //!< Command
#include "wtl/windows/CommandGroup.hpp"                     //!< CommandGroup
#include "wtl/windows/events/OwnerDrawMenuEvent.hpp"        //!< OwnerDrawEvent
#include "wtl/windows/events/OwnerMeasureMenuEvent.hpp"     //!< OwnerMeasureEvent

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct WindowMenu - Encapsulates a window menu (a collection of popup menus)
  //! 
  //! \tparam ENC - Menu text character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct WindowMenu 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = WindowMenu<ENC>;

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;

    //! \alias popup_t - Define popup menu type
    using popup_t = PopupMenu<ENC>;

    //! \var encoding - Define item text character encoding
    static constexpr Encoding  encoding = ENC;

  protected:
    //! \alias collection_t - Define popup menu collection type
    using collection_t = List<popup_t>;

    //! \alias const_iterator - Immutable popup iterator
    using const_iterator = typename collection_t::const_iterator;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  public:
    OwnerDrawMenuEvent<encoding>      OwnerDraw;        //!< Raised by 'WM_DRAWITEM'
    OwnerMeasureMenuEvent<encoding>   OwnerMeasure;     //!< Raised by 'WM_MEASUREITEM'

  protected:
    HMenu         Handle;     //!< Menu handle
    collection_t  Popups;     //!< Popup menu collection
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowMenu::WindowMenu
    //! Create empty window menu 
    /////////////////////////////////////////////////////////////////////////////////////////
    WindowMenu() : Handle(MenuType::Window) 
    {
      // Owner draw handler
      OwnerDraw += new OwnerDrawMenuEventHandler<encoding>(this, &WindowMenu::onOwnerDraw);
      OwnerMeasure += new OwnerMeasureMenuEventHandler<encoding>(this, &WindowMenu::onOwnerMeasure);
    }

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowMenu::begin const
    //! Get read-only position of first popup menu
    //! 
    //! \return const_iterator - Position of first popup menu
    /////////////////////////////////////////////////////////////////////////////////////////
    const_iterator  begin() const
    {
      return Popups.begin();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowMenu::end const
    //! Get read-only position of last popup menu
    //! 
    //! \return const_iterator - Position of last popup menu
    /////////////////////////////////////////////////////////////////////////////////////////
    const_iterator  end() const
    {
      return Popups.end();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowMenu::empty const
    //! Query whether menu is empty (has no items)
    //! 
    //! \return bool - True iff empty
    /////////////////////////////////////////////////////////////////////////////////////////
    bool  empty() const
    {
      return size() == 0;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowMenu::find const
    //! Searches for an Command 
    //! 
    //! \param[in] id - Command id
    //! \return CommandPtr<encoding> - Shared command pointer, possibly empty
    /////////////////////////////////////////////////////////////////////////////////////////
    CommandPtr<encoding> find(CommandId id) const
    {
      // Search popups for a matching command
      for (auto& popup : Popups)
        if (auto cmd = popup.find(id))
          return cmd;
      
      // [NOT FOUND] Return empty pointer
      return CommandPtr<encoding>(nullptr);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowMenu::find const
    //! Searches for an Command group
    //! 
    //! \param[in] id - Group id
    //! \return CommandGroupPtr<encoding> - Shared group pointer, possibly empty
    /////////////////////////////////////////////////////////////////////////////////////////
    CommandGroupPtr<encoding> find(CommandGroupId id) const
    {
      // Search for matching popup
      auto pos = Popups.find_if([id] (const popup_t& popup) { return popup.Group->ident() == id; });
      if (pos != end())
        return pos->Group;

      // [NOT FOUND] Return empty pointer
      return CommandGroupPtr<encoding>(nullptr);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowMenu::handle const
    //! Get the shared menu handle 
    //! 
    //! \return const HMenu& - Shared menu handle
    /////////////////////////////////////////////////////////////////////////////////////////
    const HMenu& handle() const
    {
      return Handle;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowMenu::size const
    //! Get the number of pop-up menus
    //! 
    //! \return int32_t - Number of pop-up menus
    //!
    //! \throw wtl::platform_error - Unable to query menu item count
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
    // WindowMenu::operator ::HMENU
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
    // WindowMenu::insert
    //! Inserts a new popup menu item containing the Commands of an CommandGroup 
    //! 
    //! \param[in] idx - Zero-based position 
    //! \param[in] const& group - Shared command group
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing command group
    //! \throw wtl::platform_error - Unable to insert menu item
    /////////////////////////////////////////////////////////////////////////////////////////
    void insert(int32_t idx, const CommandGroupPtr<encoding>& group)
    {
      REQUIRED_PARAM(group);

      // Insert new Popup menu into collection and extract its handle
      auto popup = Popups.emplace(Popups.at(idx), group);

      MenuItemInfo<encoding> item(*group, popup->handle());     //!< Generate pop-up menu item 

      // Insert menu item 
      if (!WinAPI<encoding>::insertMenuItem(Handle, idx, True, &item))
        throw platform_error(HERE, "Unable to insert menu item");
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowMenu::operator += 
    //! Appends a new popup menu item containing the Commands of an CommandGroup 
    //! 
    //! \param[in] const& group - Shared command group
    //! \return type& - Reference to self
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing command group
    //! \throw wtl::platform_error - Unable to insert menu item
    /////////////////////////////////////////////////////////////////////////////////////////
    type&  operator += (const CommandGroupPtr<encoding>& group)
    {
      insert(size(), group);
      return *this;
    }

  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowMenu::onOwnerDraw
    //! Called in response to a reflected 'owner draw' message 
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual wtl::LResult  onOwnerDraw(OwnerDrawMenuEventArgs<encoding>& args) 
    { 
      // Draw background
      args.Graphics.fill(args.Rect, StockBrush::Blue);

      // [GROUP] Draw name
      if (auto group = find(command_group_id(args.Ident)))
        args.Graphics.write(group->name(), args.Rect, DrawTextFlags::Centre|DrawTextFlags::VCentre);

      // [COMMAND] Draw name
      else if (auto command = find(command_id(args.Ident)))
        args.Graphics.write(command->name(), args.Rect, DrawTextFlags::Centre|DrawTextFlags::VCentre);

      // Handled
      return 0;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowMenu::onOwnerMeasure
    //! Called in response to a reflected 'owner measure' message 
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual wtl::LResult  onOwnerMeasure(OwnerMeasureMenuEventArgs<encoding>& args) 
    { 
      // [HEADING] Lookup CommandGroup
      if (auto group = find(command_group_id(args.Ident)))
      {
        // Measure group name
        args.Size = args.Graphics.measure(group->name());
        
        // debug
        cdebug << object_info(__func__, "group", (int32_t)args.Ident, 
                                        "size", args.Size) << endl;
      }

      // [ITEM] Lookup Command
      else if (auto command = find(command_id(args.Ident)))
      {
        // Measure Command name
        args.Size = args.Graphics.measure(command->name());

        // debug
        cdebug << object_info(__func__, "command", (int32_t)args.Ident, 
                                        "size", args.Size, 
                                        "name", command->name()) << endl;
      }
        
      // Handled
      return 0;
    }

  };
      
} // namespace wtl

#endif  // WTL_WINDOW_MENU_HPP
