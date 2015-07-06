//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\ClientRectProperty.cpp
//! \brief Implementation for 'ClientRect' property (avoids circular reference regarding WindowBase template)
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////

#include "wtl/WTL.hpp"
#include "wtl/windows/properties/ClientRectProperty.hpp"     //!< ClientRectProperty
#include "wtl/windows/WindowBase.hpp"                        //!< WindowBase

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // ClientRectPropertyImpl::get const
  //! Get the client rectangle
  //! 
  //! \return value_t - Current rectangle if window exists, otherwise 'initial' rectangle
  //!
  //! \throw wtl::logic_error - Window is using default size or location
  //! \throw wtl::platform_error - Unable to query client rectangle
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename ClientRectPropertyImpl<ENC>::value_t  ClientRectPropertyImpl<ENC>::get() const 
  {
    RectL rc;    //!< New client rectangle

    // [EXISTS] Return current window rectangle
    if (this->Window.exists())
    {
      // Query client rectangle
      if (!::GetClientRect(this->Window, &native_cast(rc)))
        throw platform_error(HERE, "Unable to query window rectangle");
    }
    else
    {
      // [¬EXISTS] Ensure size/position not 'default'
      if (this->Window.Size == DefaultSize || this->Window.Position == DefaultPosition)
        throw logic_error(HERE, "Cannot generate a window rectangle from default co-ordinates");

      // Calculate client from window rectangle 
      rc = RectL(this->Window.Position(), this->Window.Size());
      if (!::AdjustWindowRectEx(&native_cast(rc), 
                                enum_cast(this->Window.Style.get()), 
                                boolean_cast(!this->Window.Menu.empty()), 
                                enum_cast(this->Window.StyleEx.get())))
        throw platform_error(HERE, "Unable to calculate window rectangle from client");
    }

    // Return client rectangle
    return rc;
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // ClientRectPropertyImpl::set 
  //! Set the current client rectangle iff window exists, otherwise 'initial' rectangle
  //! 
  //! \param[in] rc - New client rectangle
  //! 
  //! \throw wtl::platform_error - Unable to calculate window rectangle from client
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  ClientRectPropertyImpl<ENC>::set(value_t rc) 
  {
    RectL rc(val);   //!< New window rectangle

    // Calculate window rectangle 
    if (!::AdjustWindowRectEx(&native_cast(rc), 
                              enum_cast(this->Window.Style.get()), 
                              boolean_cast(!this->Window.Menu.empty()), 
                              enum_cast(this->Window.StyleEx.get())))
      throw platform_error(HERE, "Unable to calculate window rectangle from client");

    // Set window rectangle
    this->Window.WindowRect = rc;
  }

      
} // namespace wtl

