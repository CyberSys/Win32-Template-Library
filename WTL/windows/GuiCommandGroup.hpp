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
  //! \interface IGuiCommandGroup - Interface for all gui commands
  //! 
  //! \tparam ENC - Command character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct IGuiCommandGroup
  {
    // ------------------- TYPES & CONSTANTS -------------------

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias command_t - Define command interface type
    using command_t = IGuiCommand<ENC>;

    //! \alias interface_t - Define own type
    using interface_t = IGuiCommandGroup<ENC>;

    //! \alias resource_t - Define resource ident type
    using resource_t = ResourceId<ENC>;

    //! \var encoding - Define encoding type
    static constexpr Encoding  encoding = ENC;
    
    // -------------------- REPRESENTATION ---------------------

    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // IGuiCommandGroup::~IGuiCommandGroup
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~IGuiCommandGroup() 
    {}
    
    // ---------------------- ACCESSORS ------------------------			
    
    ///////////////////////////////////////////////////////////////////////////////
    // IGuiCommandGroup::collection const
    //! Get all commands in the group
    //! 
    //! \return std::list<shared_command_t> - Collection of shared gui commands
    ///////////////////////////////////////////////////////////////////////////////
    std::list<shared_command_t<encoding>>  collection() const = 0;

    ///////////////////////////////////////////////////////////////////////////////
    // IGuiCommandGroup::description const
    //! Get the command group description
    //! 
    //! \return char_t* - Command group description
    ///////////////////////////////////////////////////////////////////////////////
    virtual const char_t*  description() const = 0;
    
    ///////////////////////////////////////////////////////////////////////////////
    // IGuiCommandGroup::find const
    //! Find a command within the group
    //! 
    //! \return command_t* - Command if found, otherwise nullptr
    ///////////////////////////////////////////////////////////////////////////////
    virtual command_t*  find(CommandId id) const = 0;

    ///////////////////////////////////////////////////////////////////////////////
    // IGuiCommandGroup::icon const
    //! Get the command group icon
    //! 
    //! \return HIcon - Shared icon handle
    ///////////////////////////////////////////////////////////////////////////////
    virtual HIcon  icon() const = 0;
    
    ///////////////////////////////////////////////////////////////////////////////
    // IGuiCommandGroup::ident const
    //! Get the command group identifier
    //! 
    //! \return CommandGroupId  - Command group identifier
    ///////////////////////////////////////////////////////////////////////////////
    virtual CommandGroupId  ident() const = 0;
    
    ///////////////////////////////////////////////////////////////////////////////
    // IGuiCommandGroup::name const
    //! Get the command group name
    //! 
    //! \return char_t* - Command group name
    ///////////////////////////////////////////////////////////////////////////////
    virtual const char_t*  name() const = 0;
    
    // ----------------------- MUTATORS ------------------------
  };

  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias shared_cmdgroup_t - Shared GuiCommand group pointer
  //! 
  //! \tparam ENC - Command character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using shared_cmdgroup_t = std::shared_ptr<IGuiCommandGroup<ENC>>;



  ///////////////////////////////////////////////////////////////////////////////
  //! \struct GuiCommandGroup - Provides a collection of Gui Commands, indexed by Command Id
  //! 
  //! \tparam ENC - Command character encoding
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct GuiCommandGroup : std::map<CommandId,shared_command_t<ENC>>, 
                           IGuiCommandGroup<ENC>
  {
    //! \alias base - Define base type
    using base = std::map<CommandId,shared_command_t<ENC>>;

    //! \alias command_t - Inherit command type
    using command_t = typename IGuiCommandGroup<ENC>::command_t;
    
    //! \alias name_t - Define name string resource type
    using name_t = StringResource<ENC,128>;

    //! \alias description_t - Define description string resource type
    using description_t = StringResource<ENC,1024>;
    
    //! \alias icon_t - Define icon resource type
    using icon_t = IconResource;

    //! \var encoding - Define window character encoding
    static constexpr Encoding encoding = ENC;

    // -------------------- REPRESENTATION ---------------------
  protected:
    CommandGroupId Ident;           //!< Command Id
    name_t         Name;            //!< Command Name
    description_t  Description;     //!< Command Description
    icon_t         Icon;            //!< Command Icon
    
    // --------------------- CONSTRUCTION ----------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommandGroup::GuiCommandGroup
    //! Create empty collection
    //! 
    //! \param[in] id - Group id   (Defining name, description, and icon resource)
    ///////////////////////////////////////////////////////////////////////////////
    GuiCommandGroup(CommandGroupId id) : Ident(id),
                                         Name(resource_id(id)),
                                         Description(resource_id(id)),
                                         Icon(resource_id(id)),
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommandGroup::GuiCommandGroup
    //! Create populated collection
    //! 
    //! \param[in] id - Group id   (Defining name, description, and icon resource)
    //! \param[in] cmds - Commands
    ///////////////////////////////////////////////////////////////////////////////
    GuiCommandGroup(CommandGroupId id, std::initializer_list<command_t*>&& cmds) : GuiCommandGroup(id)
    {
      // Populate
      for (command_t* c : cmds)
        *this += c;
    }
    
    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommandGroup::collection const
    //! Get all commands in the group
    //! 
    //! \return std::list<shared_command_t> - Collection of shared gui commands
    ///////////////////////////////////////////////////////////////////////////////
    std::list<shared_command_t<encoding>>  collection() const override
    {
      std::list<shared_command_t<encoding>> out;
      std::transform(begin(), end(), out.begin(), [](typename base::value_type& p) { return p.second; } );
      return out;
      /*for (auto& cmd : *this)
        out.push_back(cmd->second);*/
    }

    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommandGroup::description const
    //! Get the command description
    //! 
    //! \return char_t* - Command description
    ///////////////////////////////////////////////////////////////////////////////
    const char_t*  description() const override
    {
      return Description.Text;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommandGroup::find const
    //! Find a command within the group
    //! 
    //! \return command_t* - Command if found, otherwise nullptr
    ///////////////////////////////////////////////////////////////////////////////
    command_t*  find(CommandId id) const 
    {
      auto cmd = base::find(id);
      return cmd != base::end() ? cmd->second.get() : nullptr;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommandGroup::icon const
    //! Get the command icon
    //! 
    //! \return HIcon - Shared icon handle
    ///////////////////////////////////////////////////////////////////////////////
    HIcon  icon() const override
    {
      return Icon.Handle;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommandGroup::ident const
    //! Get the command identifier
    //! 
    //! \return CommandId - Command identifier
    ///////////////////////////////////////////////////////////////////////////////
    CommandId  ident() const override
    {
      return Ident;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommandGroup::name const
    //! Get the command name
    //! 
    //! \return char_t* - Command name
    ///////////////////////////////////////////////////////////////////////////////
    const char_t*  name() const override
    {
      return Name.Text;
    }
    
    // ----------------------- MUTATORS ------------------------

    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommandGroup::operator +=
    //! Add a command to the collection
    //!
    //! \param[in] *ptr - Gui command
    //! \return GuiCommandGroup& - Reference to self
    ///////////////////////////////////////////////////////////////////////////////
    GuiCommandGroup& operator += (command_t* ptr)
    {
      // Insert/overwrite
      this->emplace(ptr->ident(), std::shared_ptr<command_t>(ptr));
      return *this;
    }
  };
    

}

#endif // WTL_GUI_COMMAND_GROUP_HPP
