//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\modules\Module.cpp
//! \brief Provides module implementation
//! \date 7 March 2015
//! \author Nick Crowley
//! \copyright © Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////

#include "Module.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
  // ----------------------------------- REPRESENTATION -----------------------------------
  
  //! \var LoadedModules - Loaded modules collection
  ModuleCollection  LoadedModules;
  
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
  //! Can be polymorphic. Removes module from 'Loaded Modules' collection.
  /////////////////////////////////////////////////////////////////////////////////////////
  Module::~Module()
  {
    LoadedModules.remove(*this);
  }

  // ----------------------------------- STATIC METHODS -----------------------------------

  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  // ----------------------------------- MUTATOR METHODS ----------------------------------
}
