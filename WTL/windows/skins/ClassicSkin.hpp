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
#include <WTL/windows/WindowSkin.hpp>             //!< IWindowSkin

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
    
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    //! \var Instance - Temporary singleton
    static type Instance;

    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
		DISABLE_COPY(ClassicSkin);			//!< Singleton
		DISABLE_MOVE(ClassicSkin);			//!< Singleton
		
    // ----------------------------------- STATIC METHODS -----------------------------------
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ClassicSkin::draw const
    //! Draws a standard button control
    //! 
    //! \param[in,out] &btn - Button to be drawn
    //! \param[in,out] &dc - Output device context
    //! \param[in] const &rc - Drawing rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    void draw(Button<ENC>& btn, DeviceContext& dc, const RectL& rc) const override
    {
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ClassicSkin::measure const
    //! Measures a standard button control 
    //! 
    //! \param[in,out] &btn - Button to be measured
    //! \param[in,out] &dc - Output device context
    //! \return SizeL - Required size
    /////////////////////////////////////////////////////////////////////////////////////////
    SizeL measure(Button<ENC>& btn, DeviceContext& dc) const override
    {
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ClassicSkin::draw const
    //! Fallback override for drawing a window
    //! 
    //! \param[in,out] &wnd - Window to be drawn
    //! \param[in,out] &dc - Output device context
    //! \param[in] const &rc - Drawing rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    void draw(WindowBase<ENC>& wnd, DeviceContext& dc, const RectL& rc) const override
    {
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ClassicSkin::measure const
    //! Fallback override for measuring a window
    //! 
    //! \param[in,out] &btn - Window to be measured
    //! \param[in,out] &dc - Output device context
    //! \return SizeL - Required size
    /////////////////////////////////////////////////////////////////////////////////////////
    SizeL measure(WindowBase<ENC>& wnd, DeviceContext& dc) const override
    {
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };

  //! \var ClassicSkin<ENC>::Instance - Temporary singleton
  template <Encoding ENC>
  ClassicSkin<ENC>  ClassicSkin<ENC>::Instance;

} // namespace wtl


#endif // WTL_SKIN_FACTORY_HPP
