//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\CommandQueue.hpp
//! \brief Provides an undo/redo queue of all gui commands
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_GUI_COMMAND_QUEUE_HPP
#define WTL_GUI_COMMAND_QUEUE_HPP

#include "wtl/WTL.hpp"
#include "wtl/windows/Command.hpp"          //!< Command
#include "wtl/utils/Stack.hpp"             //!< Stack

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct CommandQueue - Enqueues executed Gui commands, providing an 'Undo' and 'Redo' functionality if supported by the command
  //! 
  //! \tparam ENC - Command character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct CommandQueue
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \var encoding - Define command character encoding 
    static constexpr Encoding encoding = ENC;

    //! \alias char_t - Define command character type
    using char_t = encoding_char_t<encoding>;

    //! \alias command_t - Define command base type
    using command_t = Command<encoding>;

  protected:
    //! \alias storage_t - Define storage type
    using storage_t = CommandPtr<encoding>;

    //! \alias collection_t - Define collection type
    using collection_t = Stack<storage_t>;
    
    // ------------------------------------- CONSTRUCTION -----------------------------------
  public:
    DEFAULT_CTOR(CommandQueue);           //!< Can be default-constructed
    
    // -------------------------------- COPYING & DESTRUCTION -------------------------------
  public:
    DISABLE_COPY(CommandQueue);          //!< Cannot be copied
    ENABLE_MOVE_CTOR(CommandQueue);      //!< Can be moved 
    ENABLE_POLY(CommandQueue);           //!< Can be polymorphic
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------		
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // CommandQueue::canRepeat const
    //! Query whether the last reverted command can be repeated
    //! 
    //! \return bool - True if 'Redo' is available
    /////////////////////////////////////////////////////////////////////////////////////////
    bool canRepeat() const
    {
      return !RepeatableCommands.empty();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // CommandQueue::canRevert const
    //! Query whether the last executed command can be reverted
    //! 
    //! \return bool - True if 'Undo' is available
    /////////////////////////////////////////////////////////////////////////////////////////
    bool canRevert() const
    {
      return !ExecutedCommands.empty();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // CommandQueue::peekRepeat const
    //! Peek the last reverted command (next repeatable command)
    //! 
    //! \return const command_t& - Immutable reference to next repeatable command 
    //! 
    //! \throw logic_error - No repeatable commands exist
    /////////////////////////////////////////////////////////////////////////////////////////
    const command_t&  peekRepeat() const
    {
      // Ensure non-empty
      if (RepeatableCommands.empty())
        throw logic_error(HERE, "No repeatable commands exist");

      // Peek the next repeatable command
      return *RepeatableCommands.peek();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CommandQueue::peekRevert const
    //! Peek the last executed command (next revertible command)
    //! 
    //! \return const command_t& - Immutable reference to next revertible command 
    //! 
    //! \throw logic_error - No revertible commands exist
    /////////////////////////////////////////////////////////////////////////////////////////
    const command_t&  peekRevert() const
    {
      // Ensure non-empty
      if (ExecutedCommands.empty())
        throw logic_error(HERE, "No revertible commands exist");

      // Peek the next repeatable command
      return *ExecutedCommands.peek();
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // CommandQueue::clear
    //! Clears all commands from the queue
    /////////////////////////////////////////////////////////////////////////////////////////
    void clear()
    {
      ExecutedCommands.clear();
      RepeatableCommands.clear();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // CommandQueue::execute
    //! Executes a command and saves it. Clears all repeatable commands.
    //! 
    //! \param[in] *cmd - Gui command
    //! 
    //! \throw wtl::invalid_argument - Missing command
    //! 
    //! \remarks If the command logic throws, the queue is unaffected
    /////////////////////////////////////////////////////////////////////////////////////////
    void execute(command_t* cmd)
    {
      REQUIRED_PARAM(cmd);

      // Execute
      cmd->execute();

      // [REVERTIBLE] Add to 'executed commands', and clear repeatable commands
      if (!cmd->permanent())
      {
        ExecutedCommands.push(cmd);
        RepeatableCommands.clear();
      }
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // CommandQueue::repeat
    //! Repeats the last command to be reverted
    //! 
    //! \throw wtl::logic_error - No commands have been reverted
    //! 
    //! \remarks If the command throws, the queue is unaffected
    /////////////////////////////////////////////////////////////////////////////////////////
    void repeat()
    {
      // Validate
      if (!canRepeat())
        throw logic_error(HERE, "No commands can be repeated");

      // Execute last-reverted command
      RepeatableCommands.peek()->execute();

      // Move from Redo -> Undo queue
      ExecutedCommands.push(RepeatableCommands.pop());
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CommandQueue::revert
    //! Reverts the previously executed command 
    //! 
    //! \throw wtl::logic_error - No reverted commands
    //! 
    //! \remarks If the command throws, the queue is unaffected
    /////////////////////////////////////////////////////////////////////////////////////////
    void revert()
    {
      // Validate
      if (!canRevert())
        throw logic_error(HERE, "No commands have been reverted");

      // Revert last-executed command
      ExecutedCommands.peek()->revert();

      // Move from Undo -> Redo queue
      RepeatableCommands.push(ExecutedCommands.pop());
    }

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    collection_t  ExecutedCommands,      //!< 'Undo' command queue
                  RepeatableCommands;    //!< 'Redo' command queue
                  
  };
        
} // namespace wtl

#endif // WTL_GUI_COMMAND_QUEUE_HPP
