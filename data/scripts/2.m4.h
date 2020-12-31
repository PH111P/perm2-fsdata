// Default pokemart
#include "scriptlib.m4.h"

MART_BEGIN( 0, 1 )
    BADGE_COUNT( 0 )
    CHECK_FLAG( 3, 0, 1 )
        MART_ITEM( I_POKE_BALL, 200 )
    CHECK_REGISTER_L( EVAL_REG, 3, 1 )
        MART_ITEM( I_GREAT_BALL, 600 )
    CHECK_REGISTER_L( EVAL_REG, 5, 1 )
        MART_ITEM( I_ULTRA_BALL, 1200 )
    MART_ITEM( I_POTION, 300 )
    CHECK_REGISTER_L( EVAL_REG, 1, 1 )
        MART_ITEM( I_SUPER_POTION, 700 )
    CHECK_REGISTER_L( EVAL_REG, 5, 1 )
        MART_ITEM( I_HYPER_POTION, 1200 )
    CHECK_REGISTER_L( EVAL_REG, 7, 1 )
        MART_ITEM( I_MAX_POTION, 2500 )
    CHECK_REGISTER_L( EVAL_REG, 8, 1 )
        MART_ITEM( I_FULL_RESTORE, 3000 )
    CHECK_REGISTER_L( EVAL_REG, 3, 1 )
        MART_ITEM( I_REVIVE, 1500 )
    MART_ITEM( I_ANTIDOTE, 100 )
    MART_ITEM( I_PARALYZE_HEAL, 200 )
    MART_ITEM( I_AWAKENING, 250 )
    CHECK_REGISTER_L( EVAL_REG, 1, 2 )
        MART_ITEM( I_BURN_HEAL, 250 )
        MART_ITEM( I_ICE_HEAL, 250 )
    CHECK_REGISTER_L( EVAL_REG, 5, 1 )
        MART_ITEM( I_FULL_HEAL, 600 )
    CHECK_FLAG( 3, 0, 2 )
        MART_ITEM( I_ESCAPE_ROPE, 550 )
        MART_ITEM( I_REPEL, 350 )
    CHECK_REGISTER_L( EVAL_REG, 3, 1 )
        MART_ITEM( I_SUPER_REPEL, 500 )
    CHECK_REGISTER_L( EVAL_REG, 7, 1 )
        MART_ITEM( I_MAX_REPEL, 600 )
SELL_ITEMS
END_OF_PROGRAM