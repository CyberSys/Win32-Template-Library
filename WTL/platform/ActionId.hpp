//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\ActionId.hpp
//! \brief Defines WTL Command Ids
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_COMMAND_IDS_HPP
#define WTL_COMMAND_IDS_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EnumTraits.hpp"
#include "wtl/casts/EnumCast.hpp"
#include "wtl/Resource.h"
#include <type_traits>

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \enum ActionId - Defines GUI Command Ids
  /////////////////////////////////////////////////////////////////////////////////////////
  enum class ActionId : uint16_t
  {
    File_New = ID_FILE_NEW,			                    //!< 0xE100
    File_Open = ID_FILE_OPEN,			                  //!< 0xE101
    File_Close = ID_FILE_CLOSE,			                //!< 0xE102
    File_Save = ID_FILE_SAVE,			                  //!< 0xE103
    File_Save_As = ID_FILE_SAVE_AS,			            //!< 0xE104
    File_Page_Setup = ID_FILE_PAGE_SETUP,		        //!< 0xE105
    File_Print_Setup = ID_FILE_PRINT_SETUP,	        //!< 0xE106
    File_Print = ID_FILE_PRINT,			                //!< 0xE107
    File_Print_Direct = ID_FILE_PRINT_DIRECT,			  //!< 0xE108
    File_Print_Preview = ID_FILE_PRINT_PREVIEW,			//!< 0xE109
    File_Update = ID_FILE_UPDATE,			              //!< 0xE10A
    File_Save_Copy_As = ID_FILE_SAVE_COPY_AS,			  //!< 0xE10B
    File_Send_Mail = ID_FILE_SEND_MAIL,			        //!< 0xE10C

    File_MRU_First = ID_FILE_MRU_FIRST,			        //!< 0xE110
    File_MRU_File1 = ID_FILE_MRU_FILE1,			        //!< 0xE110          // range - 16 max
    File_MRU_File2 = ID_FILE_MRU_FILE2,			        //!< 0xE111
    File_MRU_File3 = ID_FILE_MRU_FILE3,			        //!< 0xE112
    File_MRU_File4 = ID_FILE_MRU_FILE4,			        //!< 0xE113
    File_MRU_File5 = ID_FILE_MRU_FILE5,			        //!< 0xE114
    File_MRU_File6 = ID_FILE_MRU_FILE6,			        //!< 0xE115
    File_MRU_File7 = ID_FILE_MRU_FILE7,			        //!< 0xE116
    File_MRU_File8 = ID_FILE_MRU_FILE8,			        //!< 0xE117
    File_MRU_File9 = ID_FILE_MRU_FILE9,			        //!< 0xE118
    File_MRU_File10 = ID_FILE_MRU_FILE10,			      //!< 0xE119
    File_MRU_File11 = ID_FILE_MRU_FILE11,			      //!< 0xE11A
    File_MRU_File12 = ID_FILE_MRU_FILE12,			      //!< 0xE11B
    File_MRU_File13 = ID_FILE_MRU_FILE13,			      //!< 0xE11C
    File_MRU_File14 = ID_FILE_MRU_FILE14,			      //!< 0xE11D
    File_MRU_File15 = ID_FILE_MRU_FILE15,			      //!< 0xE11E
    File_MRU_File16 = ID_FILE_MRU_FILE16,			      //!< 0xE11F
    File_MRU_Last = ID_FILE_MRU_LAST,			          //!< 0xE11F

    // edit commands
    Edit_Clear = ID_EDIT_CLEAR,			                //!< 0xE120
    Edit_Clear_All = ID_EDIT_CLEAR_ALL,			        //!< 0xE121
    Edit_Copy = ID_EDIT_COPY,			                  //!< 0xE122
    Edit_Cut = ID_EDIT_CUT,			                    //!< 0xE123
    Edit_Find = ID_EDIT_FIND,			                  //!< 0xE124
    Edit_Paste = ID_EDIT_PASTE,			                //!< 0xE125
    Edit_Paste_Link = ID_EDIT_PASTE_LINK,			      //!< 0xE126
    Edit_Paste_Special = ID_EDIT_PASTE_SPECIAL,			//!< 0xE127
    Edit_Repeat = ID_EDIT_REPEAT,			              //!< 0xE128
    Edit_Replace = ID_EDIT_REPLACE,			            //!< 0xE129
    Edit_Select_All = ID_EDIT_SELECT_ALL,			      //!< 0xE12A
    Edit_Undo = ID_EDIT_UNDO,			                  //!< 0xE12B
    Edit_Redo = ID_EDIT_REDO,			                  //!< 0xE12C
    Edit_Delete = ID_EDIT_DELETE,			              //!< ID_EDIT_CLEAR
    Edit_Find_Next = ID_EDIT_FIND_NEXT,			        //!< ID_EDIT_REPEAT
    Edit_Find_Previous = ID_EDIT_FIND_PREVIOUS,			//!< 0xE12D

    // Window commands
    Window_New = ID_WINDOW_NEW,			                //!< 0xE130
    Window_Arrange = ID_WINDOW_ARRANGE,			        //!< 0xE131
    Window_Cascade = ID_WINDOW_CASCADE,			        //!< 0xE132
    Window_Tile_Horz = ID_WINDOW_TILE_HORZ,			    //!< 0xE133
    Window_Tile_Vert = ID_WINDOW_TILE_VERT,			    //!< 0xE134
    Window_Split = ID_WINDOW_SPLIT,			            //!< 0xE135

    // TabView
    Window_Tab_First = ID_WINDOW_TABFIRST,			    //!< 0xFF00	// = ATL_IDM_FIRST_MDICHILD
    Window_Tab_Last = ID_WINDOW_TABLAST,			      //!< 0xFFFD
    Window_Show_Tab_List = ID_WINDOW_SHOWTABLIST,	  //!< 0xFFFE

    // Help and App commands
    App_About = ID_APP_ABOUT,			                  //!< About program
    App_Exit = ID_APP_EXIT,			                    //!< Exit program
    Help_Index = ID_HELP_INDEX,			                //!< 0xE142
    Help_Finder = ID_HELP_FINDER,			              //!< 0xE143
    Help_Using = ID_HELP_USING,			                //!< 0xE144
    Context_Help = ID_CONTEXT_HELP,			            //!< Shift-F1
    // special commands for processing help
    Help = ID_HELP,			                            //!< first attempt for F1
    Default_Help = ID_DEFAULT_HELP,			            //!< last attempt

    // Misc
    Next_Pane = ID_NEXT_PANE,			                  //!< 0xE150
    Prev_Pane = ID_PREV_PANE,			                  //!< 0xE151
    Pane_Close = ID_PANE_CLOSE,			                //!< 0xE152
    Pane_Next = ID_PANE_NEXT,			                  //!< ID_NEXT_PANE
    Pane_Previous = ID_PANE_PREVIOUS,			          //!< ID_PREV_PANE
  };
  
  
  //! Define traits: Non-contiguous Enumeration
  template <> struct is_attribute<ActionId>  : std::false_type  {};
  template <> struct is_contiguous<ActionId> : std::false_type  {};

  //! Define limits traits
  /*template <> struct max_value<ActionId>     : std::integral_constant<ActionId,ActionId::Invalid>   {};
  template <> struct min_value<ActionId>     : std::integral_constant<ActionId,ActionId::Black>     {};*/
  

  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::action_id
  //! Creates a strongly typed command id from any integral or enumeration type
  //!
  //! \tparam TYPE - Integral or enumeration type
  //! 
  //! \param[in] id - Value representing command id
  //! \return ActionId - ActionId representation of 'id'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename VALUE, typename = std::enable_if_t<std::is_integral<VALUE>::value || std::is_enum<VALUE>::value>>
  ActionId  action_id(VALUE id)
  {
    // Convert into underlying type then cast to enumeration
    return enum_cast<ActionId>( static_cast<std::underlying_type_t<ActionId>>(id) );
  }


  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \enum ActionGroupId - Defines GUI Command Ids
  /////////////////////////////////////////////////////////////////////////////////////////
  enum class ActionGroupId : uint16_t
  {
    File = ID_FILE_GROUP,           //!< File commands group
    Edit = ID_EDIT_GROUP,           //!< Edit commands group
    View = ID_VIEW_GROUP,           //!< View commands group
    Window = ID_WINDOW_GROUP,       //!< Window commands group
    Help = ID_HELP_GROUP,           //!< Help commands group
  };
  
  
  //! Define traits: Contiguous Enumeration
  template <> struct is_attribute<ActionGroupId>  : std::false_type  {};
  template <> struct is_contiguous<ActionGroupId> : std::true_type   {};

  //! Define limits traits
  template <> struct max_value<ActionGroupId>     : std::integral_constant<ActionGroupId,ActionGroupId::Help>   {};
  template <> struct min_value<ActionGroupId>     : std::integral_constant<ActionGroupId,ActionGroupId::File>   {};
  

  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::action_group_id
  //! Creates a strongly typed command group id from any integral or enumeration type
  //!
  //! \tparam TYPE - Integral or enumeration type
  //! 
  //! \param[in] id - Value representing command group id
  //! \return ActionGroupId - ActionGroupId representation of 'id'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename VALUE, typename = std::enable_if_t<std::is_integral<VALUE>::value || std::is_enum<VALUE>::value>>
  ActionGroupId  action_group_id(VALUE id)
  {
    // Convert into underlying type then cast to enumeration
    return enum_cast<ActionGroupId>( static_cast<std::underlying_type_t<ActionGroupId>>(id) );
  }
}

#endif  // WTL_COMMAND_IDS_HPP
