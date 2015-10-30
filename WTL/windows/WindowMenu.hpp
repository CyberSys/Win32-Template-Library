//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\WindowMenu.hpp
//! \brief Provides window menus and popup menus
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_MENU_HPP
#define WTL_WINDOW_MENU_HPP

#include "wtl/WTL.hpp"
#include "wtl/casts/BaseCast.hpp"         //!< BaseCast
#include "wtl/casts/EnumCast.hpp"         //!< EnumCast
#include "wtl/utils/Handle.hpp"           //!< Handle
#include "wtl/utils/Clear.hpp"            //!< Clear
#include "wtl/utils/Default.hpp"          //!< Default
#include "wtl/platform/CommandId.hpp"     //!< CommandId
#include "wtl/platform/ResourceId.hpp"    //!< ResourceId
#include "wtl/platform/MenuFlags.hpp"     //!< MenuItemState
#include "wtl/gdi/StockObjects.hpp"       //!< StockBrush
#include "wtl/traits/EncodingTraits.hpp"  //!< Encoding
#include "wtl/traits/MenuTraits.hpp"      //!< HMenu
#include "wtl/windows/Command.hpp"         //!< Command
#include "wtl/windows/CommandGroup.hpp"    //!< Command
#include "wtl/windows/events/OwnerDrawMenuEvent.hpp"        //!< OwnerDrawEvent
#include "wtl/windows/events/OwnerMeasureMenuEvent.hpp"     //!< OwnerMeasureEvent

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct MenuItemInfo - Menu item info
  //! 
  //! \tparam ENC - Menu item text character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct MenuItemInfo : choose_t<ENC,::MENUITEMINFOA,::MENUITEMINFOW>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = MenuItemInfo<ENC>;

    //! \alias base - Define base type
    using base = choose_t<ENC,::MENUITEMINFOA,::MENUITEMINFOW>;

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \var encoding - Define item text character encoding
    static constexpr Encoding  encoding = ENC;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------- CONSTRUCTION -----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // MenuItemInfo::MenuItemInfo
    //! Create empty menu item
    /////////////////////////////////////////////////////////////////////////////////////////
    MenuItemInfo()
    {
      wtl::clear(base_cast(*this));
      this->cbSize = sizeof(base);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // MenuItemInfo::MenuItemInfo
    //! Create menu item for an command
    //! 
    //! \param[in] const& cmd - Command
    //! 
    //! \throw wtl::invalid_argument - Unsupported command state
    /////////////////////////////////////////////////////////////////////////////////////////
    MenuItemInfo(const Command<encoding>& cmd) : MenuItemInfo()
    {
      // Populate
      this->fMask      = enum_cast(MenuItemFlag::String|MenuItemFlag::State|MenuItemFlag::Id|MenuItemFlag::Data|MenuItemFlag::FType);
      this->fType      = enum_cast(MenuItemType::String|MenuItemType::OwnerDraw);
      this->dwTypeData = const_cast<char_t*>( cmd.name().c_str() );
      this->cch        = cmd.name().size();
      this->wID        = enum_cast(cmd.ident());
      this->fState     = enum_cast(toState(cmd.state()));
      this->dwItemData = opaque_cast(cmd);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // MenuItemInfo::MenuItemInfo
    //! Create pop-up menu item for an command group
    //! 
    //! \param[in] const& group - Command Group
    //! \param[in] const& popup - Popup menu handle
    //! 
    //! \throw wtl::invalid_argument - Unsupported state of command within group
    /////////////////////////////////////////////////////////////////////////////////////////
    MenuItemInfo(const CommandGroup<encoding>& group, const HMenu& popup) : MenuItemInfo()
    {
      // Populate
      this->fMask      = enum_cast(MenuItemFlag::String|MenuItemFlag::State|MenuItemFlag::Id|MenuItemFlag::Data|MenuItemFlag::FType|MenuItemFlag::SubMenu);
      this->fType      = enum_cast(MenuItemType::String|MenuItemType::OwnerDraw);
      this->dwTypeData = const_cast<char_t*>( group.name().c_str() );
      this->cch        = group.name().size();
      this->wID        = enum_cast(group.ident());
      this->fState     = enum_cast(MenuItemState::Enabled);
      this->dwItemData = opaque_cast(group);
      this->hSubMenu   = popup;
    }

    
	  // -------------------------------- COPY, MOVE & DESTROY  -------------------------------
  public:
    ENABLE_COPY(MenuItemInfo);      //!< Can be shallow copied
    ENABLE_MOVE(MenuItemInfo);      //!< Can be moved
    ENABLE_POLY(MenuItemInfo);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // MenuItemInfo::toState
    //! Get menu item state from an command state
    //! 
    //! \param[in] s - Command state
    //! 
    //! \throw wtl::invalid_argument - Unsupported state 
    /////////////////////////////////////////////////////////////////////////////////////////
    static MenuItemState toState(CommandState s)
    {
      switch (s)
      {
      case CommandState::Enabled:   return MenuItemState::Enabled;
      case CommandState::Disabled:  return MenuItemState::Disabled;
      }

      // Error: Unsupported
      throw invalid_argument(HERE, "Unsupported command state");
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
  };


  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct PopupMenu - Encapsulates a popup menu
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
      if (!choose<encoding>(::InsertMenuItemA,::InsertMenuItemW)(Handle, idx, True, &item))
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



  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct WindowMenu - Encapsulates a window menu
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
      if (!choose<encoding>(::InsertMenuItemA,::InsertMenuItemW)(Handle, idx, True, &item))
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
