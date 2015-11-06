//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\WindowMenu.hpp
//! \brief Separate definitions for WindowMenu to resolve cyclic dependency
//! \date 6 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_MENU_HPP
#define WTL_WINDOW_MENU_HPP

#include "wtl/WTL.hpp"
#include "wtl/windows/WindowMenu.hpp"                       //!< WindowMenu
#include "wtl/windows/Window.hpp"                           //!< NativeWindow

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct WindowMenu - Encapsulates a window menu (a collection of popup menus)
  //! 
  //! \tparam ENC - Menu text character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  LResult  WindowMenu<ENC>::onOwnerDraw(OwnerDrawMenuEventArgs<encoding>& args) 
  { 
    /*Theme theme(this->handle(), L"Menu");

    RectL rcBar;
    ::MENUBARINFO bar { sizeof(MENUBARINFO) };
      
    ::GetMenuBarInfo(Handle, OBJID_MENU, 0, &bar);
    ::GetWindowRect(bar.hwndMenu, rcBar);

    theme.drawBackground(args.Graphics, MENU_BARBACKGROUND, MB_ACTIVE, args.Rect);

    theme.drawText(args.Graphics, BP_PUSHBUTTON, PBS_NORMAL, this->Text(), args.Rect, DrawTextFlags::Centre|DrawTextFlags::VCentre);*/

      

    // debug
    cdebug << object_info(__func__, "ident", args.Ident) << endl;

    // Draw background
      args.Graphics.fill(args.Rect, StockBrush::Blue);

    // [GROUP] Draw name
    if (CommandGroupPtr<encoding> group = find(command_group_id(args.Ident)))
    {
      // Query menu bar info
      ::MENUBARINFO bar { sizeof(MENUBARINFO) };
      ::GetMenuBarInfo(::WindowFromDC(args.Graphics.handle()), OBJID_MENU, 0, &bar);

      NativeWindow<encoding> wndMenu(bar.hwndMenu);
      Theme theme(wndMenu.handle(), L"Menu");
        
      theme.drawBackground(args.Graphics, MENU_BARITEM, MB_ACTIVE, args.Rect);
      theme.drawText(args.Graphics, MENU_BARITEM, MB_ACTIVE, group->name(), args.Rect, DrawTextFlags::Centre|DrawTextFlags::VCentre);

      //args.Graphics.write(group->name(), args.Rect, DrawTextFlags::Centre|DrawTextFlags::VCentre);
    }

    // [COMMAND] Draw name
    else if (auto command = find(command_id(args.Ident)))
    {
        
      args.Graphics.write(command->name(), args.Rect, DrawTextFlags::Centre|DrawTextFlags::VCentre);
    }

    // Handled
    return 0;
  }

      
} // namespace wtl

#endif  // WTL_WINDOW_MENU_HPP
