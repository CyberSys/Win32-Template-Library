////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\commands\PasteClipboardCommand.hpp
//! \brief Encapsulates the 'paste' command
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_PASTE_CLIPBOARD_HPP
#define WTL_PASTE_CLIPBOARD_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct PasteClipboardCommand - Base for all Gui commands
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct PasteClipboardCommand : GuiCommand<ENC>
  {
    // ------------------- TYPES & CONSTANTS -------------------

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;

    //! \var encoding - Define encoding type
    static constexpr Encoding encoding = ENC;
    
    // --------------------- CONSTRUCTION ----------------------
  protected:
    ///////////////////////////////////////////////////////////////////////////////
    // PasteClipboardCommand::PasteClipboardCommand
    //! Derived c-tor
    ///////////////////////////////////////////////////////////////////////////////
    PasteClipboardCommand() : base(ID_FILE_EXIT)
    {}
    
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // PasteClipboardCommand::~PasteClipboardCommand
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~PasteClipboardCommand() 
    {}
    
    // ---------------------- ACCESSORS ------------------------			

    ///////////////////////////////////////////////////////////////////////////////
    // PasteClipboardCommand::canUndo const
    //! Query the whether the command can be undone
    //! 
    //! \return bool - True iff command can be undone
    ///////////////////////////////////////////////////////////////////////////////
    virtual bool canUndo() const
    {
      return true;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // PasteClipboardCommand::description const
    //! Get the command description
    //! 
    //! \return char_t* - Command description
    ///////////////////////////////////////////////////////////////////////////////
    virtual const char_t*  description() const
    {
      return nullptr;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // PasteClipboardCommand::icon const
    //! Get the command icon
    //! 
    //! \return HIcon - Shared icon handle
    ///////////////////////////////////////////////////////////////////////////////
    virtual HIcon icon() const
    {
      return true;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // PasteClipboardCommand::state const
    //! Query the current state of the command 
    //! 
    //! \return CommandState - Current state of command
    ///////////////////////////////////////////////////////////////////////////////
    virtual CommandState state() const
    {
      return CommandState::Enabled;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // PasteClipboardCommand::name const
    //! Get the command name
    //! 
    //! \return char_t* - Command name
    ///////////////////////////////////////////////////////////////////////////////
    virtual const char_t* name() const
    {
      return nullptr;
    }
    
    // ----------------------- MUTATORS ------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // PasteClipboardCommand::execute 
    //! Executes the command
    //! 
    //! \param[in] &&... args - [optional] Strongly typed variadic arguments
    ///////////////////////////////////////////////////////////////////////////////
    /*template <typename... ARGS>
    virtual void execute(ARGS&&...) */

    ///////////////////////////////////////////////////////////////////////////////
    // PasteClipboardCommand::execute 
    //! Executes the command
    //! 
    //! \throw logic_error - Command has not been implemented
    ///////////////////////////////////////////////////////////////////////////////
    virtual void execute() 
    {
      throw logic_error(HERE, "Command has not been implemented");
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // PasteClipboardCommand::undo
    //! Undo the command
    //! 
    //! \throw logic_error - Command has not been implemented
    ///////////////////////////////////////////////////////////////////////////////
    virtual void undo() 
    {
      throw logic_error(HERE, "Command has not been implemented");
    }

    ///////////////////////////////////////////////////////////////////////////////
    // PasteClipboardCommand::undo
    //! Undo the command
    //! 
    //! \param[in] &&... args - [optional] Strongly typed variadic arguments
    ///////////////////////////////////////////////////////////////////////////////
    /*template <typename... ARGS>
    virtual void undo(ARGS&&...) 
    {
    }*/
    
  };
  

}

#endif // WTL_PASTE_CLIPBOARD_HPP
