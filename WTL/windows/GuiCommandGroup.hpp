////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\GuiCommandGroup.hpp
//! \brief Groups gui commands for use in menus and toolbars 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_GUI_COMMAND_GROUP_HPP
#define WTL_GUI_COMMAND_GROUP_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct GuiCommandGroup - Groups gui commands for use in menus and toolbars 
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct GuiCommandGroup
  {
    // ------------------- TYPES & CONSTANTS -------------------

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;

    //! \var encoding - Define encoding type
    static constexpr Encoding encoding = ENC;
    
    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommandGroup::~GuiCommandGroup
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~GuiCommandGroup() 
    {}
    
    // ---------------------- ACCESSORS ------------------------			

    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommandGroup::canUndo const
    //! Query the whether the command can be undone
    //! 
    //! \return bool - True iff command can be undone
    ///////////////////////////////////////////////////////////////////////////////
    virtual bool canUndo() const
    {
      return true;
    }
    
    // ----------------------- MUTATORS ------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommandGroup::operator +=
    //! Add a command to the group
    //! 
    //! \param[in] const& cmd - Command 
    //! \return GuiCommandGroup& - Reference to self
    ///////////////////////////////////////////////////////////////////////////////
    /*GuiCommandGroup& operator += (const command_t<ENC>& cmd)
    {
      return *this;
    }*/
    
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommandGroup::operator -=
    //! Remove a command from the group
    //! 
    //! \param[in] const& cmd - Command 
    //! \return GuiCommandGroup& - Reference to self
    ///////////////////////////////////////////////////////////////////////////////
    /*GuiCommandGroup& operator -= (const GuiCommand<ENC>& cmd)
    {
      return *this;
    }*/
    
    // ----------------------- REPRESENTATION ------------------------
  };



  

}

#endif // WTL_GUI_COMMAND_GROUP_HPP
