////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Resources.h
//! \brief Resource header defining standard window command Ids
//! \date 6 March 2015
//! \author Nick Crowley
//! Copyright © Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RESOURCE_HEADER_HPP 
#define WTL_RESOURCE_HEADER_HPP

///////////////////////////////////////////////////////////////////////////////
// Standard Commands

// File commands
#define ID_FILE_NEW                     0xE100
#define ID_FILE_OPEN                    0xE101
#define ID_FILE_CLOSE                   0xE102
#define ID_FILE_SAVE                    0xE103
#define ID_FILE_SAVE_AS                 0xE104
#define ID_FILE_PAGE_SETUP              0xE105
#define ID_FILE_PRINT_SETUP             0xE106
#define ID_FILE_PRINT                   0xE107
#define ID_FILE_PRINT_DIRECT            0xE108
#define ID_FILE_PRINT_PREVIEW           0xE109
#define ID_FILE_UPDATE                  0xE10A
#define ID_FILE_SAVE_COPY_AS            0xE10B
#define ID_FILE_SEND_MAIL               0xE10C

#define ID_FILE_MRU_FIRST               0xE110
#define ID_FILE_MRU_FILE1               0xE110          // range - 16 max
#define ID_FILE_MRU_FILE2               0xE111
#define ID_FILE_MRU_FILE3               0xE112
#define ID_FILE_MRU_FILE4               0xE113
#define ID_FILE_MRU_FILE5               0xE114
#define ID_FILE_MRU_FILE6               0xE115
#define ID_FILE_MRU_FILE7               0xE116
#define ID_FILE_MRU_FILE8               0xE117
#define ID_FILE_MRU_FILE9               0xE118
#define ID_FILE_MRU_FILE10              0xE119
#define ID_FILE_MRU_FILE11              0xE11A
#define ID_FILE_MRU_FILE12              0xE11B
#define ID_FILE_MRU_FILE13              0xE11C
#define ID_FILE_MRU_FILE14              0xE11D
#define ID_FILE_MRU_FILE15              0xE11E
#define ID_FILE_MRU_FILE16              0xE11F
#define ID_FILE_MRU_LAST                0xE11F

// Edit commands
#define ID_EDIT_CLEAR                   0xE120
#define ID_EDIT_CLEAR_ALL               0xE121
#define ID_EDIT_COPY                    0xE122
#define ID_EDIT_CUT                     0xE123
#define ID_EDIT_FIND                    0xE124
#define ID_EDIT_PASTE                   0xE125
#define ID_EDIT_PASTE_LINK              0xE126
#define ID_EDIT_PASTE_SPECIAL           0xE127
#define ID_EDIT_REPEAT                  0xE128
#define ID_EDIT_REPLACE                 0xE129
#define ID_EDIT_SELECT_ALL              0xE12A
#define ID_EDIT_UNDO                    0xE12B
#define ID_EDIT_REDO                    0xE12C
#define ID_EDIT_DELETE                  ID_EDIT_CLEAR
#define ID_EDIT_FIND_NEXT               ID_EDIT_REPEAT
#define ID_EDIT_FIND_PREVIOUS           0xE12D

// Window commands
#define ID_WINDOW_NEW                   0xE130
#define ID_WINDOW_ARRANGE               0xE131
#define ID_WINDOW_CASCADE               0xE132
#define ID_WINDOW_TILE_HORZ             0xE133
#define ID_WINDOW_TILE_VERT             0xE134
#define ID_WINDOW_SPLIT                 0xE135

// TabView
#define ID_WINDOW_TABFIRST              0xFF00	// = ATL_IDM_FIRST_MDICHILD
#define ID_WINDOW_TABLAST               0xFFFD
#define ID_WINDOW_SHOWTABLIST           0xFFFE

// Help and App commands
#define ID_APP_ABOUT                    0xE140
#define ID_APP_EXIT                     0xE141
#define ID_HELP_INDEX                   0xE142
#define ID_HELP_FINDER                  0xE143
#define ID_HELP_USING                   0xE144
#define ID_CONTEXT_HELP                 0xE145      // shift-F1
// special commands for processing help
#define ID_HELP                         0xE146      // first attempt for F1
#define ID_DEFAULT_HELP                 0xE147      // last attempt

// Misc
#define ID_NEXT_PANE                    0xE150
#define ID_PREV_PANE                    0xE151
#define ID_PANE_CLOSE                   0xE152
#define ID_PANE_NEXT                    ID_NEXT_PANE
#define ID_PANE_PREVIOUS                ID_PREV_PANE

// Format
#define ID_FORMAT_FONT                  0xE160

// Scroll
#define ID_SCROLL_UP                    0xE170
#define ID_SCROLL_DOWN                  0xE171
#define ID_SCROLL_PAGE_UP               0xE172
#define ID_SCROLL_PAGE_DOWN             0xE173
#define ID_SCROLL_TOP                   0xE174
#define ID_SCROLL_BOTTOM                0xE175
#define ID_SCROLL_LEFT                  0xE176
#define ID_SCROLL_RIGHT                 0xE177
#define ID_SCROLL_PAGE_LEFT             0xE178
#define ID_SCROLL_PAGE_RIGHT            0xE179
#define ID_SCROLL_ALL_LEFT              0xE17A
#define ID_SCROLL_ALL_RIGHT             0xE17B

// OLE commands
#define ID_OLE_INSERT_NEW               0xE200
#define ID_OLE_EDIT_LINKS               0xE201
#define ID_OLE_EDIT_CONVERT             0xE202
#define ID_OLE_EDIT_CHANGE_ICON         0xE203
#define ID_OLE_EDIT_PROPERTIES          0xE204
#define ID_OLE_VERB_FIRST               0xE210     // range - 16 max
#ifndef RC_INVOKED      // code only
  #define ID_OLE_VERB_LAST              0xE21F
#endif // !RC_INVOKED

// Command groups
#define ID_FILE_GROUP                   0xE230
#define ID_EDIT_GROUP                   0xE231
#define ID_VIEW_GROUP                   0xE232
#define ID_WINDOW_GROUP                 0xE233
#define ID_HELP_GROUP                   0xE234

// View commands (same number used as IDW used for toolbar and status bar)
#define ID_VIEW_TOOLBAR                 0xE800
#define ID_VIEW_STATUS_BAR              0xE801
#define ID_VIEW_REFRESH                 0xE803
#define ID_VIEW_RIBBON                  0xE804

///////////////////////////////////////////////////////////////////////////////
// Misc. control IDs

// Property Sheet control id's (determined with Spy++)
#define ID_APPLY_NOW                    0x3021
#define ID_WIZBACK                      0x3023
#define ID_WIZNEXT                      0x3024
#define ID_WIZFINISH                    0x3025
#define ATL_IDC_TAB_CONTROL             0x3020


#endif // WTL_RESOURCE_HEADER_HPP
