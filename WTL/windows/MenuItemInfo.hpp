//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\MenuItemInfo.hpp
//! \brief Encpsulates menu items
//! \date 6 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MENU_ITEM_INFO_HPP
#define WTL_MENU_ITEM_INFO_HPP

#include "wtl/WTL.hpp"
#include "wtl/casts/BaseCast.hpp"         //!< base_cast
#include "wtl/casts/EnumCast.hpp"         //!< enum_cast
#include "wtl/casts/OpaqueCast.hpp"       //!< opaque_cast
#include "wtl/utils/Handle.hpp"           //!< Handle
#include "wtl/utils/Clear.hpp"            //!< clear
#include "wtl/traits/EncodingTraits.hpp"  //!< Encoding
#include "wtl/traits/MenuTraits.hpp"      //!< HMenu
#include "wtl/platform/MenuFlags.hpp"     //!< MenuItemState,MenuItemFlag,MenuItemType
#include "wtl/windows/Command.hpp"        //!< Command,CommandState
#include "wtl/windows/CommandGroup.hpp"   //!< CommandGroup

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct MenuItemInfo - Provides the menu item data for a GuiCommand
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

      
} // namespace wtl

#endif  // WTL_MENU_ITEM_INFO_HPP
