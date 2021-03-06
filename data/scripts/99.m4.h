// mr brine
#include "scriptlib.m4.h"

// check if mission in dewford is completed
SET_REGISTER( 4, 0 )
CHECK_FLAG( 166, 0, 3 )
    CHECK_FLAG( 150, 0, 2 )
        SET_REGISTER( 4, 1 )
        SKIP( 2 )
    CHECK_FLAG( 172, 0, 1 )
        SET_REGISTER( 4, 1 )

CHECK_FLAG( 167, 1, 9 )
    MESSAGE( 412, 0 )

    CHECK_FLAG( 161, 1, 1 )
        JUMP_TO_END

    SET_FLAG( 167, 1 )

    CHECK_FLAG( 166, 1, 2 )
        YES_NO_MESSAGE_D( 413, 0 )
        SKIP( 1 )
        YES_NO_MESSAGE_D( 426, 0 )
    SKIP( 10 )

CHECK_REGISTER( 4, 0, 5 )
    CHECK_FLAG( 166, 1, 2 )
        YES_NO_MESSAGE_D( 438, 0 )
        SKIP( 1 )
        YES_NO_MESSAGE_D( 440, 0 )
    SKIP( 4 )
CHECK_FLAG( 166, 1, 2 )
    YES_NO_MESSAGE_D( 416, 0 )
    SKIP( 1 )
    YES_NO_MESSAGE_D( 427, 0 )

CHECK_REGISTER( EVAL_REG, 1, 2 )
    // player doesn't want to sail
    MESSAGE( 415, 0 )
    JUMP_TO_END
// player wants to sail to dewford
MESSAGE( 414, 0 )
// warp outside
WARP_CROSSBANK( 10, 0x49, 0x11f, 3 )
MOVE_PLAYER( LEFT, 1 )
MOVE_PLAYER( DOWN, 0 )
SPAWN_MAPOBJECT( 62, 9, 31 )
    COPY_REGISTER( 0, 1 )
    MOVE_MAPOBJECT_R( 1, DOWN, 1 )
    MOVE_MAPOBJECT_R( 1, LEFT, 1 )
    ATTACH_PLAYER
    MOVE_MAPOBJECT_R( 1, LEFT, 4 )
    MOVE_MAPOBJECT_R( 1, DOWN, 1 )
    REMOVE_PLAYER
DESTROY_MAPOBJECT_R( 1 )

WALK_PLAYER( DOWN, 1 )
HIDE_PLAYER
WALK_PLAYER( DOWN, 1 )
// start boat trip
SET_FLAG( 168, 1 )
SET_FLAG( 169, 1 )
PLAY_MUSIC( MOD_SURFING )
GET_MAPOBJECT( 4, 2, 5 )
    COPY_REGISTER( 0, 1 )
    FIX_MAPOBJECT_R( 1 )
    ATTACH_PLAYER
    MOVE_MAPOBJECT_R( 1, DOWN, 5 )
    MOVE_MAPOBJECT_FAST_R( 1, DOWN, 17 )
    MOVE_MAPOBJECT_R( 1, DOWN, 5 )
    MOVE_MAPOBJECT_R( 1, RIGHT, 10 )
    MOVE_MAPOBJECT_R( 1, DOWN, 5 )
    MOVE_MAPOBJECT_FAST_R( 1, DOWN, 78 )
    MOVE_MAPOBJECT_R( 1, DOWN, 5 )
    MOVE_MAPOBJECT_R( 1, RIGHT, 5 )
    MOVE_MAPOBJECT_FAST_R( 1, RIGHT, 40 )
    MOVE_MAPOBJECT_R( 1, RIGHT, 5 )
    MOVE_MAPOBJECT_R( 1, DOWN, 5 )
    MOVE_MAPOBJECT_FAST_R( 1, DOWN, 9 )
    MOVE_MAPOBJECT_R( 1, DOWN, 5 )
    REMOVE_PLAYER
RESTORE_MUSIC

SET_FLAG( 168, 0 )
SET_FLAG( 169, 0 )
SPAWN_MAPOBJECT( 62, 0, 8 )
    COPY_REGISTER( 0, 2 )
    MOVE_MAPOBJECT_R( 2, DOWN, 1 )
    MOVE_MAPOBJECT_R( 2, RIGHT, 1 )
    MOVE_MAPOBJECT_R( 2, LEFT, 0 )
    FIX_MAPOBJECT_R( 2 )
UNFIX_MAPOBJECT_R( 1 )
UNFIX_MAPOBJECT_R( 2 )
SHOW_PLAYER
WALK_PLAYER( DOWN, 1 )
MOVE_PLAYER( RIGHT, 0 )

// post-landing message
CHECK_REGISTER( 4, 0, 2 )
    MESSAGE( 432, 0 )
    SKIP( 4 )
CHECK_FLAG( 166, 1, 2 )
    MESSAGE( 417, 0 )
    SKIP( 1 )
    MESSAGE( 428, 0 )

DESTROY_MAPOBJECT_R( 1 )
DESTROY_MAPOBJECT_R( 2 )
REDRAW_OBJECTS