// hand over letter to steven
#include "scriptlib.m4.h"

MESSAGE( 422, 0 )
TAKE_ITEM( I_LETTER, 1 )
MESSAGE( 423, 0 )
GIVE_ITEM( I_TM30, 1 )
MESSAGE( 424, 0 )
MESSAGE( 425, 0 )
CHECK_PLAYER_POS( 16, 16, 3 )
CHECK_REGISTER( 0, 0, 2 )
    MOVE_PLAYER( RIGHT, 1 )
    MOVE_PLAYER( LEFT, 0 )
GET_MAPOBJECT( 16, 17, 3 )
    MOVE_MAPOBJECT_FAST_R( 0, UP, 4 )
SET_FLAG( 170, 0 )
SET_FLAG( 172, 1 )
REDRAW_OBJECTS
END_OF_PROGRAM
