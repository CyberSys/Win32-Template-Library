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
      this->cbSize = sizeof(base);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // MenuItemInfo::MenuItemInfo
    //! Create menu item from Gui Command
    //! 
    //! \param[in] const* cmd - Gui command
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing command -or- [Always] Unsupported command state
    ///////////////////////////////////////////////////////////////////////////////
    MenuItemInfo(const IGuiCommand<encoding>* cmd) : MenuItemInfo()
    {
      REQUIRED_PARAM(cmd);

      // Populate
      this->fMask      = enum_cast(MenuItemFlag::String|MenuItemFlag::State|MenuItemFlag::Id|MenuItemFlag::Data|MenuItemFlag::Type);
      this->fType      = enum_cast(MenuItemType::String|MenuItemType::OwnerDraw);
      this->dwTypeData = (char_t*)cmd->name();
      this->wID        = enum_cast(cmd->ident());
      this->fState     = enum_cast(toState(cmd->state()));
      this->dwItemData = opaque_cast(*cmd);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // MenuItemInfo::MenuItemInfo
    //! Create pop-up menu item for Gui Command Group
    //! 
    //! \param[in] const& group - Gui Command Group
    //! \param[in] const& popup - Popup menu handle
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing group -or- [Always] Unsupported state of command within group
    ///////////////////////////////////////////////////////////////////////////////
    MenuItemInfo(const GuiCommandGroup<encoding>& group, const HMenu& popup) : MenuItemInfo()
    {
      REQUIRED_PARAM(group);

      // Populate
      this->fMask      = enum_cast(MenuItemFlag::String|MenuItemFlag::State|MenuItemFlag::Id|MenuItemFlag::Data|MenuItemFlag::Type|MenuItemFlag::SubMenu);
      this->fType      = enum_cast(MenuItemType::String|MenuItemType::OwnerDraw);
      this->dwTypeData = (char_t*)group.name();
      this->wID        = enum_cast(group.ident());
      this->fState     = enum_cast(MenuItemState::Enabled);
      this->dwItemData = opaque_cast(group);
      this->hSubMenu   = popup;
    }
    
    // ------------------------ STATIC -------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // MenuItemInfo::toState
    //! Get menu item state from Gui Command state
    //! 
    //! \param[in] s - Gui command state
    ///////////////////////////////////////////////////////////////////////////////
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
    
    // ---------------------- ACCESSORS ------------------------			
  };


  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct PopupMenu - Encapsulates a window menu
  //! 
  //! \tparam ENC - Menu text character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct PopupMenu : List<shared_command_t<ENC>,ListType::Double>
  {
    // ------------------- TYPES & CONSTANTS -------------------

    //! \alias base - Define base type
    using base = List<shared_command_t<ENC>,ListType::Double>;

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias command_t - Define command type
    //using command_t = IGuiCommand<ENC>;
    
    //! \alias cmdgroup_t - Define command group type
    //using cmdgroup_t = IGuiCommandGroup<ENC>;

    //! \alias item_t - Define item type
    using item_t = MenuItemInfo<ENC>;
    
    //! \alias type - Define own type
    using type = PopupMenu<ENC>;

    //! \var encoding - Define item text character encoding
    static constexpr Encoding  encoding = ENC;
    
    // -------------------- REPRESENTATION ---------------------
  public:
    OwnerDrawEvent<encoding>   OwnerDraw;     //!< 'Owner draw' event 

  protected:
    HMenu      Handle;     //!< Menu handle
    
    // --------------------- CONSTRUCTION ----------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // PopupMenu::PopupMenu
    //! Create menu with zero items
    ///////////////////////////////////////////////////////////////////////////////
    //PopupMenu() : Handle(MenuType::Popup) 
    //{
    //  // Owner draw handler
    //  OwnerDraw += new OwnerDrawEventHandler<encoding>(this, &PopupMenu::onOwnerDraw);
    //}

    ///////////////////////////////////////////////////////////////////////////////
    // PopupMenu::PopupMenu
    //! Populate from group of Gui Commands
    //!
    //! \param[in] const& group - Group of Gui Commands
    ///////////////////////////////////////////////////////////////////////////////
    PopupMenu(const GuiCommandGroup<ENC>& group) : Handle(MenuType::Popup) 
    {
      // Owner draw handler
      OwnerDraw += new OwnerDrawEventHandler<encoding>(this, &PopupMenu::onOwnerDraw);

      // Populate from group
      for (auto& cmd : group)
        *this += *cmd;
    }

    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // PopupMenu::getCommand const
    //! Get the gui command for an item
    //! 
    //! \param[in] id - Command Id
    //! \return IGuiCommand<encoding>* - Gui Command
    //! 
    //! \throw wtl::platform_error - Unable to find command
    ///////////////////////////////////////////////////////////////////////////////
    IGuiCommand<encoding>* getCommand(CommandId id) const
    {
      MenuItemInfo<encoding> info;
      
      // Query item data
      if (!getFunc<ENC>(::GetMenuItemInfoA,GetMenuItemInfoW)(Handle, enum_cast(id), FALSE, &info))
        throw platform_error(HERE, "Unable to query menu item");

      // Extract command
      return opaque_cast<IGuiCommand<encoding>>(info.dwItemData);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // PopupMenu::handle const
    //! Get the native menu handle 
    //! 
    //! \return ::HMENU - Native handle
    //! 
    //! \remarks This method can be executed upon a nullptr
    ///////////////////////////////////////////////////////////////////////////////
    ::HMENU  handle() const
    {
      return this ? Handle.get() : nullptr;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // PopupMenu::size const
    //! Get the number of items
    //! 
    //! \return int32 - Number of items
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
    // PopupMenu::operator ::HMENU const
    //! Implicit user conversion to native menu handle
    //! 
    //! \return ::HMENU - Native handle
    ///////////////////////////////////////////////////////////////////////////////
    operator ::HMENU() const
    {
      return handle();
    }

    // ----------------------- MUTATORS ------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // PopupMenu::insert
    //! Inserts a Gui Command at a position
    //! 
    //! \param[in] idx - Zero-based position 
    //! \param[in] const* cmd - Gui command
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing command
    //! \throw wtl::platform_error - Unable to insert menu item
    ///////////////////////////////////////////////////////////////////////////////
    void insert(int32 idx, const IGuiCommand<encoding>* cmd)
    {
      REQUIRED_PARAM(cmd);

      MenuItemInfo<encoding> item(cmd);     //!< Generate Gui Command menu item

      // Insert item at position
      if (!getFunc<encoding>(::InsertMenuItemA,::InsertMenuItemW)(Handle, idx, FALSE, &item))
        throw platform_error(HERE, "Unable to insert menu item");

      // Insert into collection at position
      base::emplace_back(cmd);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // PopupMenu::operator += 
    //! Appends a Gui Command item 
    //! 
    //! \param[in] const* cmd - Gui command
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing command
    //! \throw wtl::platform_error - Unable to insert menu item
    ///////////////////////////////////////////////////////////////////////////////
    PopupMenu&  operator += (const IGuiCommand<encoding>* cmd)
    {
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
      CharArray<encoding,128> text;

      // Draw background
      args.Graphics.fill(args.Rect, wtl::StockBrush::Gold);

      // Draw items
      for (auto& cmd : *this)
      {
        // Draw text
        args.Graphics.write<encoding>(cmd->name(), strlen_t(cmd->name()), args.Rect, DrawTextFlags::Centre|DrawTextFlags::VCentre);

        // Advance rectangle
        args.Rect += PointL(0, 18);
      }

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
  struct WindowMenu : List<PopupMenu<ENC>,ListType::Double>
  {
    // ------------------- TYPES & CONSTANTS -------------------

    //! \alias base - Define base type
    using base = List<PopupMenu<ENC>,ListType::Double>;

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;

    //! \alias type - Define own type
    using type = WindowMenu<ENC>;

    //! \var encoding - Define item text character encoding
    static constexpr Encoding  encoding = ENC;
    
    // -------------------- REPRESENTATION ---------------------
  public:
    OwnerDrawEvent<encoding>   OwnerDraw;     //!< 'Owner draw' event 

  protected:
    HMenu      Handle;     //!< Menu handle
    
    // --------------------- CONSTRUCTION ----------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // WindowMenu::WindowMenu
    //! Create menu with zero items
    ///////////////////////////////////////////////////////////////////////////////
    WindowMenu() : Handle(MenuType::Window) 
    {
      // Owner draw handler
      OwnerDraw += new OwnerDrawEventHandler<encoding>(this, &WindowMenu::onOwnerDraw);
    }

    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // WindowMenu::getGroup const
    //! Get a command group 
    //! 
    //! \param[in] id - Command group Id
    //! \return IGuiCommandGroup<encoding>* - Command group
    //! 
    //! \throw wtl::platform_error - Unable to find command group
    ///////////////////////////////////////////////////////////////////////////////
    IGuiCommandGroup<encoding>* getGroup(CommandGroupId id) const
    {
      MenuItemInfo<encoding> info;
      
      // Query item data
      if (!getFunc<ENC>(::GetMenuItemInfoA,GetMenuItemInfoW)(Handle, enum_cast(id), FALSE, &info))
        throw platform_error(HERE, "Unable to query menu item");

      // Extract command
      return opaque_cast<cmdgroup_t>(info.dwItemData);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // WindowMenu::handle const
    //! Get the native menu handle 
    //! 
    //! \return ::HMENU - Native handle
    //! 
    //! \remarks This method can be executed upon a nullptr
    ///////////////////////////////////////////////////////////////////////////////
    ::HMENU  handle() const
    {
      return this ? Handle.get() : nullptr;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowMenu::size const
    //! Get the number of pop-up menus
    //! 
    //! \return int32 - Number of pop-up menus
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
      return handle();
    }

    // ----------------------- MUTATORS ------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowMenu::insert
    //! Inserts a command group at a position (as a pop-up menu item)
    //! 
    //! \param[in] idx - Zero-based position 
    //! \param[in] const& group - Shared command group
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing command group
    //! \throw wtl::platform_error - Unable to insert menu item
    ///////////////////////////////////////////////////////////////////////////////
    void insert(int32 idx, const GuiCommandGroup<encoding>& group)
    {
      REQUIRED_PARAM(group);

      // Find position
      typename base::iterator pos = base::begin();
      std::advance(pos, idx);

      // Insert into collection (creates handle)
      base::emplace_back(group);
      auto popup = base::back().handle(); 

      MenuItemInfo<encoding> item(group.get(), popup);     //!< Generate pop-up menu item

      // Insert menu item 
      if (!getFunc<encoding>(::InsertMenuItemA,::InsertMenuItemW)(Handle, idx, FALSE, &item))
        throw platform_error(HERE, "Unable to insert menu item");
    }

    ///////////////////////////////////////////////////////////////////////////////
    // WindowMenu::operator += 
    //! Appends a command group at a position (as a pop-up menu item)
    //! 
    //! \param[in] const& group - Shared command group
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing command group
    //! \throw wtl::platform_error - Unable to insert menu item
    ///////////////////////////////////////////////////////////////////////////////
    WindowMenu&  operator += (const shared_cmdgroup_t<encoding>& group)
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
      CharArray<encoding,128> text;

      // Draw background
      args.Graphics.fill(args.Rect, wtl::StockBrush::Gold);

      // Draw text
      args.Graphics.write(text, args.Rect, DrawTextFlags::Centre|DrawTextFlags::VCentre);

      // Handled
      return 0;
    }

  };

}

#endif  // WTL_WINDOW_MENU_HPP
