////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\WindowMenu.hpp
//! \brief Provides window menus and popup menus
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_MENU_HPP
#define WTL_WINDOW_MENU_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  

  ///////////////////////////////////////////////////////////////////////////////
  //! \struct MenuItemInfo - Menu item info
  //! 
  //! \tparam ENC - Menu item text character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct MenuItemInfo : getType_t<ENC,::MENUITEMINFOA,::MENUITEMINFOW>
  {
    // ------------------- TYPES & CONSTANTS -------------------

    //! \alias base - Define base type
    using base = getType_t<ENC,::MENUITEMINFOA,::MENUITEMINFOW>;

    //! \alias type - Define own type
    using type = MenuItemInfo<ENC>;

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \var encoding - Define item text character encoding
    static constexpr Encoding  encoding = ENC;

    // -------------------- REPRESENTATION ---------------------

    // --------------------- CONSTRUCTION ----------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // MenuItemInfo::MenuItemInfo
    //! Create empty menu item
    ///////////////////////////////////////////////////////////////////////////////
    MenuItemInfo()
    {
      wtl::clear(static_cast<base&>(*this));
      this->cbSize = sizeof(base);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // MenuItemInfo::MenuItemInfo
    //! Create menu item for an action
    //! 
    //! \param[in] const& cmd - Action
    //! 
    //! \throw wtl::invalid_argument - Unsupported action state
    ///////////////////////////////////////////////////////////////////////////////
    MenuItemInfo(const Action<encoding>& cmd) : MenuItemInfo()
    {
      // Populate
      this->fMask      = enum_cast(MenuItemFlag::String|MenuItemFlag::State|MenuItemFlag::Id|MenuItemFlag::Data|MenuItemFlag::FType);
      this->fType      = enum_cast(MenuItemType::String|MenuItemType::OwnerDraw);
      this->dwTypeData = (char_t*)cmd.name();
      this->cch        = strlen_t(cmd.name());
      this->wID        = enum_cast(cmd.ident());
      this->fState     = enum_cast(toState(cmd.state()));
      this->dwItemData = opaque_cast(cmd);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // MenuItemInfo::MenuItemInfo
    //! Create pop-up menu item for an action group
    //! 
    //! \param[in] const& group - Action Group
    //! \param[in] const& popup - Popup menu handle
    //! 
    //! \throw wtl::invalid_argument - Unsupported state of action within group
    ///////////////////////////////////////////////////////////////////////////////
    MenuItemInfo(const ActionGroup<encoding>& group, const HMenu& popup) : MenuItemInfo()
    {
      // Populate
      this->fMask      = enum_cast(MenuItemFlag::String|MenuItemFlag::State|MenuItemFlag::Id|MenuItemFlag::Data|MenuItemFlag::FType|MenuItemFlag::SubMenu);
      this->fType      = enum_cast(MenuItemType::String|MenuItemType::OwnerDraw);
      this->dwTypeData = (char_t*)group.name();
      this->cch        = strlen_t(group.name());
      this->wID        = enum_cast(group.ident());
      this->fState     = enum_cast(MenuItemState::Enabled);
      this->dwItemData = opaque_cast(group);
      this->hSubMenu   = popup;
    }

    virtual ~MenuItemInfo()
    {}
    
    // ------------------------ STATIC -------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // MenuItemInfo::toState
    //! Get menu item state from an action state
    //! 
    //! \param[in] s - Action state
    //! 
    //! \throw wtl::invalid_argument - Unsupported state 
    ///////////////////////////////////////////////////////////////////////////////
    static MenuItemState toState(ActionState s)
    {
      switch (s)
      {
      case ActionState::Enabled:   return MenuItemState::Enabled;
      case ActionState::Disabled:  return MenuItemState::Disabled;
      }

      // Error: Unsupported
      throw invalid_argument(HERE, "Unsupported command state");
    }
    
    // ---------------------- ACCESSORS ------------------------			
  };


  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct PopupMenu - Encapsulates a popup menu
  //! 
  //! \tparam ENC - Menu text character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct PopupMenu 
  {
    // ------------------- TYPES & CONSTANTS -------------------

    //! \alias type - Define own type
    using type = PopupMenu<ENC>;

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias iteminfo_t - Define item info type
    using iteminfo_t = MenuItemInfo<ENC>;
    
    //! \alias type - Define own type
    using type = PopupMenu<ENC>;

    //! \var encoding - Define item text character encoding
    static constexpr Encoding  encoding = ENC;

  protected:
    //! \alias collection_t - Define collection type
    using collection_t = List<ActionPtr<ENC>>;

    //! \alias const_iterator - Define collection iterator type
    using const_iterator = typename collection_t::const_iterator;
    
    // -------------------- REPRESENTATION ---------------------
  public:
    OwnerDrawEvent<encoding>   OwnerDraw;     //!< 'Owner draw' event 

  protected:
    HMenu         Handle;     //!< Menu handle
    collection_t  Items;      //!< Menu command items
    
    // --------------------- CONSTRUCTION ----------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // PopupMenu::PopupMenu
    //! Populate from an Action Group
    //!
    //! \param[in] const& group - Action group
    ///////////////////////////////////////////////////////////////////////////////
    PopupMenu(const ActionGroup<ENC>& group) : Handle(MenuType::Popup) 
    {
      // Owner draw handler
      OwnerDraw += new OwnerDrawEventHandler<encoding>(this, &PopupMenu::onOwnerDraw);

      // Insert items from group
      for (auto& cmd : group)
        *this += cmd.second;
    }

    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // PopupMenu::begin const
    //! Get read-only position of first action
    //! 
    //! \return const_iterator - Position of first action
    ///////////////////////////////////////////////////////////////////////////////
    const_iterator  begin() const
    {
      return Items.cbegin();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // PopupMenu::end const
    //! Get read-only position of last action
    //! 
    //! \return const_iterator - Position of last action
    ///////////////////////////////////////////////////////////////////////////////
    const_iterator  end() const
    {
      return Items.cend();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // PopupMenu::handle const
    //! Get the shared menu handle 
    //! 
    //! \return const HMenu& - Shared menu handle
    ///////////////////////////////////////////////////////////////////////////////
    const HMenu& handle() const
    {
      return Handle;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // PopupMenu::size const
    //! Get the number of items
    //! 
    //! \return int32 - Number of items
    //! 
    //! \throw wtl::platform_error - Unable to query item count
    ///////////////////////////////////////////////////////////////////////////////
    int32  size() const
    {
      // Query item count
      int32 num = GetMenuItemCount(Handle);
      if (num != -1)
        return num;

      // Error: failed
      throw platform_error(HERE, "Unable to query menu item count");
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // PopupMenu::find const
    //! Searches for an Action 
    //! 
    //! \param[in] id - Action Id
    //! \return const_iterator - Position of action, if found, otherwise 'end'
    ///////////////////////////////////////////////////////////////////////////////
    const_iterator find(CommandId id) const
    {
      // Lookup item
      return Items.find_if([id] (const ActionPtr<encoding>& action) { return action->ident() == id; });
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // PopupMenu::operator[] const
    //! Get an Action 
    //! 
    //! \param[in] id - Action Id
    //!
    //! \throw wtl::out_of_range - Action not found
    ///////////////////////////////////////////////////////////////////////////////
    ActionPtr<encoding> operator[] (CommandId id) const
    {
      // Lookup item
      auto pos = find(id);
      if (pos != Items.cend())
        return *pos;

      // [ERROR] Not found
      throw out_of_range(HERE, "Action %d not found", enum_cast(id));
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // PopupMenu::operator ::HMENU const
    //! Implicit user conversion to native menu handle
    //! 
    //! \return ::HMENU - Native handle
    ///////////////////////////////////////////////////////////////////////////////
    operator ::HMENU() const
    {
      return Handle;
    }
    
    // ----------------------- MUTATORS ------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // PopupMenu::insert
    //! Inserts a Gui Action menu item at a position
    //! 
    //! \param[in] idx - Zero-based position 
    //! \param[in] const& cmd - Gui Action
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing command
    //! \throw wtl::platform_error - Unable to insert menu item
    ///////////////////////////////////////////////////////////////////////////////
    void  insert(int32 idx, const ActionPtr<encoding>& cmd)
    {
      REQUIRED_PARAM(cmd);

      MenuItemInfo<encoding> item(*cmd);     //!< Generate Gui Command menu item

      // Insert item into menu
      if (!getFunc<encoding>(::InsertMenuItemA,::InsertMenuItemW)(Handle, idx, TRUE, &item))
        throw platform_error(HERE, "Unable to insert menu item");

      // Insert Action into collection 
      Items.insert(Items.at(idx), cmd);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // PopupMenu::operator += 
    //! Appends a Gui Action menu item 
    //! 
    //! \param[in] const& cmd - Gui action
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing command
    //! \throw wtl::platform_error - Unable to insert menu item
    ///////////////////////////////////////////////////////////////////////////////
    PopupMenu&  operator += (const ActionPtr<encoding>& cmd)
    {
      REQUIRED_PARAM(cmd);

      // Append action to menu
      insert(size(), cmd);
      return *this;
    }

  protected:
    ///////////////////////////////////////////////////////////////////////////////
    // PopupMenu::onOwnerDraw
    //! Called in response to a reflected 'owner draw' message 
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Message result and routing
    ///////////////////////////////////////////////////////////////////////////////
    virtual wtl::LResult  onOwnerDraw(wtl::OwnerDrawEventArgs<encoding>& args) 
    { 
      //CharArray<encoding,128> text;

      // Draw background
      args.Graphics.fill(args.Rect, wtl::StockBrush::Gold);

      // Draw items
      //for (auto& cmd : *this)
      //{
      //  // Draw text
      //  args.Graphics.write<encoding>(cmd->name(), strlen_t(cmd->name()), args.Rect, DrawTextFlags::Centre|DrawTextFlags::VCentre);

      //  // Advance rectangle
      //  args.Rect += PointL(0, 18);
      //}

      // Handled
      return 0;
    }
  };



  ///////////////////////////////////////////////////////////////////////////////
  //! \struct WindowMenu - Encapsulates a window menu
  //! 
  //! \tparam ENC - Menu text character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct WindowMenu 
  {
    // ------------------- TYPES & CONSTANTS -------------------
    
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
    
    // -------------------- REPRESENTATION ---------------------
  public:
    OwnerDrawEvent<encoding>      OwnerDraw;        //!< Raised by 'WM_DRAWITEM'
    OwnerMeasureEvent<encoding>   OwnerMeasure;     //!< Raised by 'WM_MEASUREITEM'

  protected:
    HMenu         Handle;     //!< Menu handle
    collection_t  Popups;     //!< Popup menu collection
    
    // --------------------- CONSTRUCTION ----------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // WindowMenu::WindowMenu
    //! Create empty window menu 
    ///////////////////////////////////////////////////////////////////////////////
    WindowMenu() : Handle(MenuType::Window) 
    {
      // Owner draw handler
      OwnerDraw += new OwnerDrawEventHandler<encoding>(this, &WindowMenu::onOwnerDraw);
      OwnerMeasure += new OwnerMeasureEventHandler<encoding>(this, &WindowMenu::onOwnerMeasure);
    }

    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // WindowMenu::getGroup const
    //! Get a command group 
    //! 
    //! \param[in] id - Command group Id
    //! \return ActionGroup<encoding>* - Command group
    //! 
    //! \throw wtl::platform_error - Unable to find command group
    ///////////////////////////////////////////////////////////////////////////////
    //ActionGroup<encoding>* getGroup(CommandGroupId id) const
    //{
    //  MenuItemInfo<encoding> info;
    //  
    //  // Query item data
    //  if (!getFunc<encoding>(::GetMenuItemInfoA,GetMenuItemInfoW)(Handle, enum_cast(id), FALSE, &info))
    //    throw platform_error(HERE, "Unable to query menu item");

    //  // Extract command
    //  return opaque_cast<ActionGroup<encoding>>(info.dwItemData);
    //}

    ///////////////////////////////////////////////////////////////////////////////
    // WindowMenu::handle const
    //! Get the shared menu handle 
    //! 
    //! \return const HMenu& - Shared menu handle
    ///////////////////////////////////////////////////////////////////////////////
    const HMenu& handle() const
    {
      return Handle;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowMenu::size const
    //! Get the number of pop-up menus
    //! 
    //! \return int32 - Number of pop-up menus
    //!
    //! \throw wtl::platform_error - Unable to query menu item count
    ///////////////////////////////////////////////////////////////////////////////
    int32  size() const
    {
      // Query item count
      int32 num = GetMenuItemCount(Handle);
      if (num != -1)
        return num;

      // Error: failed
      throw platform_error(HERE, "Unable to query menu item count");
    }

    ///////////////////////////////////////////////////////////////////////////////
    // WindowMenu::operator ::HMENU
    //! Implicit user conversion to native menu handle
    //! 
    //! \return ::HMENU - Native handle
    ///////////////////////////////////////////////////////////////////////////////
    operator ::HMENU() const
    {
      return Handle;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowMenu::find const
    //! Searches for an Action 
    //! 
    //! \param[in] id - Action Id
    //! \return const_iterator - Position of action, if found, otherwise 'end'
    ///////////////////////////////////////////////////////////////////////////////
    //typename popup_t::const_iterator find(CommandId id) const
    //{
    //  // Lookup item
    //  return Items.find_if([id] (const ActionPtr<encoding>& action) { return action->ident() == id; });
    //}

    ///////////////////////////////////////////////////////////////////////////////
    // WindowMenu::operator[] const
    //! Search all popup menus for an action
    //! 
    //! \param[in] id - Action Id
    //! \return ActionPtr<encoding> - Shared action pointer
    ///////////////////////////////////////////////////////////////////////////////
    ActionPtr<encoding> operator[] (CommandId id) const
    {
      // Search all popups
      for (auto& popup : Popups)
      {
        // Find & return action
        auto cmd = popup.find(id);
        if (cmd != popup.end())
          return *cmd;
      }
      
      // [ERROR] Not found
      throw out_of_range(HERE, "Action %d not found", enum_cast(id));
    }

    // ----------------------- MUTATORS ------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowMenu::insert
    //! Inserts a new popup menu item containing the Actions of an ActionGroup 
    //! 
    //! \param[in] idx - Zero-based position 
    //! \param[in] const& group - Shared action group
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing command group
    //! \throw wtl::platform_error - Unable to insert menu item
    ///////////////////////////////////////////////////////////////////////////////
    void insert(int32 idx, const ActionGroupPtr<encoding>& group)
    {
      REQUIRED_PARAM(group);

      // Insert new Popup menu into collection and extract its handle
      auto popup = Popups.emplace(Popups.at(idx), *group);

      MenuItemInfo<encoding> item(*group, popup->handle());     //!< Generate pop-up menu item 

      // Insert menu item 
      if (!getFunc<encoding>(::InsertMenuItemA,::InsertMenuItemW)(Handle, idx, TRUE, &item))
        throw platform_error(HERE, "Unable to insert menu item");
    }

    ///////////////////////////////////////////////////////////////////////////////
    // WindowMenu::operator += 
    //! Appends a new popup menu item containing the Actions of an ActionGroup 
    //! 
    //! \param[in] const& group - Shared command group
    //! \return type& - Reference to self
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing command group
    //! \throw wtl::platform_error - Unable to insert menu item
    ///////////////////////////////////////////////////////////////////////////////
    type&  operator += (const ActionGroupPtr<encoding>& group)
    {
      insert(size(), group);
      return *this;
    }

  protected:
    ///////////////////////////////////////////////////////////////////////////////
    // WindowMenu::onOwnerDraw
    //! Called in response to a reflected 'owner draw' message 
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Message result and routing
    ///////////////////////////////////////////////////////////////////////////////
    virtual wtl::LResult  onOwnerDraw(wtl::OwnerDrawEventArgs<encoding>& args) 
    { 
      // Lookup action 
      auto action = operator[](args.Ident);

      // Draw background
      args.Graphics.fill(args.Rect, wtl::StockBrush::Gold);

      // Draw text
      //args.Graphics.write(text, args.Rect, DrawTextFlags::Centre|DrawTextFlags::VCentre);

      // Handled
      return 0;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // WindowMenu::onOwnerMeasure
    //! Called in response to a reflected 'owner measure' message 
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Message result and routing
    ///////////////////////////////////////////////////////////////////////////////
    virtual wtl::LResult  onOwnerMeasure(wtl::OwnerMeasureEventArgs<encoding>& args) 
    { 
      // Lookup action 
      auto action = operator[](args.Ident.Action);

      // Measure text
      args.Size = args.Graphics.measure(action->name());

      // Handled
      return 0;
    }

  };

}

#endif  // WTL_WINDOW_MENU_HPP
