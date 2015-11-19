//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\modules\Module.cpp
//! \brief Provides module implementation
//! \date 7 March 2015
//! \author Nick Crowley
//! \copyright © Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////

#include "Module.h"
#include <WTL/modules/ModuleCollection.h>
#include <WTL/windows/MessageWindow.hpp>

//! \namespace wtl - Windows template library
namespace wtl
{
  // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
  // ----------------------------------- REPRESENTATION -----------------------------------
  
  // ------------------------------------ CONSTRUCTION ------------------------------------
	
  /////////////////////////////////////////////////////////////////////////////////////////
  // Module::Module
  //! Create from native module handle. Adds module to 'Loaded Modules' collection.
  //!
  //! \param[in] m - Native module handle
  /////////////////////////////////////////////////////////////////////////////////////////
  Module::Module(::HMODULE m) : Handle(m, AllocType::WeakRef)
  {
    LoadedModules.add(*this);
  }
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // Module::Module
  //! Removes module from 'Loaded Modules' collection.
  /////////////////////////////////////////////////////////////////////////////////////////
  Module::~Module()
  {
    LoadedModules.remove(*this);
  }

  // ----------------------------------- STATIC METHODS -----------------------------------

  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  // ----------------------------------- MUTATOR METHODS ----------------------------------
}

// Debug
#include <wtl/windows/Window.hpp>
static wtl::MessageWindow<> wnd(0);      //!< Forced compilation of library

