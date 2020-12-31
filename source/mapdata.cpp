#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

#include "fsdata.h"

string FILENAME;

enum moveMode {
    // Player modes
    WALK       = 0,
    SURF       = ( 1 << 0 ),
    BIKE       = ( 1 << 2 ),
    ACRO_BIKE  = ( 1 << 2 ) | ( 1 << 0 ),
    MACH_BIKE  = ( 1 << 2 ) | ( 1 << 1 ),
    BIKE_JUMP  = ( 1 << 2 ) | ( 1 << 0 ) | ( 1 << 1 ),
    SIT        = ( 1 << 3 ),
    DIVE       = ( 1 << 4 ),
    ROCK_CLIMB = ( 1 << 5 ),
    // NPC modes
    NO_MOVEMENT = 0,
};

enum eventType : u8 {
    EVENT_NONE        = 0,
    EVENT_MESSAGE     = 1,
    EVENT_ITEM        = 2,
    EVENT_TRAINER     = 3,
    EVENT_OW_PKMN     = 4,
    EVENT_NPC         = 5,
    EVENT_WARP        = 6,
    EVENT_GENERIC     = 7,
    EVENT_HMOBJECT    = 8,
    EVENT_BERRYTREE   = 9,
    EVENT_NPC_MESSAGE = 10,
};
enum eventTrigger : u8 {
    TRIGGER_NONE           = 0,
    TRIGGER_STEP_ON        = ( 1 << 0 ),
    TRIGGER_INTERACT       = ( 1 << 1 ) | ( 1 << 2 ) | ( 1 << 3 ) | ( 1 << 4 ),
    TRIGGER_INTERACT_DOWN  = ( 1 << 1 ),
    TRIGGER_INTERACT_LEFT  = ( 1 << 2 ),
    TRIGGER_INTERACT_UP    = ( 1 << 3 ),
    TRIGGER_INTERACT_RIGHT = ( 1 << 4 ),
    TRIGGER_ON_MAP_ENTER   = ( 1 << 5 ),
};
enum wildPkmnType : u8 {
    GRASS,
    HIGH_GRASS,
    WATER,
    OLD_ROD,
    GOOD_ROD,
    SUPER_ROD,
    HEADBUTT,
    ROCK_SMASH,
    SWEET_SCENT,
};
enum mapWeather : u8 {
    NOTHING        = 0, // Inside
    SUNNY          = 1,
    REGULAR        = 2,
    RAINY          = 3,
    SNOW           = 4,
    THUNDERSTORM   = 5,
    MIST           = 6,
    BLIZZARD       = 7,
    SANDSTORM      = 8,
    FOG            = 9,
    DENSE_MIST     = 0xa,
    CLOUDY         = 0xb, // Dark Forest clouds
    HEAVY_SUNLIGHT = 0xc,
    HEAVY_RAIN     = 0xd,
    UNDERWATER     = 0xe
};
enum mapType : u8 { OUTSIDE = 0, CAVE = 1, INSIDE = 2, DARK = 4, FLASHABLE = 8 };
enum warpType : u8 {
    NO_SPECIAL,
    CAVE_ENTRY,
    DOOR,
    TELEPORT,
    EMERGE_WATER,
    LAST_VISITED,
    SLIDING_DOOR
};

struct mapData {
    mapType    m_mapType;
    mapWeather m_weather;
    u8         m_battleBG;
    u8         m_battlePlat1;
    u8         m_battlePlat2;
    u8         m_surfBattleBG;
    u8         m_surfBattlePlat1;
    u8         m_surfBattlePlat2;

    u16 m_baseLocationId;
    u8  m_extraLocationCount;
    struct locationData {
        u8  m_left;
        u8  m_top;
        u8  m_right;
        u8  m_bottom;
        u16 m_locationId;
    } m_extraLocations[ 4 ];
    u8 m_pokemonDescrCount;
    struct wildPkmnData {
        u16          m_speciesId;
        u8           m_forme;
        wildPkmnType m_encounterType;
        u8           m_slot;
        u8           m_daytime;
        u8           m_encounterRate;
    } m_pokemon[ 30 ];
    u8 m_eventCount;
    struct event {
        u8           m_posX;
        u8           m_posY;
        u8           m_posZ;
        u16          m_activateFlag;
        u16          m_deactivateFlag;
        eventType    m_type;
        eventTrigger m_trigger;
        union data {
            struct {
                u8  m_msgType;
                u16 m_msgId;
            } m_message;
            struct {
                u8  m_itemType;
                u16 m_itemId;
            } m_item;
            struct {
                u8  m_movementType;
                u16 m_spriteId;
                u16 m_trainerId;
                u8  m_sight;
            } m_trainer;
            struct {
                u16 m_speciesId;
                u8  m_level;
                u8  m_forme; // BIT(6) female; BIT(7) genderless
                u8  m_shiny; // BIT(6) hidden ability, BIT(7) fateful
            } m_owPkmn;
            struct {
                u8  m_movementType;
                u16 m_spriteId;
                u16 m_scriptId; // message id for npc_message
                u8  m_scriptType;
            } m_npc;
            struct {
                warpType m_warpType;
                u8       m_bank;
                u8       m_mapX;
                u8       m_mapY;
                u8       m_posX;
                u8       m_posY;
                u8       m_posZ;
            } m_warp;
            struct {
                u16 m_scriptId;
                u8  m_scriptType;
            } m_generic;
            struct {
                u8 m_hmType;
            } m_hmObject;
            struct {
                u8 m_treeIdx; // internal id of this berry tree
            } m_berryTree;
        } m_data;
    } m_events[ 64 ];
};

map<u16, names>  location_names;
map<string, int> locations;

map<string, int> items;
vector<names>    item_names;

map<string, int>      pkmns;
vector<names>         pkmn_names;
vector<vector<names>> forme_names;

mapWeather getMapWeather( char* p_str ) {
    if( !strcmp( p_str, "none" ) ) return REGULAR;

    if( !strcmp( p_str, "sunny" ) ) return SUNNY;
    if( !strcmp( p_str, "inside" ) ) return NOTHING;
    if( !strcmp( p_str, "rainy" ) ) return RAINY;
    if( !strcmp( p_str, "snow" ) ) return SNOW;
    if( !strcmp( p_str, "thunderstorm" ) ) return THUNDERSTORM;
    if( !strcmp( p_str, "mist" ) ) return MIST;
    if( !strcmp( p_str, "blizzard" ) ) return BLIZZARD;
    if( !strcmp( p_str, "sandstorm" ) ) return SANDSTORM;
    if( !strcmp( p_str, "fog" ) ) return FOG;
    if( !strcmp( p_str, "dense mist" ) ) return DENSE_MIST;
    if( !strcmp( p_str, "cloudy" ) ) return CLOUDY;
    if( !strcmp( p_str, "heavy sunshine" ) ) return HEAVY_SUNLIGHT;
    if( !strcmp( p_str, "heavy rain" ) ) return HEAVY_RAIN;
    if( !strcmp( p_str, "underwater" ) ) return UNDERWATER;

    fprintf( stderr, "unknown weather %s\n", p_str );
    return NOTHING;
}

wildPkmnType getEncounterType( char* p_str ) {
    if( !strcmp( p_str, "none" ) ) return GRASS;

    if( !strcmp( p_str, "grass" ) ) return GRASS;
    if( !strcmp( p_str, "high grass" ) ) return HIGH_GRASS;
    if( !strcmp( p_str, "surf" ) ) return WATER;
    if( !strcmp( p_str, "water" ) ) return WATER;
    if( !strcmp( p_str, "old rod" ) ) return OLD_ROD;
    if( !strcmp( p_str, "good rod" ) ) return GOOD_ROD;
    if( !strcmp( p_str, "super rod" ) ) return SUPER_ROD;
    if( !strcmp( p_str, "headbutt" ) ) return HEADBUTT;
    if( !strcmp( p_str, "rock smash" ) ) return ROCK_SMASH;
    if( !strcmp( p_str, "sweet scent" ) ) return SWEET_SCENT;

    fprintf( stderr, "unknown wild pkmn encounter type %s\n", p_str );
    return GRASS;
}

mapType getMapType( char* p_str ) {
    if( !strcmp( p_str, "none" ) ) return OUTSIDE;

    if( !strcmp( p_str, "outside" ) ) return OUTSIDE;
    if( !strcmp( p_str, "cave" ) ) return CAVE;
    if( !strcmp( p_str, "inside" ) ) return INSIDE;
    if( !strcmp( p_str, "dark" ) ) return DARK;
    if( !strcmp( p_str, "flashable" ) ) return FLASHABLE;

    fprintf( stderr, "unknown map type %s\n", p_str );
    return OUTSIDE;
}

eventType getEventType( char* p_str ) {
    if( !strcmp( p_str, "none" ) ) return EVENT_NONE;

    if( !strcmp( p_str, "message" ) ) return EVENT_MESSAGE;
    if( !strcmp( p_str, "item" ) ) return EVENT_ITEM;
    if( !strcmp( p_str, "trainer" ) ) return EVENT_TRAINER;
    if( !strcmp( p_str, "pkmn" ) ) return EVENT_OW_PKMN;
    if( !strcmp( p_str, "npc" ) ) return EVENT_NPC;
    if( !strcmp( p_str, "npc message" ) ) return EVENT_NPC_MESSAGE;
    if( !strcmp( p_str, "warp" ) ) return EVENT_WARP;
    if( !strcmp( p_str, "script" ) ) return EVENT_GENERIC;
    if( !strcmp( p_str, "hmobject" ) ) return EVENT_HMOBJECT;
    if( !strcmp( p_str, "berry tree" ) ) return EVENT_BERRYTREE;

    fprintf( stderr, "unknown event type %s\n", p_str );
    return EVENT_NONE;
}

eventTrigger getEventTrigger( char* p_str ) {
    if( !strcmp( p_str, "none" ) ) return TRIGGER_NONE;

    if( !strcmp( p_str, "step on" ) ) return TRIGGER_STEP_ON;
    if( !strcmp( p_str, "interact" ) ) return TRIGGER_INTERACT;
    if( !strcmp( p_str, "interact up" ) ) return TRIGGER_INTERACT_UP;
    if( !strcmp( p_str, "interact down" ) ) return TRIGGER_INTERACT_DOWN;
    if( !strcmp( p_str, "interact left" ) ) return TRIGGER_INTERACT_LEFT;
    if( !strcmp( p_str, "interact right" ) ) return TRIGGER_INTERACT_RIGHT;
    if( !strcmp( p_str, "on map enter" ) ) return TRIGGER_ON_MAP_ENTER;

    fprintf( stderr, "unknown event trigger %s\n", p_str );
    return TRIGGER_NONE;
}

u8 getHMObject( const char* p_str ) {
    if( !strcmp( p_str, "strength" ) ) return 3;
    if( !strcmp( p_str, "rock smash" ) ) return 4;
    if( !strcmp( p_str, "cut" ) ) return 5;

    fprintf( stderr, "unknown HM object %s\n", p_str );
    return 0;
}

eventTrigger parseEventTriggers( char* p_buffer ) {
    eventTrigger res = TRIGGER_NONE;
    if( !strcmp( p_buffer, "none" ) ) return res;

    char* p = strtok( p_buffer, "|" );
    if( !p ) {
        res = eventTrigger( res | getEventTrigger( p_buffer ) );
    } else {
        do {
            res = eventTrigger( res | getEventTrigger( p ) );
        } while( ( p = strtok( NULL, "|" ) ) );
    }
    return res;
}

mapType parseMapTypes( char* p_buffer ) {
    mapType res = mapType( );
    if( !strcmp( p_buffer, "none" ) ) return res;

    char* p = strtok( p_buffer, "|" );
    if( !p ) {
        res = mapType( res | getMapType( p_buffer ) );
    } else {
        do { res = mapType( res | getMapType( p ) ); } while( ( p = strtok( NULL, "|" ) ) );
    }
    return res;
}

u8 parseTime( char* p_buffer ) {
    if( !strcmp( p_buffer, "Night" ) ) { return 0; }
    if( !strcmp( p_buffer, "Dawn" ) ) { return 1; }
    if( !strcmp( p_buffer, "Morning" ) ) { return 1; }
    if( !strcmp( p_buffer, "Day" ) ) { return 2; }
    if( !strcmp( p_buffer, "Dusk" ) ) { return 3; }
    if( !strcmp( p_buffer, "Evening" ) ) { return 0; }

    fprintf( stderr, "Unknown time \"%s\"", p_buffer );
    return 0;
}

u8 parseDaytimes( char* p_buffer ) {
    u8 res = 0;
    if( !strcmp( p_buffer, "Always" ) ) return 15;

    char* p = strtok( p_buffer, "|" );
    if( !p ) {
        res |= 1 << parseTime( p_buffer );
    } else {
        do { res |= 1 << parseTime( p ); } while( ( p = strtok( NULL, "|" ) ) );
    }
    return res;
}

u8 getMessageType( char* p_str ) {
    if( !strcmp( p_str, "none" ) ) return 0;

    if( !strcmp( p_str, "normal" ) ) return 0;
    if( !strcmp( p_str, "info" ) ) return 1;
    if( !strcmp( p_str, "sign" ) ) return 6;

    fprintf( stderr, "unknown message type %s\n", p_str );
    return 0;
}

u8 getOWItemType( char* p_str ) {
    if( !strcmp( p_str, "none" ) ) return 0;

    if( !strcmp( p_str, "hidden" ) ) return 0;
    if( !strcmp( p_str, "item" ) ) return 1;
    if( !strcmp( p_str, "tm" ) ) return 2;

    fprintf( stderr, "unknown item type %s\n", p_str );
    return 0;
}

u8 getMovementType( char* p_str ) {
    if( !strcmp( p_str, "none" ) ) return 0;
    if( !strcmp( p_str, "look around" ) ) return 15;
    if( !strcmp( p_str, "look left" ) ) return 8;
    if( !strcmp( p_str, "look right" ) ) return 4;
    if( !strcmp( p_str, "look up" ) ) return 1;
    if( !strcmp( p_str, "look down" ) ) return 2;
    if( !strcmp( p_str, "walk left right" ) ) return 16;
    if( !strcmp( p_str, "walk up down" ) ) return 17;
    if( !strcmp( p_str, "walk circle" ) ) return 18;
    if( !strcmp( p_str, "walk around left right" ) ) return 19;
    if( !strcmp( p_str, "walk around up down" ) ) return 20;

    fprintf( stderr, "unknown movement type %s\n", p_str );
    return 0;
}

u8 parseMovementType( char* p_buffer ) {
    u8 res = 0;
    if( !strcmp( p_buffer, "none" ) ) return res;

    char* p = strtok( p_buffer, "|" );
    if( !p ) {
        res |= getMovementType( p_buffer );
    } else {
        do { res |= getMovementType( p ); } while( ( p = strtok( NULL, "|" ) ) );
    }
    return res;
}

warpType getWarpType( char* p_str ) {
    if( !strcmp( p_str, "none" ) ) return NO_SPECIAL;

    if( !strcmp( p_str, "cave entry" ) ) return CAVE_ENTRY;
    if( !strcmp( p_str, "door" ) ) return DOOR;
    if( !strcmp( p_str, "sliding door" ) ) return SLIDING_DOOR;
    if( !strcmp( p_str, "teleport" ) ) return TELEPORT;
    if( !strcmp( p_str, "last visited" ) ) return LAST_VISITED;

    fprintf( stderr, "unknown warp type %s\n", p_str );
    return NO_SPECIAL;
}

mapData::event::data parseEventData( eventType p_type, const char* p_str ) {
    mapData::event::data res;
    char                 buf1[ 50 ], buf2[ 50 ];

    switch( p_type ) {
    case EVENT_MESSAGE:
        sscanf( p_str, "%[^,],%hu", buf1, &res.m_message.m_msgId );
        res.m_message.m_msgType = getMessageType( buf1 );
        break;
    case EVENT_ITEM:
        sscanf( p_str, "%[^,],%[^,]", buf1, buf2 );
        res.m_item.m_itemType = getOWItemType( buf1 );
        res.m_item.m_itemId   = items[ string( fixEncoding( buf2 ) ) ];
        if( !res.m_item.m_itemId ) {
            fprintf( stderr, "[%s] unknown item \"%s\"\n", FILENAME.c_str( ), buf2 );
        }
        break;
    case EVENT_TRAINER:
        sscanf( p_str, "%hu,%hu,%[^,],%hhu", &res.m_trainer.m_spriteId, &res.m_trainer.m_trainerId,
                buf1, &res.m_trainer.m_sight );
        res.m_trainer.m_movementType = parseMovementType( buf1 );
        break;
    case EVENT_OW_PKMN:
        sscanf( p_str, "%[^,],%hhu,%hhu,%hhu", buf1, &res.m_owPkmn.m_level, &res.m_owPkmn.m_forme,
                &res.m_owPkmn.m_shiny );
        res.m_owPkmn.m_speciesId = pkmns[ string( fixEncoding( buf1 ) ) ];
        break;
    case EVENT_NPC:
    case EVENT_NPC_MESSAGE:
        sscanf( p_str, "%[^,],%hu,%hu,%hhu", buf1, &res.m_npc.m_spriteId, &res.m_npc.m_scriptId,
                &res.m_npc.m_scriptType );
        res.m_npc.m_movementType = parseMovementType( buf1 );
        break;
    case EVENT_WARP:
        sscanf( p_str, "%[^,],%hhu,%hhu,%hhu,%hhu,%hhu,%hhu", buf1, &res.m_warp.m_bank,
                &res.m_warp.m_mapX, &res.m_warp.m_mapY, &res.m_warp.m_posX, &res.m_warp.m_posY,
                &res.m_warp.m_posZ );
        res.m_warp.m_warpType = getWarpType( buf1 );
        break;
    case EVENT_GENERIC:
        sscanf( p_str, "%hu,%hhu", &res.m_generic.m_scriptId, &res.m_npc.m_scriptType );
        break;
    case EVENT_HMOBJECT:
        sscanf( p_str, "%[^,]", buf1 );
        res.m_hmObject.m_hmType = getHMObject( buf1 );
        break;
    case EVENT_BERRYTREE:
        sscanf( p_str, "%hhu", &res.m_berryTree.m_treeIdx );
        break;
    default:
        break;
    }

    return res;
}

mapData parseMapData( const char* p_path ) {
    FILE*   f = fopen( p_path, "r" );
    char    buffer[ 500 ];
    mapData res = mapData( );
    char    buf1[ 100 ], buf2[ 100 ], buf3[ 100 ];

    // General data

    fgets( buffer, sizeof( buffer ), f );
    sscanf( buffer, "%[^,],%[^,],%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,", buf1, buf2, &res.m_battleBG,
            &res.m_battlePlat1, &res.m_battlePlat2, &res.m_surfBattleBG, &res.m_surfBattlePlat1,
            &res.m_surfBattlePlat2 );

    res.m_mapType = parseMapTypes( buf1 );
    res.m_weather = getMapWeather( buf2 );

    // location data

    fgets( buffer, sizeof( buffer ), f );
    sscanf( buffer, "%[^,],%hhu,", buf1, &res.m_extraLocationCount );
    res.m_baseLocationId = locations[ string( fixEncoding( buf1 ) ) ];
    if( !res.m_baseLocationId ) {
        fprintf( stderr, "[%s] Unknown or zero location name %s.\n", FILENAME.c_str( ), buf1 );
    }

    for( u8 i = 0; i < res.m_extraLocationCount; ++i ) {
        fgets( buffer, sizeof( buffer ), f );
        sscanf( buffer, "%hhu,%hhu,%hhu,%hhu,%[^,],", &res.m_extraLocations[ i ].m_left,
                &res.m_extraLocations[ i ].m_top, &res.m_extraLocations[ i ].m_right,
                &res.m_extraLocations[ i ].m_bottom, buf1 );
        res.m_extraLocations[ i ].m_locationId = locations[ string( fixEncoding( buf1 ) ) ];
        if( !res.m_extraLocations[ i ].m_locationId ) {
            fprintf( stderr, "[%s] Unknown or zero location name %s.\n", FILENAME.c_str( ), buf1 );
        }
    }

    // wild pokemon

    fgets( buffer, sizeof( buffer ), f );
    sscanf( buffer, "%hhu,", &res.m_pokemonDescrCount );

    for( u8 i = 0; i < res.m_pokemonDescrCount; ++i ) {
        fgets( buffer, sizeof( buffer ), f );
        sscanf( buffer, "%[^,],%hhu,%[^,],%hhu,%[^,],%hhu,", buf1, &res.m_pokemon[ i ].m_forme,
                buf2, &res.m_pokemon[ i ].m_slot, buf3, &res.m_pokemon[ i ].m_encounterRate );

        res.m_pokemon[ i ].m_speciesId     = pkmns[ string( fixEncoding( buf1 ) ) ];
        res.m_pokemon[ i ].m_encounterType = getEncounterType( buf2 );
        res.m_pokemon[ i ].m_daytime       = parseDaytimes( buf3 );
    }

    // events

    fgets( buffer, sizeof( buffer ), f );
    sscanf( buffer, "%hhu,", &res.m_eventCount );

    for( u8 i = 0; i < res.m_eventCount; ++i ) {
        fgets( buffer, sizeof( buffer ), f );
        sscanf( buffer, "%hhu,%hhu,%hhu,%hu,%hu,%[^,],%[^,],", &res.m_events[ i ].m_posX,
                &res.m_events[ i ].m_posY, &res.m_events[ i ].m_posZ,
                &res.m_events[ i ].m_activateFlag, &res.m_events[ i ].m_deactivateFlag, buf1,
                buf2 );

        res.m_events[ i ].m_type    = getEventType( buf1 );
        res.m_events[ i ].m_trigger = parseEventTriggers( buf2 );

        fgets( buffer, sizeof( buffer ), f );
        res.m_events[ i ].m_data = parseEventData( res.m_events[ i ].m_type, buffer );
    }

    fclose( f );
    return res;
}

void printMapData( const mapData& p_mapData, u8 p_bank, u16 p_mapX, u16 p_mapY ) {
    auto path = string( FSROOT ) + "/MAP_DATA/" + std::to_string( p_bank ) + "/"
                + std::to_string( p_mapX ) + "/";
    auto fileName = std::to_string( p_mapX ) + "_" + std::to_string( p_mapY ) + ".map.data";

    fs::create_directories( path );

    FILE* f = fopen( ( path + fileName ).c_str( ), "w" );
    assert( f );
    fwrite( &p_mapData, sizeof( mapData ), 1, f );
    fclose( f );
}

int main( int p_argc, char** p_argv ) {
    if( p_argc < 5 ) {
        fprintf( stderr, "too few args." );
        return 1;
    }
    FILENAME = string( p_argv[ 4 ] );

    readNames( p_argv[ 1 ], pkmn_names );
    for( size_t i = 0; i < pkmn_names.size( ); ++i ) pkmns[ pkmn_names[ i ].m_name[ 0 ] ] = i;

    readNames( p_argv[ 2 ], item_names, 29, 1 );
    for( size_t i = 0; i < item_names.size( ); ++i ) { items[ item_names[ i ].m_name[ 0 ] ] = i; }

    readNames( p_argv[ 3 ], location_names );
    for( auto i : location_names ) locations[ i.second.m_name[ 0 ] ] = i.first;

    auto tdata = parseMapData( p_argv[ 4 ] );
    u8   bank = 0, x = 0, y = 0;
    sscanf( p_argv[ 4 ], "%*[^0-9]%hhu-%hhu_%hhu", &bank, &x, &y );
    // printf( "[%s] Saving to %hhu/%hhu_%hhu\n", p_argv[ 4 ], bank, x, y );
    printMapData( tdata, bank, x, y );
}