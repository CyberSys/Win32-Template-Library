//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\skins\ThemedSkin.hpp
//! \brief Provides a themed look & feel control renderer
//! \date 1 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_THEMED_SKIN_HPP
#define WTL_THEMED_SKIN_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/Rectangle.hpp>                      //!< Rect
#include <wtl/utils/Size.hpp>                           //!< Size
#include <WTL/platform/Metrics.hpp>                     //!< Metrics
#include <wtl/gdi/DeviceContext.hpp>                    //!< DeviceContext
#include <wtl/gdi/Theme.hpp>                            //!< Theme
#include <WTL/windows/Window.hpp>                       //!< Window
#include <WTL/windows/WindowSkin.hpp>                   //!< IWindowSkin
#include <WTL/windows/controls/button/Button.hpp>       //!< Button
#include <WTL/windows/controls/checkbox/CheckBox.hpp>   //!< CheckBox
#include <WTL/windows/controls/combobox/ComboBox.hpp>   //!< ComboBox
#include <WTL/windows/controls/edit/Edit.hpp>           //!< Edit

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ThemedSkin - Renders standard controls using a 'themed' look n feel
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ThemedSkin : IWindowSkin<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = ThemedSkin<ENC>;
  
    //! \alias base - Define base type
    using base = IWindowSkin<ENC>;
    
    //! \var encoding - Inherit character encoding
    static constexpr Encoding  encoding = base::encoding;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  private:
    //! \var Instance - Singleton instance
    static ThemedSkin<ENC> Instance;

    // ------------------------------------ CONSTRUCTION ------------------------------------
  private:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ThemedSkin::ThemedSkin
    //! Control singleton instance and set as default window skin
    /////////////////////////////////////////////////////////////////////////////////////////
    ThemedSkin()
    {
      SkinFactory<encoding>::set(*this);
    }

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
		DISABLE_COPY(ThemedSkin);			//!< Singleton
		DISABLE_MOVE(ThemedSkin);			//!< Singleton
		
    // ----------------------------------- STATIC METHODS -----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ThemedSkin::get
    //! Get the singleton instance
    //! 
    //! \return type& - Reference to singleton
    /////////////////////////////////////////////////////////////////////////////////////////
    static type& get()
    {
      return Instance;
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ThemedSkin::draw const
    //! Draws a standard button control
    //! 
    //! \param[in,out] &btn - Button to be drawn
    //! \param[in,out] &dc - Output device context
    //! \param[in] const &rc - Drawing rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    void draw(Button<ENC>& btn, DeviceContext& dc, const RectL& rc) const override
    {
      Theme theme(btn.handle(), L"Button");

      // Determine state
      ::PUSHBUTTONSTATES state = PBS_NORMAL;
      if (!btn.Enabled)
        state = PBS_DISABLED;
      else if (btn.State == ButtonState::Pushed) //else if (args.State && OwnerDrawState::Selected)
        state = PBS_PRESSED;
      else if (btn.isMouseOver())
        state = PBS_HOT;
      
      // Draw background 
      theme.fill(dc, BP_PUSHBUTTON, state, rc);

      // Query content rect
      RectL rcContent = theme.content(dc, BP_CHECKBOX, state, rc);

      // Pressed: Offset drawing rect
      if (state == PBS_PRESSED)
        rcContent += PointL(1,1);

      // Draw icon
      if (btn.Icon.exists()) 
      {
        RectL rcIcon = rcContent.arrange(Metrics::WindowIcon, {RectL::FromLeft,Metrics::WindowEdge.Width}, RectL::Centre);
        dc.draw(btn.Icon, rcIcon);
      }
      
      // Calculate text rectangle
      RectL rcText = rcContent;
      if (btn.Icon.exists()) 
        rcText.Left += Metrics::WindowIcon.Width + Metrics::WindowEdge.Width;

      // Draw text
      if (state != PBS_DISABLED)
        theme.write(dc, BP_PUSHBUTTON, state, btn.Text(), rcText, DrawTextFlags::Centre|DrawTextFlags::VCentre|DrawTextFlags::SingleLine);
      else
      {
        const auto grayString = choose<ENC>(::GrayStringA,::GrayStringW);
        grayString(dc.handle(), StockBrush::AppWorkspace, nullptr, (LPARAM)btn.Text().c_str(), btn.Text().size(), rcText.Left, rcText.Top, rcText.width(), rcText.height());
      }
      
      // [FOCUS] Draw focus rectangle
      if (btn.Focus)   
      {
        RectL rcFocus = rcContent.inflate(-Metrics::WindowEdge);
        dc.focus(rcFocus);
      }
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ThemedSkin::measure const
    //! Measures a standard button control 
    //! 
    //! \param[in,out] &btn - Button to be measured
    //! \param[in,out] &dc - Output device context
    //! \return SizeL - Required size
    /////////////////////////////////////////////////////////////////////////////////////////
    SizeL measure(Button<ENC>& btn, DeviceContext& dc) const override
    {
      // Measure button text
      return dc.measure(btn.Text());
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ThemedSkin::draw const
    //! Draws a standard checkbox control
    //! 
    //! \param[in,out] &chk - CheckBox to be drawn
    //! \param[in,out] &dc - Output device context
    //! \param[in] const &rc - Drawing rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    void draw(CheckBox<ENC>& chk, DeviceContext& dc, const RectL& rc) const override
    {
      Theme theme(chk.handle(), L"Button");
        
      // Determine state
      CHECKBOXSTATES state = CBS_UNCHECKEDNORMAL;
      if (!chk.Enabled)
        state = CBS_UNCHECKEDDISABLED;
      else if (chk.Check == ButtonState::Checked)    //else if (args.State && OwnerDrawState::Selected)
        state = CBS_UNCHECKEDPRESSED;
      else if (chk.isMouseOver())
        state = CBS_UNCHECKEDHOT;
      
      //if (chk.Checked == ButtonState::Checked)  //if (args.State && OwnerDrawState::Checked)
      //  state += (CBS_CHECKEDNORMAL-1);
      //if (chk.State && ButtonState::Checked)  
      //  state += (CBS_CHECKEDNORMAL-1);
      //else if (chk.Checked == ButtonState::Indeterminate)
      //  state += (CBS_MIXEDNORMAL-1);

      // Query content rect
      RectL rcContent = theme.content(dc, BP_CHECKBOX, state, rc);

      // Draw background
      dc.fill(rc, StockBrush::ButtonFace);

      // Calculate checkbox / text rectangles
      SizeL szCheckBox = theme.measure(dc, BP_CHECKBOX, state);
      RectL rcCheckBox = rcContent.arrange(szCheckBox, {RectL::FromLeft,Metrics::WindowEdge.Width}, RectL::Centre);
      
      // Draw checkbox
      theme.fill(dc, BP_CHECKBOX, state, rcCheckBox);

      // Draw text
      rcContent.Left = rcCheckBox.Right + Metrics::WindowEdge.Width;
      theme.write(dc, BP_CHECKBOX, state, chk.Text(), rcContent, DrawTextFlags::Left|DrawTextFlags::VCentre|DrawTextFlags::SingleLine);
      
      // [FOCUS] Draw focus rectangle
      if (chk.Focus)    //if (args.State && OwnerDrawState::Focus)
      {
        RectL rcText = theme.measure(dc, BP_CHECKBOX, state, chk.Text(), rcContent, DrawTextFlags::Left|DrawTextFlags::VCentre|DrawTextFlags::SingleLine);
        dc.focus(rcText);
      }
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ThemedSkin::measure const
    //! Measures a standard CheckBox control 
    //! 
    //! \param[in,out] &chk - CheckBox to be measured
    //! \param[in,out] &dc - Output device context
    //! \return SizeL - Required size
    /////////////////////////////////////////////////////////////////////////////////////////
    SizeL measure(CheckBox<ENC>& chk, DeviceContext& dc) const override
    {
      Theme theme(chk.handle(), L"Button");

      // Measure checkbox + text + edges
      return theme.measure(dc, BP_CHECKBOX, CBS_UNCHECKEDNORMAL) 
           + dc.measure(chk.Text()) 
           + SizeL(3*Metrics::WindowEdge.Width, 0);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ThemedSkin::draw const
    //! Draws a standard ComboBox control
    //! 
    //! \param[in,out] &cmb - ComboBox to be drawn
    //! \param[in,out] &dc - Output device context
    //! \param[in] const &rc - Drawing rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    void draw(ComboBox<ENC>& cmb, DeviceContext& dc, const RectL& rc) const override
    {
      //! TODO: Drawing code not yet implemented
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ThemedSkin::measure const
    //! Measures a standard ComboBox control 
    //! 
    //! \param[in,out] &cmb - ComboBox to be measured
    //! \param[in,out] &dc - Output device context
    //! \return SizeL - Required size
    /////////////////////////////////////////////////////////////////////////////////////////
    SizeL measure(ComboBox<ENC>& cmb, DeviceContext& dc) const override
    {
      //! TODO: Measuring code not yet implemented
      return {};
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ThemedSkin::draw const
    //! Draws a standard Edit control
    //! 
    //! \param[in,out] &edt - Edit to be drawn
    //! \param[in,out] &dc - Output device context
    //! \param[in] const &rc - Drawing rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    void draw(Edit<ENC>& edt, DeviceContext& dc, const RectL& rc) const override
    {
      //! TODO: Drawing code not yet implemented
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ThemedSkin::measure const
    //! Measures a standard Edit control 
    //! 
    //! \param[in,out] &edt - Edit to be measured
    //! \param[in,out] &dc - Output device context
    //! \return SizeL - Required size
    /////////////////////////////////////////////////////////////////////////////////////////
    SizeL measure(Edit<ENC>& edt, DeviceContext& dc) const override
    {
      //! TODO: Measuring code not yet implemented
      return {};
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ThemedSkin::draw const
    //! Fallback override for drawing a window
    //! 
    //! \param[in,out] &wnd - Window to be drawn
    //! \param[in,out] &dc - Output device context
    //! \param[in] const &rc - Drawing rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    void draw(Window<ENC>& wnd, DeviceContext& dc, const RectL& rc) const override
    {
      Theme theme(wnd.handle(), L"Window");

      // Draw window background
      dc.fill(rc, theme.brush(ThemeColour::Window));   

      // Query window menu
      if (!wnd.Menu.empty())
      {
        ::MENUBARINFO bar { sizeof(::MENUBARINFO) };
        Theme menu(wnd.handle(), L"Menu");

        // Draw window menu bar     // [BUG] Attempting to draw into the non-client area
        ::GetMenuBarInfo(wnd.handle(), OBJID_MENU , 0, &bar);
        menu.fill(dc, MENU_BARBACKGROUND, MB_INACTIVE, wnd.toClient(bar.rcBar));
      }
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \var ThemedSkin<ENC>::Instance - Singleton instance
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  ThemedSkin<ENC> ThemedSkin<ENC>::Instance;


} // namespace wtl


#endif // WTL_THEMED_SKIN_HPP
