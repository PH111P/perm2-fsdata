// Norman in his gym
#include "scriptlib.m4.h"

CHECK_FLAG( 55, 0, 2 )
    MESSAGE( 274, 0 )
    JUMP_TO_END

CHECK_FLAG( 26, 0, 2 )
    MESSAGE( 157, 0 )
    JUMP_TO_END

GET_MAPOBJECT( 17, 17, 3 ) // Norman
    COPY_REGISTER( 0, 1 )
    MESSAGE( 139, 0 )
    SPAWN_MAPOBJECT( 22, 17, 21 ) // Wally
    MOVE_MAPOBJECT_R( 0, UP, 2 )
    MOVE_MAPOBJECT_R( 0, RIGHT, 1 )
    MOVE_MAPOBJECT_R( 0, UP, 2 )
    MOVE_MAPOBJECT_R( 0, LEFT, 0 )
    MESSAGE( 140, 0 )
    MOVE_MAPOBJECT_R( 1, RIGHT, 0 )
    MESSAGE( 141, 0 )
    MESSAGE( 142, 0 )
    MESSAGE( 143, 0 )
    MOVE_MAPOBJECT_R( 1, DOWN, 0 )
    MESSAGE( 144, 0 )
    MOVE_MAPOBJECT_R( 1, RIGHT, 0 )
    MESSAGE( 145, 0 )
    PLAY_SOUND_EFFECT( SFX_OBTAIN_ITEM )
    MESSAGE( 146, 1 )
    MESSAGE( 147, 0 )
    PLAY_SOUND_EFFECT( SFX_OBTAIN_ITEM )
    MESSAGE( 148, 1 )
    MESSAGE( 149, 0 )
    MOVE_MAPOBJECT_R( 0, DOWN, 1 )
    MOVE_MAPOBJECT_R( 0, LEFT, 0 )
    MOVE_PLAYER( RIGHT, 0 )
    MESSAGE( 150, 0 )
    MOVE_MAPOBJECT_R( 0, DOWN, 3 )
    DESTROY_MAPOBJECT_R( 0 )
    SET_FLAG( 24, 1 )
    MOVE_PLAYER( DOWN, 3 )
// Warp outside
GET_MAPOBJECT( 14, 8, 3 ) // Wally
    LOCK_R( 0 )
    ATTACH_PLAYER
    MOVE_MAPOBJECT_R( 0, DOWN, 7 )
    MOVE_MAPOBJECT_R( 0, RIGHT, 20 )
    MOVE_MAPOBJECT_R( 0, LEFT, 0 )
    REMOVE_PLAYER
    MESSAGE( 151, 0 )
    SET_FLAG( 25, 1 )
    DESTROY_MAPOBJECT_R( 0 )
REDRAW_OBJECTS
    GET_MAPOBJECT( 2, 15, 3 ) // Wally
    LOCK_R( 0 )
    // Catching tutorial
    CATCHING_TUTORIAL
    MESSAGE( 152, 0 )
    MOVE_MAPOBJECT_R( 0, LEFT, 0 )
    MESSAGE( 153, 0 )
WARP_CROSSBANK( 28, 0x31, 0x92, 3 )

GET_MAPOBJECT( 17, 17, 3 ) // Norman
    COPY_REGISTER( 0, 1 )
    GET_MAPOBJECT( 18, 18, 3 ) // Wally
    LOCK_R( 0 )
    MOVE_PLAYER( UP, 0 )
    MOVE_MAPOBJECT_R( 0, UP, 0 )
    MESSAGE( 154, 0 )

    CHECK_FLAG( 27, 1, 1 )
    // Wally caught a non-shiny Ralts
        MESSAGE( 155, 0 )
    CHECK_FLAG( 27, 0, 4 )
        MESSAGE( 164, 0 )
        EXCLAMATION_R( 1 )
        MESSAGE( 165, 0 )
        MESSAGE( 166, 0 )

    MOVE_MAPOBJECT_R( 0, DOWN, 3 )
    DESTROY_MAPOBJECT_R( 0 )
    MESSAGE( 156, 0 )
    SET_FLAG( 26, 1 )
END_OF_PROGRAM
