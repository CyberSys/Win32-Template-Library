//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\skins\ClassicSkin.hpp
//! \brief Provides a classic look'n'feel control renderer
//! \date 8 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_SKIN_FACTORY_HPP
#define WTL_SKIN_FACTORY_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/Rectangle.hpp>                //!< Rect
#include <wtl/utils/Size.hpp>                     //!< Size
#include <WTL/platform/Metrics.hpp>               //!< Metrics
#include <wtl/gdi/DeviceContext.hpp>              //!< DeviceContext
#include <WTL/windows/Window.hpp>                 //!< Window
#include <WTL/windows/WindowSkin.hpp>             //!< IWindowSkin
#include <WTL/windows/controls/Button.hpp>        //!< Button
#include <WTL/windows/controls/CheckBox.hpp>      //!< CheckBox
#include <WTL/windows/controls/ComboBox.hpp>      //!< ComboBox
#include <WTL/windows/controls/Edit.hpp>          //!< Edit

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ClassicSkin - Renders standard controls using a 'classic' look n feel
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ClassicSkin : IWindowSkin<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = ClassicSkin<ENC>;
  
    //! \alias base - Define base type
    using base = IWindowSkin<ENC>;
    
    //! \var encoding - Inherit character encoding
    static constexpr Encoding  encoding = base::encoding;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    //! \var Instance - Singleton instance
    static ClassicSkin<ENC> Instance;

    // ------------------------------------ CONSTRUCTION ------------------------------------
  private:
    DEFAULT_CTOR(ClassicSkin);			//!< Singleton

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
		DISABLE_COPY(ClassicSkin);			//!< Singleton
		DISABLE_MOVE(ClassicSkin);			//!< Singleton
		
    // ----------------------------------- STATIC METHODS -----------------------------------
    
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
      //! TODO: Non-themed drawing code not yet implemented
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
      //! TODO: Non-themed drawing code not yet implemented
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
      // Measure text + edges
      return dc.measure(chk.Text()) 
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
      //! TODO: Non-themed drawing code not yet implemented
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
      //! TODO: Non-themed measuring code not yet implemented
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
      //! TODO: Non-themed drawing code not yet implemented
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
      //! TODO: Non-themed measuring code not yet implemented
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
      //! Fill background
      dc.fill(rc, StockBrush::Window);
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \var ClassicSkin<ENC>::Instance - Singleton instance
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  ClassicSkin<ENC>  ClassicSkin<ENC>::Instance;

} // namespace wtl


#endif // WTL_SKIN_FACTORY_HPP
