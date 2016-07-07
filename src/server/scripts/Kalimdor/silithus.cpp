/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010-2015 QuantumCore <http://vk.com/quantumcore>
 *
 * Copyright (C) 2010-2012 MaNGOS project <http://getmangos.com>
 *
 */

#include "ScriptMgr.h"
#include "QuantumCreature.h"
#include "QuantumGossip.h"
#include "Group.h"

#define GOSSIP_DEMITRIAN1 "What do you know of it?"
#define GOSSIP_DEMITRIAN2 "I am listening, Demitrian."
#define GOSSIP_DEMITRIAN3 "Continue, please."
#define GOSSIP_DEMITRIAN4 "A battle?"
#define GOSSIP_DEMITRIAN5 "<Nod>"
#define GOSSIP_DEMITRIAN6 "Caught unaware? How?"
#define GOSSIP_DEMITRIAN7 "So what did Ragnaros do next?"

enum HighlordDemitrian
{
	QUEST_EXAMINE_THE_VESSEL  = 7785,

	ITEM_ID_LEFT_HAND         = 18563,
	ITEM_ID_RIGHT_HAND        = 18564,
	ITEM_ID_VESSEL_OF_REBIRTH = 19016
};

class npc_highlord_demitrian : public CreatureScript
{
public:
    npc_highlord_demitrian() : CreatureScript("npc_highlord_demitrian") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();

        switch (action)
        {
        case GOSSIP_ACTION_INFO_DEF:
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_DEMITRIAN2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->SEND_GOSSIP_MENU(6842, creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_DEMITRIAN3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            player->SEND_GOSSIP_MENU(6843, creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_DEMITRIAN4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            player->SEND_GOSSIP_MENU(6844, creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_DEMITRIAN5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
            player->SEND_GOSSIP_MENU(6867, creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_DEMITRIAN6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
            player->SEND_GOSSIP_MENU(6868, creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_DEMITRIAN7, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+6);
            player->SEND_GOSSIP_MENU(6869, creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+6:
            player->SEND_GOSSIP_MENU(6870, creature->GetGUID());

            ItemPosCountVec dest;
            uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, ITEM_ID_VESSEL_OF_REBIRTH, 1);
            if (msg == EQUIP_ERR_OK)
                player->StoreNewItem(dest, ITEM_ID_VESSEL_OF_REBIRTH, true);
            break;
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (player->GetQuestStatus(QUEST_EXAMINE_THE_VESSEL) == QUEST_STATUS_NONE
			&& (player->HasItemCount(ITEM_ID_LEFT_HAND, 1, false) || player->HasItemCount(ITEM_ID_RIGHT_HAND, 1, false)))
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_DEMITRIAN1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

		player->SEND_GOSSIP_MENU(6812, creature->GetGUID());
		return true;
    }
};

#define GOSSIP_ITEM1 "I seek information about Natalia"
#define GOSSIP_ITEM2 "That sounds dangerous!"
#define GOSSIP_ITEM3 "What did you do?"
#define GOSSIP_ITEM4 "Who?"
#define GOSSIP_ITEM5 "Women do that. What did she demand?"
#define GOSSIP_ITEM6 "What do you mean?"
#define GOSSIP_ITEM7 "What happened next?"

#define GOSSIP_ITEM11 "Yes, please continue"
#define GOSSIP_ITEM12 "What language?"
#define GOSSIP_ITEM13 "The Priestess attacked you?!"
#define GOSSIP_ITEM14 "I should ask the monkey about this"
#define GOSSIP_ITEM15 "Then what..."

enum RutgarAndFrankal
{
    NPC_TRIGGER_FRANKAL = 15221,
    NPC_TRIGGER_RUTGAR  = 15222,
};

class npcs_rutgar_and_frankal : public CreatureScript
{
public:
    npcs_rutgar_and_frankal() : CreatureScript("npcs_rutgar_and_frankal") { }

	bool OnGossipHello(Player* player, Creature* creature)
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (player->GetQuestStatus(8304) == QUEST_STATUS_INCOMPLETE && creature->GetEntry() == 15170 && !player->GetReqKillOrCastCurrentCount(8304, NPC_TRIGGER_RUTGAR))
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        if (player->GetQuestStatus(8304) == QUEST_STATUS_INCOMPLETE && creature->GetEntry() == 15171 && player->GetReqKillOrCastCurrentCount(8304, NPC_TRIGGER_RUTGAR))
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+9);

        player->SEND_GOSSIP_MENU(7754, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        switch (action)
        {
            case GOSSIP_ACTION_INFO_DEF:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                player->SEND_GOSSIP_MENU(7755, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+1:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
                player->SEND_GOSSIP_MENU(7756, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
                player->SEND_GOSSIP_MENU(7757, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+3:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
                player->SEND_GOSSIP_MENU(7758, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+4:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
                player->SEND_GOSSIP_MENU(7759, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+5:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM7, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+6);
                player->SEND_GOSSIP_MENU(7760, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+6:
                player->SEND_GOSSIP_MENU(7761, creature->GetGUID());
                player->KilledMonsterCredit(NPC_TRIGGER_RUTGAR, 0);
                break;
            case GOSSIP_ACTION_INFO_DEF+9:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM11, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+11);
                player->SEND_GOSSIP_MENU(7762, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+10:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM12, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+11);
                player->SEND_GOSSIP_MENU(7763, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+11:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM13, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+12);
                player->SEND_GOSSIP_MENU(7764, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+12:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM14, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+13);
                player->SEND_GOSSIP_MENU(7765, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+13:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM15, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+14);
                player->SEND_GOSSIP_MENU(7766, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+14:
                player->SEND_GOSSIP_MENU(7767, creature->GetGUID());
                player->KilledMonsterCredit(NPC_TRIGGER_FRANKAL, 0);
                break;
        }
        return true;
    }
};

enum EternalBoard
{
    QUEST_A_PAWN_ON_THE_ETERNAL_BOARD = 8519,

    FACTION_HOSTILE      = 16,
    FACTION_FRIENDLY     = 35,

	NPC_MERITHRA         = 15378,
    NPC_CAELESTRASZ      = 15379,
	NPC_ARYGOS           = 15380,
    NPC_ANACHRONOS       = 15381,
    NPC_FANDRAL_STAGHELM = 15382,

    ANACHRONOS_SAY_1 = -1350000,
    ANACHRONOS_SAY_2 = -1350001,
    ANACHRONOS_SAY_3 = -1350002,
    ANACHRONOS_SAY_4 = -1350003,
    ANACHRONOS_SAY_5 = -1350004,
    ANACHRONOS_SAY_6 = -1350005,
    ANACHRONOS_SAY_7 = -1350006,
    ANACHRONOS_SAY_8 = -1350007,
    ANACHRONOS_SAY_9 = -1350008,
    ANACHRONOS_SAY_10 = -1350009,
    ANACHRONOS_EMOTE_1 = -1350010,
    ANACHRONOS_EMOTE_2 = -1350011,
    ANACHRONOS_EMOTE_3 = -1350012,

    FANDRAL_SAY_1 = -1350013,
    FANDRAL_SAY_2 = -1350014,
    FANDRAL_SAY_3 = -1350015,
    FANDRAL_SAY_4 = -1350016,
    FANDRAL_SAY_5 = -1350017,
    FANDRAL_SAY_6 = -1350018,
    FANDRAL_EMOTE_1 = -1350019,
    FANDRAL_EMOTE_2 = -1350020,

    CAELESTRASZ_SAY_1 = -1350021,
    CAELESTRASZ_SAY_2 = -1350022,
    CAELESTRASZ_YELL_1 = -1350023,

    ARYGOS_SAY_1 = -1350024,
    ARYGOS_YELL_1 = -1350025,
    ARYGOS_EMOTE_1 = -1350026,

    MERITHRA_SAY_1 = -1350027,
    MERITHRA_SAY_2 = -1350028,
    MERITHRA_YELL_1 = -1350029,
    MERITHRA_EMOTE_1 = -1350030,

    GO_GATE_OF_AHN_QIRAJ  = 176146,
	GO_ROOTS_OF_AHN_QIRAJ = 176147,
    GO_GLYPH_OF_AHN_QIRAJ = 176148,
};

#define EVENT_AREA_RADIUS 65 //65yds
#define EVENT_COOLDOWN 500000 //in ms. appear after event completed or failed (should be = Adds despawn time)

struct QuestCinematic
{
    int32 TextId;
    uint32 Creature, Timer;
};

// Creature 0 - Anachronos, 1 - Fandral, 2 - Arygos, 3 - Merithra, 4 - Caelestrasz
static QuestCinematic EventAnim[]=
{
    {ANACHRONOS_SAY_1, 0, 2000},
    {FANDRAL_SAY_1, 1, 4000},
    {MERITHRA_EMOTE_1, 3, 500},
    {MERITHRA_SAY_1, 3, 500},
    {ARYGOS_EMOTE_1, 2, 2000},
    {CAELESTRASZ_SAY_1, 4, 8000},
    {MERITHRA_SAY_2, 3, 6000},
    {0, 3, 2000},
    {MERITHRA_YELL_1, 3, 2500},
    {0, 3, 3000}, //Morph
    {0, 3, 4000}, //EmoteLiftoff
    {0, 3, 4000}, // spell
    {0, 3, 1250}, //fly
    {0, 3, 250}, //remove flags
    {ARYGOS_SAY_1, 2, 3000},
    {0, 3, 2000},
    {ARYGOS_YELL_1, 2, 3000},
    {0, 3, 3000}, //Morph
    {0, 3, 4000}, //EmoteLiftoff
    {0, 3, 4000}, // spell
    {0, 3, 1000}, //fly
    {0, 3, 1000}, //remove flags
    {CAELESTRASZ_SAY_2, 4, 5000},
    {0, 3, 3000},
    {CAELESTRASZ_YELL_1, 4, 3000},
    {0, 3, 3000}, //Morph
    {0, 3, 4000}, //EmoteLiftoff
    {0, 3, 2500}, // spell
    {ANACHRONOS_SAY_2, 0, 2000},
    {0, 3, 250}, //fly
    {0, 3, 25}, //remove flags
    {FANDRAL_SAY_2, 1, 3000},
    {ANACHRONOS_SAY_3, 0, 10000}, //Both run through the armies
    {0, 3, 2000}, // Sands will stop
    {0, 3, 8000}, // Summon Gate
    {ANACHRONOS_SAY_4, 0, 4000},
    {0, 0, 2000}, //spell 1-> Arcane cosmetic (Mobs freeze)
    {0, 0, 5000}, //Spell 2-> Arcane long cosmetic (barrier appears) (Barrier -> Glyphs)
    {0, 0, 7000}, //BarrieR
    {0, 0, 4000}, //Glyphs
    {ANACHRONOS_SAY_5, 0, 2000},
    {0, 0, 4000}, // Roots
    {FANDRAL_SAY_3, 1, 3000}, //Root Text
    {FANDRAL_EMOTE_1, 1, 3000}, //falls knee
    {ANACHRONOS_SAY_6, 0, 3000},
    {ANACHRONOS_SAY_7, 0, 3000},
    {ANACHRONOS_SAY_8, 0, 8000},
    {ANACHRONOS_EMOTE_1, 0, 1000}, //Give Scepter
    {FANDRAL_SAY_4, 1, 3000},
    {FANDRAL_SAY_5, 1, 3000}, //->Equip hammer~Scepter, throw it at door
    {FANDRAL_EMOTE_2, 1, 3000}, //Throw hammer at door.
    {ANACHRONOS_SAY_9, 0, 3000},
    {FANDRAL_SAY_6, 1, 3000}, //fandral goes away
    {ANACHRONOS_EMOTE_2, 0, 3000},
    {ANACHRONOS_EMOTE_3, 0, 3000},
    {0, 0, 2000},
    {0, 0, 2000},
    {0, 0, 4000},
    {ANACHRONOS_SAY_10, 0, 3000},
    {0, 0, 2000},
    {0, 0, 3000},
    {0, 0, 15000},
    {0, 0, 5000},
    {0, 0, 3500},
    {0, 0, 5000},
    {0, 0, 3500},
    {0, 0, 5000},
    {0, 0, 0}
};

//Cordinates for Spawns
static const Position SpawnLocation[]=
{
    {-8085.0f, 1528.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8080.0f, 1526.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8085.0f, 1524.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8080.0f, 1522.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8085.0f, 1520.0f, 2.61f, 3.141592f}, //Kaldorei Infantry

    {-8085.0f, 1524.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8080.0f, 1522.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8085.0f, 1520.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8080.0f, 1518.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8085.0f, 1516.0f, 2.61f, 3.141592f}, //Kaldorei Infantry

    {-8085.0f, 1518.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8080.0f, 1516.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8080.0f, 1520.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8080.0f, 1424.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8085.0f, 1422.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    // 2 waves of warriors
    {-8082.0f, 1528.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8078.0f, 1525.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8082.0f, 1524.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8078.0f, 1526.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8082.0f, 1527.0f, 2.61f, 3.141592f}, //Kaldorei Infantry

    {-8082.0f, 1524.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8078.0f, 1522.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8082.0f, 1520.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8078.0f, 1518.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8082.0f, 1516.0f, 2.61f, 3.141592f}, //Kaldorei Infantry

    {-8082.0f, 1523.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8078.0f, 1521.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8082.0f, 1528.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8078.0f, 1519.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8082.0f, 1526.0f, 2.61f, 3.141592f}, //Kaldorei Infantry

    {-8082.0f, 1524.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8078.0f, 1522.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8082.0f, 1520.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8078.0f, 1518.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8082.0f, 1516.0f, 2.61f, 3.141592f}, //Kaldorei Infantry

    {-8088.0f, 1510.0f, 2.61f, 0.0f}, //Anubisath Conqueror
    {-8084.0f, 1520.0f, 2.61f, 0.0f}, //Anubisath Conqueror
    {-8088.0f, 1530.0f, 2.61f, 0.0f}, //Anubisath Conqueror

    {-8080.0f, 1513.0f, 2.61f, 0.0f}, //Qiraj Wasp
    {-8082.0f, 1523.0f, 2.61f, 0.0f}, //Qiraj Wasp
    {-8085.0f, 1518.0f, 2.61f, 0.0f}, //Qiraj Wasp
    {-8082.0f, 1516.0f, 2.61f, 0.0f}, //Qiraj Wasp
    {-8085.0f, 1520.0f, 2.61f, 0.0f}, //Qiraj Wasp
    {-8080.0f, 1528.0f, 2.61f, 0.0f}, //Qiraj Wasp

    {-8082.0f, 1513.0f, 2.61f, 0.0f}, //Qiraj Wasp
    {-8079.0f, 1523.0f, 2.61f, 0.0f}, //Qiraj Wasp
    {-8080.0f, 1531.0f, 2.61f, 0.0f}, //Qiraj Wasp
    {-8079.0f, 1516.0f, 2.61f, 0.0f}, //Qiraj Wasp
    {-8082.0f, 1520.0f, 2.61f, 0.0f}, //Qiraj Wasp
    {-8080.0f, 1518.0f, 2.61f, 0.0f}, //Qiraj Wasp

    {-8081.0f, 1514.0f, 2.61f, 0.0f}, //Qiraj Tank
    {-8081.0f, 1520.0f, 2.61f, 0.0f}, //Qiraj Tank
    {-8081.0f, 1526.0f, 2.61f, 0.0f}, //Qiraj Tank
    {-8081.0f, 1512.0f, 2.61f, 0.0f}, //Qiraj Tank
    {-8082.0f, 1520.0f, 2.61f, 0.0f}, //Qiraj Tank
    {-8081.0f, 1528.0f, 2.61f, 0.0f}, //Qiraj Tank

    {-8082.0f, 1513.0f, 2.61f, 3.141592f}, //Anubisath Conqueror
    {-8082.0f, 1520.0f, 2.61f, 3.141592f}, //Anubisath Conqueror
    {-8082.0f, 1527.0f, 2.61f, 3.141592f}, //Anubisath Conqueror
};

struct WaveData
{
    uint8 SpawnCount, UsedSpawnPoint;
    uint32 CreatureId, SpawnTimer, YellTimer, DespTimer;
    int32 WaveTextId;
};

static WaveData WavesInfo[] =
{
    {30,  0, 15423, 0, 0, 24000, 0},    // Kaldorei Soldier
    { 3, 35, 15424, 0, 0, 24000, 0},    // Anubisath Conqueror
    {12, 38, 15414, 0, 0, 24000, 0},    // Qiraji Wasps
    { 6, 50, 15422, 0, 0, 24000, 0},    // Qiraji Tanks
    {15, 15, 15423, 0, 0, 24000, 0}     // Kaldorei Soldier
};

struct SpawnSpells
{
    uint32 Timer1, Timer2, SpellId;
};

static SpawnSpells SpawnCast[]=//
{
    {100000, 2000, 33652},   // Stop Time
    {38500, 300000, 28528},  // Poison Cloud
    {58000, 300000, 35871},  // Frost Debuff (need correct spell)
    {80950, 300000, 42075},  // Fire Explosion (need correct spell however this one looks cool)
};

class npc_anachronos_the_ancient : public CreatureScript
{
public:
    npc_anachronos_the_ancient() : CreatureScript("npc_anachronos_the_ancient") { }

    struct npc_anachronos_the_ancientAI : public QuantumBasicAI
    {
        npc_anachronos_the_ancientAI(Creature* creature) : QuantumBasicAI(creature) {}

        uint32 AnimationTimer;
        uint8 AnimationCount;

        uint64 AnachronosQuestTriggerGUID;
        uint64 MerithraGUID;
        uint64 ArygosGUID;
        uint64 CaelestraszGUID;
        uint64 FandralGUID;
        uint64 PlayerGUID;
        bool eventEnd;

        void Reset()
        {
            AnimationTimer = 1500;
            AnimationCount = 0;
            AnachronosQuestTriggerGUID = 0;
            MerithraGUID = 0;
            ArygosGUID = 0;
            CaelestraszGUID = 0;
            FandralGUID = 0;
            PlayerGUID = 0;
            eventEnd = false;

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        void HandleAnimation()
        {
            Player* player = Unit::GetPlayer(*me, PlayerGUID);
            if (!player)
                return;

            Unit* Fandral = player->FindCreatureWithDistance(NPC_FANDRAL_STAGHELM, 100.0f, me);
            Unit* Arygos = player->FindCreatureWithDistance(NPC_ARYGOS, 100.0f, me);
            Unit* Caelestrasz = player->FindCreatureWithDistance(NPC_CAELESTRASZ, 100.0f, me);
            Unit* Merithra = player->FindCreatureWithDistance(NPC_MERITHRA, 100.0f, me);

            if (!Fandral || !Arygos || !Caelestrasz || !Merithra)
                return;

            Unit* mob = NULL;
            AnimationTimer = EventAnim[AnimationCount].Timer;
            if (eventEnd == false)
            {
                switch (AnimationCount)
                {
                    case 0:
                        DoSendQuantumText(ANACHRONOS_SAY_1, me , Fandral);
                        break;
                    case 1:
                        Fandral->SetTarget(me->GetGUID());
                        DoSendQuantumText(FANDRAL_SAY_1, Fandral, me);
                        break;
                    case 2:
                        Fandral->SetTarget(0);
                        DoSendQuantumText(MERITHRA_EMOTE_1, Merithra);
                        break;
                    case 3:
                        DoSendQuantumText(MERITHRA_SAY_1, Merithra);
                        break;
                    case 4:
                        DoSendQuantumText(ARYGOS_EMOTE_1, Arygos);
                        break;
                    case 5:
                        Caelestrasz->SetTarget(Fandral->GetGUID());
                        DoSendQuantumText(CAELESTRASZ_SAY_1, Caelestrasz);
                        break;
                    case 6:
                        DoSendQuantumText(MERITHRA_SAY_2, Merithra);
                        break;
                    case 7:
                        Caelestrasz->SetTarget(0);
                        Merithra->GetMotionMaster()->MoveCharge(-8065, 1530, 2.61f, 10);
                        break;
                    case 8:
                        DoSendQuantumText(MERITHRA_YELL_1, Merithra);
                        break;
                    case 9:
                        Merithra->CastSpell(Merithra, 25105, true);
                        break;
                    case 10:
						Merithra->SetDisableGravity(true);
						Merithra->SetUInt32Value(UNIT_FIELD_BYTES_1, FIELD_BYTE_1_FLYING);
						Merithra->HandleEmoteCommand(EMOTE_ONESHOT_FLY_SIT_GROUND_UP);
                        Merithra->GetMotionMaster()->MoveCharge(-8065, 1530, 6.61f, 3);
                        break;
                    case 11:
                        Merithra->CastSpell(Merithra, 24818, false);
                        break;
                    case 12:
                        Merithra->GetMotionMaster()->MoveCharge(-8100, 1530, 50, 42);
                        break;
                    case 13:
                        break;
                    case 14:
                        DoSendQuantumText(ARYGOS_SAY_1, Arygos);
                        Merithra->SetVisible(false);
                        break;
                    case 15:
                        Arygos->GetMotionMaster()->MoveCharge(-8065, 1530, 2.61f, 10);
                        Merithra->GetMotionMaster()->MoveCharge(-8034.535f, 1535.14f, 2.61f, 42);
                        break;
                    case 16:
                        DoSendQuantumText(ARYGOS_YELL_1, Arygos);
                        break;
                    case 17:
                        Arygos->CastSpell(Arygos, 25107, true);
                        break;
                    case 18:
						Arygos->SetDisableGravity(true);
						Arygos->SetUInt32Value(UNIT_FIELD_BYTES_1, FIELD_BYTE_1_FLYING);
						Arygos->HandleEmoteCommand(EMOTE_ONESHOT_FLY_SIT_GROUND_UP);
                        Arygos->GetMotionMaster()->MoveCharge(-8065, 1530, 6.61f, 42);
                        break;
                    case 19:
                        Arygos->CastSpell(Arygos, 50505, false);
                        break;
                    case 20:
                        Arygos->GetMotionMaster()->MoveCharge(-8095, 1530, 50, 42);
                        break;
                    case 21:
                        break;
                    case 22:
                        DoSendQuantumText(CAELESTRASZ_SAY_2, Caelestrasz, Fandral);
                        break;
                    case 23:
                        Caelestrasz->GetMotionMaster()->MoveCharge(-8065, 1530, 2.61f, 10);
                        Arygos->SetVisible(false);
                        Arygos->GetMotionMaster()->MoveCharge(-8034.535f, 1535.14f, 2.61f, 10);
                        break;
                    case 24:
                        DoSendQuantumText(CAELESTRASZ_YELL_1, Caelestrasz);
                        break;
                    case 25:
                        Caelestrasz->CastSpell(Caelestrasz, 25106, true);
                        break;
                    case 26:
						Caelestrasz->SetDisableGravity(true);
						Caelestrasz->SetUInt32Value(UNIT_FIELD_BYTES_1, FIELD_BYTE_1_FLYING);
						Caelestrasz->HandleEmoteCommand(EMOTE_ONESHOT_FLY_SIT_GROUND_UP);
                        Caelestrasz->GetMotionMaster()->MoveCharge(-8065, 1530, 7.61f, 4);
                        break;
                    case 27:
                        Caelestrasz->CastSpell(Caelestrasz, 54293, false);
                        break;
                    case 28:
                        DoSendQuantumText(ANACHRONOS_SAY_2, me, Fandral);
                        break;
                    case 29:
                        Caelestrasz->GetMotionMaster()->MoveCharge(-8095, 1530, 50, 42);
                        DoSendQuantumText(FANDRAL_SAY_2, Fandral, me);
                        break;
                    case 30:
                        break;
                    case 31:
                        DoSendQuantumText(ANACHRONOS_SAY_3, me, Fandral);
                        break;
                    case 32:
                        Caelestrasz->SetVisible(false);
                        Caelestrasz->GetMotionMaster()->MoveCharge(-8034.535f, 1535.14f, 2.61f, 42);
                        Fandral->GetMotionMaster()->MoveCharge(-8108, 1529, 2.77f, 8);
                        me->GetMotionMaster()->MoveCharge(-8113, 1525, 2.77f, 8);
                        break;//both run to the gate
                    case 33:
                        DoSendQuantumText(ANACHRONOS_SAY_4, me);
                        Caelestrasz->GetMotionMaster()->MoveCharge(-8050, 1473, 65, 15);
                        break; //Text: sands will stop
                    case 34:
                        DoCast(player, 23017, true);//Arcane Channeling
                        break;
                    case 35:
                        me->CastSpell(-8088, 1520.43f, 2.67f, 25158, true);
                        break;
                    case 36:
                        DoCast(player, 25159, true);
                        break;
                    case 37:
                        me->SummonGameObject(GO_GATE_OF_AHN_QIRAJ, -8130, 1525, 17.5f, 0, 0, 0, 0, 0, 0);
                        break;
                    case 38:
                        DoCast(player, 25166, true);
                        me->SummonGameObject(GO_GLYPH_OF_AHN_QIRAJ, -8130, 1525, 17.5f, 0, 0, 0, 0, 0, 0);
                        break;
                    case 39:
                        DoSendQuantumText(ANACHRONOS_SAY_5, me, Fandral);
                        break;
                    case 40:
                        Fandral->CastSpell(me, 25167, true);
                        break;
                    case 41:
                        Fandral->SummonGameObject(GO_ROOTS_OF_AHN_QIRAJ, -8130, 1525, 17.5f, 0, 0, 0, 0, 0, 0);
                        DoSendQuantumText(FANDRAL_SAY_3, Fandral);
                        break;
                    case 42:
                        me->CastStop();
                        DoSendQuantumText(FANDRAL_EMOTE_1, Fandral);
                        break;
                    case 43:
                        Fandral->CastStop();
                        break;
                    case 44:
                        DoSendQuantumText(ANACHRONOS_SAY_6, me);
                        break;
                    case 45:
                        DoSendQuantumText(ANACHRONOS_SAY_7, me);
                        break;
                    case 46:
                        DoSendQuantumText(ANACHRONOS_SAY_8, me);
                        me->GetMotionMaster()->MoveCharge(-8110, 1527, 2.77f, 4);
                        break;
                    case 47:
                        DoSendQuantumText(ANACHRONOS_EMOTE_1, me);
                        break;
                    case 48:
                        DoSendQuantumText(FANDRAL_SAY_4, Fandral, me);
                        break;
                    case 49:
                        DoSendQuantumText(FANDRAL_SAY_5, Fandral, me);
                        break;
                    case 50:
                        DoSendQuantumText(FANDRAL_EMOTE_2, Fandral);
                        Fandral->CastSpell(-8127, 1525, 17.5f, 33806, true);
                        break;
                    case 51:
                    {
                        uint32 entries[4] = { 15423, 15424, 15414, 15422 };
                        for (uint8 i = 0; i < 4; ++i)
                        {
                            mob = player->FindCreatureWithDistance(entries[i], 50, me);
                            while (mob)
                            {
                                mob->RemoveFromWorld();
                                mob = player->FindCreatureWithDistance(15423, 50, me);
                            }
                        }
                        break;
                    }
                    case 52:
                        Fandral->GetMotionMaster()->MoveCharge(-8028.75f, 1538.795f, 2.61f, 4);
                        DoSendQuantumText(ANACHRONOS_SAY_9, me, Fandral);
                        break;
                    case 53:
                        DoSendQuantumText(FANDRAL_SAY_6, Fandral);
                        break;
                    case 54:
                        DoSendQuantumText(ANACHRONOS_EMOTE_2, me);
                        break;
                    case 55:
                        Fandral->SetVisible(false);
                        break;
                    case 56:
                        DoSendQuantumText(ANACHRONOS_EMOTE_3, me);
                        me->GetMotionMaster()->MoveCharge(-8116, 1522, 3.65f, 4);
                        break;
                    case 57:
                        me->GetMotionMaster()->MoveCharge(-8116.7f, 1527, 3.7f, 4);
                        break;
                    case 58:
                        me->GetMotionMaster()->MoveCharge(-8112.67f, 1529.9f, 2.86f, 4);
                        break;
                    case 59:
                        me->GetMotionMaster()->MoveCharge(-8117.99f, 1532.24f, 3.94f, 4);
                        break;
                    case 60:
                        if (player)
                            DoSendQuantumText(ANACHRONOS_SAY_10, me, player);
                        me->GetMotionMaster()->MoveCharge(-8113.46f, 1524.16f, 2.89f, 4);
                        break;
                    case 61:
                        me->GetMotionMaster()->MoveCharge(-8057.1f, 1470.32f, 2.61f, 6);
                        if (player->IsInRange(me, 0, 15))
						{
							player->AreaExploredOrEventHappens(QUEST_A_PAWN_ON_THE_ETERNAL_BOARD);
                            player->GroupEventHappens(QUEST_A_PAWN_ON_THE_ETERNAL_BOARD , me);
						}
                        break;
                    case 62:
                        me->SetDisplayId(15500);
                        break;
                    case 63:
                        me->HandleEmoteCommand(254);
                        me->SetDisableGravity(true);
                        break;
                    case 64:
                        me->GetMotionMaster()->MoveCharge(-8000, 1400, 150, 9);
                        break;
                    case 65:
                        me->SetVisible(false);
                        if (Creature* AnachronosQuestTrigger = (Unit::GetCreature(*me, AnachronosQuestTriggerGUID)))
                        {
                            DoSendQuantumText(ARYGOS_YELL_1, me);
                            AnachronosQuestTrigger->AI()->EnterEvadeMode();
                            eventEnd = true;
                        }
                        break;
                }
            }
            ++AnimationCount;
        }
        void UpdateAI(const uint32 diff)
        {
            if (AnimationTimer)
            {
                if (AnimationTimer <= diff)
                    HandleAnimation();
                else AnimationTimer -= diff;
            }
            if (AnimationCount < 65)
                me->CombatStop();
            if (AnimationCount == 65 || eventEnd)
                me->AI()->EnterEvadeMode();
        }
    };

	CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_anachronos_the_ancientAI(creature);
    }
};

class mob_qiraj_war_spawn : public CreatureScript
{
public:
    mob_qiraj_war_spawn() : CreatureScript("mob_qiraj_war_spawn") { }

    struct mob_qiraj_war_spawnAI : public QuantumBasicAI
    {
        mob_qiraj_war_spawnAI(Creature* creature) : QuantumBasicAI(creature) {}

        uint64 MobGUID;
        uint64 PlayerGUID;
        uint32 SpellTimer1, SpellTimer2, SpellTimer3, SpellTimer4;

        bool Timers;
        bool hasTarget;

        void Reset()
        {
            MobGUID = 0;
            PlayerGUID = 0;
            Timers = false;
            hasTarget = false;
        }

        void EnterToBattle(Unit* /*who*/) {}

		void JustDied(Unit* /*killer*/);

        void UpdateAI(const uint32 diff)
        {
            Unit* target = NULL;

            if (!Timers)
            {
                if (me->GetEntry() == 15424 || me->GetEntry() == 15422 || me->GetEntry() == 15414) //all but Kaldorei Soldiers
                {
                    SpellTimer1 = SpawnCast[1].Timer1;
                    SpellTimer2 = SpawnCast[2].Timer1;
                    SpellTimer3 = SpawnCast[3].Timer1;
                }
                if (me->GetEntry() == 15423 || me->GetEntry() == 15424 || me->GetEntry() == 15422 || me->GetEntry() == 15414)
                    SpellTimer4 = SpawnCast[0].Timer1;
                Timers = true;
            }

            if (me->GetEntry() == 15424 || me->GetEntry() == 15422|| me->GetEntry() == 15414)
            {
                if (SpellTimer1 <= diff)
                {
                    DoCast(me, SpawnCast[1].SpellId);
                    DoCast(me, 24319);
                    SpellTimer1 = SpawnCast[1].Timer2;
                }
				else SpellTimer1 -= diff;

                if (SpellTimer2 <= diff)
                {
                    DoCast(me, SpawnCast[2].SpellId);
                    SpellTimer2 = SpawnCast[2].Timer2;
                }
				else SpellTimer2 -= diff;

                if (SpellTimer3 <= diff)
                {
                    DoCast(me, SpawnCast[3].SpellId);
                    SpellTimer3 = SpawnCast[3].Timer2;
                }
				else SpellTimer3 -= diff;
            }

            if (me->GetEntry() == 15423 || me->GetEntry() == 15424 || me->GetEntry() == 15422 || me->GetEntry() == 15414)
            {
                if (SpellTimer4 <= diff)
                {
                    me->RemoveAllAttackers();
                    me->AttackStop();
                    DoCast(me, 15533);
                    SpellTimer4 = SpawnCast[0].Timer2;
                }
				else SpellTimer4 -= diff;
            }

            if (!hasTarget)
            {
                if (me->GetEntry() == 15424 || me->GetEntry() == 15422 || me->GetEntry() == 15414)
                    target = me->FindCreatureWithDistance(15423, 20, true);

                if (me->GetEntry() == 15423)
                {
                    uint8 tar = urand(0, 2);

                    if (tar == 0)
                        target = me->FindCreatureWithDistance(15422, 20, true);
                    else if (tar == 1)
                        target = me->FindCreatureWithDistance(15424, 20, true);
                    else if (tar == 2)
                        target = me->FindCreatureWithDistance(15414, 20, true);
                }
                hasTarget = true;
                if (target)
                    me->AI()->AttackStart(target);
            }

            if (!(me->FindCreatureWithDistance(NPC_CAELESTRASZ, 60)))
                DoCast(me, 33652);

            if (!UpdateVictim())
            {
                hasTarget = false;
                return;
            }

            DoMeleeAttackIfReady();
        }
    };

	CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_qiraj_war_spawnAI(creature);
    }
};

class npc_anachronos_quest_trigger : public CreatureScript
{
public:
    npc_anachronos_quest_trigger() : CreatureScript("npc_anachronos_quest_trigger") { }

    struct npc_anachronos_quest_triggerAI : public QuantumBasicAI
    {
        npc_anachronos_quest_triggerAI(Creature* creature) : QuantumBasicAI(creature) {}

        uint64 PlayerGUID;

        uint32 WaveTimer;
        uint32 AnnounceTimer;

        int8 LiveCount;
        uint8 WaveCount;

        bool EventStarted;
        bool Announced;
        bool Failed;

        void Reset()
        {
            PlayerGUID = 0;

            WaveTimer = 2000;
            AnnounceTimer = 1000;
            LiveCount = 0;
            WaveCount = 0;

            EventStarted = false;
            Announced = false;
            Failed = false;

            me->SetVisible(false);
        }

        void SummonNextWave()
        {
            uint8 locIndex = WavesInfo[WaveCount].UsedSpawnPoint;
            for (uint8 i = 0; i < 67; ++i)
            {
                if (Creature* spawn = me->SummonCreature(WavesInfo[WaveCount].CreatureId, SpawnLocation[locIndex+i], TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, WavesInfo[WaveCount].DespTimer))
                {
                    spawn->LoadCreaturesAddon();
                    if (spawn->GetEntry() == 15423)
                        spawn->SetUInt32Value(UNIT_FIELD_DISPLAY_ID, 15427+rand()%4);
                    if (i >= 30)
						WaveCount = 1;
                    if (i >= 33)
						WaveCount = 2;
                    if (i >= 45)
						WaveCount = 3;
                    if (i >= 51)
						WaveCount = 4;

                    if (WaveCount < 5) //1-4 Wave
                    {
                        mob_qiraj_war_spawn::mob_qiraj_war_spawnAI* spawnAI = CAST_AI(mob_qiraj_war_spawn::mob_qiraj_war_spawnAI, spawn->AI());
                        spawnAI->MobGUID = me->GetGUID();
                        spawnAI->PlayerGUID = PlayerGUID;
                    }
                }
            }

            WaveTimer = WavesInfo[WaveCount].SpawnTimer;
            AnnounceTimer = WavesInfo[WaveCount].YellTimer;
        }

        void CheckEventFail()
        {
            Player* player = Unit::GetPlayer(*me, PlayerGUID);

            if (!player)
                return;

            if (Group* EventGroup = player->GetGroup())
            {
                Player* GroupMember;

                uint8 GroupMemberCount = 0;
                uint8 DeadMemberCount = 0;
                uint8 FailedMemberCount = 0;

                const Group::MemberSlotList members = EventGroup->GetMemberSlots();

                for (Group::member_citerator itr = members.begin(); itr!= members.end(); ++itr)
                {
                    GroupMember = (Unit::GetPlayer(*me, itr->guid));
                    if (!GroupMember)
                        continue;
                    if (!GroupMember->IsWithinDistInMap(me, EVENT_AREA_RADIUS) && GroupMember->GetQuestStatus(QUEST_A_PAWN_ON_THE_ETERNAL_BOARD) == QUEST_STATUS_INCOMPLETE)
                    {
                         GroupMember->FailQuest(QUEST_A_PAWN_ON_THE_ETERNAL_BOARD);
                        ++FailedMemberCount;
                    }
                    ++GroupMemberCount;

                    if (GroupMember->IsDead())
                        ++DeadMemberCount;
                }

                if (GroupMemberCount == FailedMemberCount || !player->IsWithinDistInMap(me, EVENT_AREA_RADIUS))
                    Failed = true; //only so event can restart
            }
        }

        void LiveCounter()
        {
            --LiveCount;
            if (!LiveCount)
                Announced = false;
        }

        void UpdateAI(const uint32 diff)
        {
            if (!PlayerGUID || !EventStarted)
                return;

            if (WaveCount < 4)
            {
                if (!Announced && AnnounceTimer <= diff)
                {
                    DoSendQuantumText(WavesInfo[WaveCount].WaveTextId, me);
                    Announced = true;
                }
				else AnnounceTimer -= diff;

                if (WaveTimer <= diff)
                    SummonNextWave();
                else WaveTimer -= diff;
            }
            CheckEventFail();
            if (WaveCount == 4 || Failed)
                EnterEvadeMode();
        };
    };

	CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_anachronos_quest_triggerAI(creature);
    }
};

void mob_qiraj_war_spawn::mob_qiraj_war_spawnAI::JustDied(Unit* /*slayer*/)
{
    me->RemoveCorpse();
    if (Creature* Mob = (Unit::GetCreature(*me, MobGUID)))
        CAST_AI(npc_anachronos_quest_trigger::npc_anachronos_quest_triggerAI, Mob->AI())->LiveCounter();
};

class go_crystalline_tear : public GameObjectScript
{
public:
    go_crystalline_tear() : GameObjectScript("go_crystalline_tear") { }

    bool OnQuestAccept(Player* player, GameObject* go, Quest const* quest)
    {
        if (quest->GetQuestId() == QUEST_A_PAWN_ON_THE_ETERNAL_BOARD)
        {
            if (Unit* Anachronos_Quest_Trigger = go->FindCreatureWithDistance(15454, 100, player))
            {
                Unit* Merithra = Anachronos_Quest_Trigger->SummonCreature(NPC_MERITHRA, -8034.535f, 1535.14f, 2.61f, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 220000);
                Unit* Caelestrasz = Anachronos_Quest_Trigger->SummonCreature(NPC_CAELESTRASZ, -8032.767f, 1533.148f, 2.61f, 1.5f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 220000);
                Unit* Arygos = Anachronos_Quest_Trigger->SummonCreature(NPC_ARYGOS, -8034.52f, 1537.843f, 2.61f, 5.7f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 220000);
                /* Unit* Fandral = */ Anachronos_Quest_Trigger->SummonCreature(NPC_FANDRAL_STAGHELM, -8028.462f, 1535.843f, 2.61f, 3.141592f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 220000);
                Creature* Anachronos = Anachronos_Quest_Trigger->SummonCreature(NPC_ANACHRONOS, -8028.75f, 1538.795f, 2.61f, 4, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 220000);

                if (Merithra)
                {
                    Merithra->SetUInt32Value(UNIT_NPC_FLAGS, 0);
                    Merithra->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
                    Merithra->SetUInt32Value(UNIT_FIELD_DISPLAY_ID, 15420);
                    Merithra->SetCurrentFaction(35);
                }

                if (Caelestrasz)
                {
                    Caelestrasz->SetUInt32Value(UNIT_NPC_FLAGS, 0);
                    Caelestrasz->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
                    Caelestrasz->SetUInt32Value(UNIT_FIELD_DISPLAY_ID, 15419);
                    Caelestrasz->SetCurrentFaction(35);
                }

                if (Arygos)
                {
                    Arygos->SetUInt32Value(UNIT_NPC_FLAGS, 0);
                    Arygos->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
                    Arygos->SetUInt32Value(UNIT_FIELD_DISPLAY_ID, 15418);
                    Arygos->SetCurrentFaction(35);
                }

                if (Anachronos)
                {
                    CAST_AI(npc_anachronos_the_ancient::npc_anachronos_the_ancientAI, Anachronos->AI())->PlayerGUID = player->GetGUID();
                    CAST_AI(npc_anachronos_quest_trigger::npc_anachronos_quest_triggerAI, CAST_CRE(Anachronos_Quest_Trigger)->AI())->Failed=false;
                    CAST_AI(npc_anachronos_quest_trigger::npc_anachronos_quest_triggerAI, CAST_CRE(Anachronos_Quest_Trigger)->AI())->PlayerGUID = player->GetGUID();
                    CAST_AI(npc_anachronos_quest_trigger::npc_anachronos_quest_triggerAI, CAST_CRE(Anachronos_Quest_Trigger)->AI())->EventStarted=true;
                    CAST_AI(npc_anachronos_quest_trigger::npc_anachronos_quest_triggerAI, CAST_CRE(Anachronos_Quest_Trigger)->AI())->Announced=true;
                }
            }
        }
        return true;
    }
};

#define GOSSIP_SUM_TEMPLAR_RANDOM  "Summon Elemtal Templar"
#define GOSSIP_SUM_TEMPLAR_FIRE    "Summon Templar of the Fire"
#define GOSSIP_SUM_TEMPLAR_WATER   "Summon Templar of the Water"
#define GOSSIP_SUM_TEMPLAR_STONE   "Summon Templar of the Stone"
#define GOSSIP_SUM_TEMPLAR_THUNDER "Summon Templar of the Thunder"
#define GOSSIP_SUM_DUKE_RANDOM     "Summon Elemtal Duke"
#define GOSSIP_SUM_DUKE_FIRE       "Summon Duke of the Fire"
#define GOSSIP_SUM_DUKE_WATER      "Summon Duke of the Water"
#define GOSSIP_SUM_DUKE_STONE      "Summon Duke of the Stone"
#define GOSSIP_SUM_DUKE_THUNDER    "Summon Duke of the Thunder"
#define GOSSIP_SUM_ROYAL_RANDOM    "Summon Elemtal Royal"
#define GOSSIP_SUM_ROYAL_FIRE      "Summon Royal of the Fire"
#define GOSSIP_SUM_ROYAL_WATER     "Summon Royal of the Water"
#define GOSSIP_SUM_ROYAL_STONE     "Summon Royal of the Stone"
#define GOSSIP_SUM_ROYAL_THUNDER   "Summon Royal of the Thunder"

enum WindStone
{
	ENTRY_LESSER_WINDSTONE         = 180456,
	ENTRY_GREATER_WINDSTONE        = 180466,
	ENTRY_NORMAL_WINDSTONE         = 180461,

	ENTRY_TEMPLAR_FIRE             = 15209,
	ENTRY_TEMPLAR_THUNDER          = 15212,
	ENTRY_TEMPLAR_STONE            = 15307,
	ENTRY_TEMPLAR_WATER            = 15211,
	ENTRY_DUKE_FIRE                = 15206,
	ENTRY_DUKE_THUNDER             = 15220,
	ENTRY_DUKE_STONE               = 15208,
	ENTRY_DUKE_WATER               = 15207,
	ENTRY_ROYAL_FIRE               = 15203,
	ENTRY_ROYAL_THUNDER            = 15204,
	ENTRY_ROYAL_STONE              = 15205,
	ENTRY_ROYAL_WATER              = 15305,

	ITEM_CREST_FIRE                = 20416,
	ITEM_CREST_THUNDER             = 20418,
	ITEM_CREST_STONE               = 20419,
	ITEM_CREST_WATER               = 20420,
	ITEM_SIGNET_FIRE               = 20432,
	ITEM_SIGNET_THUNDER            = 20433,
	ITEM_SIGNET_STONE              = 20435,
	ITEM_SIGNET_WATER              = 20436,
	ITEM_SCEPTER_FIRE              = 20447,
	ITEM_SCEPTER_THUNDER           = 20448,
	ITEM_SCEPTER_STONE             = 20449,
	ITEM_SCEPTER_WATER             = 20450,

	SPELL_TWILIGHT_LESSER          = 24746,
	SPELL_TWILIGHT_NORMAL          = 24748,
	SPELL_TWILIGHT_GREATER         = 24782,
	SPELL_SUMMON_TEMPLAR_FIRE      = 24744,
	SPELL_SUMMON_TEMPLAR_THUNDER   = 24756,
	SPELL_SUMMON_TEMPLAR_STONE     = 24758,
	SPELL_SUMMON_TEMPLAR_WATER     = 24760,
	SPELL_SUMMON_TEMPLAR_RANDOM    = 24734,
	SPELL_SUMMON_DUKE_FIRE         = 24765,
	SPELL_SUMMON_DUKE_THUNDER      = 24768,
	SPELL_SUMMON_DUKE_STONE        = 24770,
	SPELL_SUMMON_DUKE_WATER        = 24772,
	SPELL_SUMMON_DUKE_RANDOM       = 24763,
	SPELL_SUMMON_ROYAL_FIRE        = 24786,
	SPELL_SUMMON_ROYAL_THUNDER     = 24788,
	SPELL_SUMMON_ROYAL_STONE       = 24789,
	SPELL_SUMMON_ROYAL_WATER       = 24790,
	SPELL_SUMMON_ROYAL_RANDOM      = 24784,
};

class go_wind_stone : public GameObjectScript
{
public:
    go_wind_stone() : GameObjectScript("go_wind_stone") { }

    bool IsLesserWindStone(GameObject* _GO)
    {
        return (_GO->GetEntry() == 180529 || _GO->GetEntry() == 180456 || _GO->GetEntry() == 180518 || _GO->GetEntry() == 180544 || _GO->GetEntry() == 180549 || _GO->GetEntry() == 180564);
    }

    bool IsNormalWindStone(GameObject* _GO)
    {
        return (_GO->GetEntry() == 180502 || _GO->GetEntry() == 180534 || _GO->GetEntry() == 180461 || _GO->GetEntry() == 180554);
    }

    bool IsGreaterWindStone(GameObject* _GO)
    {
        return (_GO->GetEntry() == 180466 || _GO->GetEntry() == 180539 || _GO->GetEntry() == 180559);
    }

    bool OnGossipHello(Player* player, GameObject* _GO)
    {
        if ( IsLesserWindStone(_GO) && player->HasAura(SPELL_TWILIGHT_LESSER,0))
        {
            player->ADD_GOSSIP_ITEM(0, GOSSIP_SUM_TEMPLAR_RANDOM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

            if (player->HasItemCount(ITEM_CREST_FIRE,1))
                player->ADD_GOSSIP_ITEM(0, GOSSIP_SUM_TEMPLAR_FIRE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+11);
            if (player->HasItemCount(ITEM_CREST_THUNDER,1))
                player->ADD_GOSSIP_ITEM(0, GOSSIP_SUM_TEMPLAR_THUNDER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+12);
            if (player->HasItemCount(ITEM_CREST_STONE,1))
                player->ADD_GOSSIP_ITEM(0, GOSSIP_SUM_TEMPLAR_STONE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+13);
            if (player->HasItemCount(ITEM_CREST_WATER,1))
                player->ADD_GOSSIP_ITEM(0, GOSSIP_SUM_TEMPLAR_WATER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+14);

            player->SEND_GOSSIP_MENU(_GO->GetGOInfo()->questgiver.gossipID, _GO->GetGUID());
        }
        else if (IsNormalWindStone(_GO) && player->HasAura(SPELL_TWILIGHT_NORMAL,0))
        {
            player->ADD_GOSSIP_ITEM(0, GOSSIP_SUM_DUKE_RANDOM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

            if (player->HasItemCount(ITEM_SIGNET_FIRE,1))
                player->ADD_GOSSIP_ITEM(0, GOSSIP_SUM_DUKE_FIRE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+21);
            if (player->HasItemCount(ITEM_SIGNET_THUNDER,1))
                player->ADD_GOSSIP_ITEM(0, GOSSIP_SUM_DUKE_THUNDER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+22);
            if (player->HasItemCount(ITEM_SIGNET_STONE,1))
                player->ADD_GOSSIP_ITEM(0, GOSSIP_SUM_DUKE_STONE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+23);
            if (player->HasItemCount(ITEM_SIGNET_WATER,1))
                player->ADD_GOSSIP_ITEM(0, GOSSIP_SUM_DUKE_WATER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+24);

            player->SEND_GOSSIP_MENU(_GO->GetGOInfo()->questgiver.gossipID, _GO->GetGUID());
        }
        else if (IsGreaterWindStone(_GO) && player->HasAura(SPELL_TWILIGHT_GREATER,0))
        {
            player->ADD_GOSSIP_ITEM(0, GOSSIP_SUM_ROYAL_RANDOM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);

            if (player->HasItemCount(ITEM_SCEPTER_FIRE,1))
                player->ADD_GOSSIP_ITEM(0, GOSSIP_SUM_ROYAL_FIRE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+31);
            if (player->HasItemCount(ITEM_SCEPTER_THUNDER,1))
                player->ADD_GOSSIP_ITEM(0, GOSSIP_SUM_ROYAL_THUNDER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+32);
            if (player->HasItemCount(ITEM_SCEPTER_STONE,1))
                player->ADD_GOSSIP_ITEM(0, GOSSIP_SUM_ROYAL_STONE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+33);
            if (player->HasItemCount(ITEM_SCEPTER_WATER,1))
                player->ADD_GOSSIP_ITEM(0, GOSSIP_SUM_ROYAL_WATER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+34);

            player->SEND_GOSSIP_MENU(_GO->GetGOInfo()->questgiver.gossipID, _GO->GetGUID());
        }
		else
        {
            // Cast Unknown Fire Spell
        }
        return true;
    }

    void SummonTemplar(Player* player, uint32 typ)
    {
        uint32 entry = 0;
        switch (typ)
        {
        case 0:
            entry = ENTRY_TEMPLAR_FIRE;
            break;
        case 1:
            entry = ENTRY_TEMPLAR_THUNDER;
            break;
        case 2:
            entry = ENTRY_TEMPLAR_STONE;
            break;
        case 3:
            entry = ENTRY_TEMPLAR_WATER;
            break;
        }

        float x,y,z;
        player->GetPosition(x,y,z);
        Creature* temp = player->SummonCreature(entry,x,y,z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
        temp->AI()->AttackStart(player);
    }

    void SummonDuke(Player* player, uint32 typ)
    {
        uint32 entry = 0;
        switch (typ)
        {
        case 0:
            entry = ENTRY_DUKE_FIRE;
            break;
        case 1:
            entry = ENTRY_DUKE_THUNDER;
            break;
        case 2:
            entry = ENTRY_DUKE_STONE;
            break;
        case 3:
            entry = ENTRY_DUKE_WATER;
            break;
        }

        float x,y,z;
        player->GetPosition(x,y,z);
        Creature* temp = player->SummonCreature(entry,x,y,z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
        temp->AI()->AttackStart(player);
    }

    void SummonRoyal(Player* player, uint32 typ)
    {
        uint32 entry = 0;
        switch (typ)
        {
        case 0:
            entry = ENTRY_ROYAL_FIRE;
            break;
        case 1:
            entry = ENTRY_ROYAL_THUNDER;
            break;
        case 2:
            entry = ENTRY_ROYAL_STONE;
            break;
        case 3:
            entry = ENTRY_ROYAL_WATER;
            break;
        }

        float x,y,z;
        player->GetPosition(x,y,z);
        Creature* temp = player->SummonCreature(entry,x,y,z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
        temp->AI()->AttackStart(player);
    }

    void SendActionMenu_go_wind_stone(Player* player, GameObject* _GO, uint32 action)
    {
        switch (action)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                player->CastSpell(player, SPELL_SUMMON_TEMPLAR_RANDOM,false);
                SummonTemplar(player,rand()%4);
                break;
            case GOSSIP_ACTION_INFO_DEF+11:
                player->CastSpell(player, SPELL_SUMMON_TEMPLAR_FIRE,false);
                SummonTemplar(player,0);
                break;
            case GOSSIP_ACTION_INFO_DEF+12:
                player->CastSpell(player, SPELL_SUMMON_TEMPLAR_THUNDER,false);
                SummonTemplar(player,1);
                break;
            case GOSSIP_ACTION_INFO_DEF+13:
                player->CastSpell(player, SPELL_SUMMON_TEMPLAR_STONE,false);
                SummonTemplar(player,2);
                break;
            case GOSSIP_ACTION_INFO_DEF+14:
                player->CastSpell(player, SPELL_SUMMON_TEMPLAR_WATER,false);
                SummonTemplar(player,3);
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                player->CastSpell(player, SPELL_SUMMON_DUKE_RANDOM,false);
                SummonDuke(player,rand()%4);
                break;
            case GOSSIP_ACTION_INFO_DEF+21:
                player->CastSpell(player, SPELL_SUMMON_DUKE_FIRE,false);
                SummonDuke(player,0);
                break;
            case GOSSIP_ACTION_INFO_DEF+22:
                player->CastSpell(player, SPELL_SUMMON_DUKE_THUNDER,false);
                SummonDuke(player,1);
                break;
            case GOSSIP_ACTION_INFO_DEF+23:
                player->CastSpell(player, SPELL_SUMMON_DUKE_STONE,false);
                SummonDuke(player,2);
                break;
            case GOSSIP_ACTION_INFO_DEF+24:
                player->CastSpell(player, SPELL_SUMMON_DUKE_WATER,false);
                SummonDuke(player,3);
                break;
            case GOSSIP_ACTION_INFO_DEF+3:
                player->CastSpell(player, SPELL_SUMMON_ROYAL_RANDOM,false);
                SummonRoyal(player,rand()%4);
                break;
            case GOSSIP_ACTION_INFO_DEF+31:
                player->CastSpell(player, SPELL_SUMMON_ROYAL_FIRE,false);
                SummonDuke(player,0);
                break;
            case GOSSIP_ACTION_INFO_DEF+32:
                player->CastSpell(player, SPELL_SUMMON_ROYAL_THUNDER,false);
                SummonDuke(player,1);
                break;
            case GOSSIP_ACTION_INFO_DEF+33:
                player->CastSpell(player, SPELL_SUMMON_ROYAL_STONE,false);
                SummonDuke(player,2);
                break;
            case GOSSIP_ACTION_INFO_DEF+34:
                player->CastSpell(player, SPELL_SUMMON_ROYAL_WATER,false);
                SummonDuke(player,3);
                break;
        }
    }

    bool OnGossipSelect(Player* player, GameObject* _GO, uint32 sender, uint32 action )
    {
        switch (sender)
        {
            case GOSSIP_SENDER_MAIN:
                SendActionMenu_go_wind_stone(player, _GO, action);
                player->CLOSE_GOSSIP_MENU();
                break;
        }
        return true;
    }
};

void AddSC_silithus()
{
    new go_crystalline_tear();
    new npc_anachronos_quest_trigger();
    new npc_anachronos_the_ancient();
    new mob_qiraj_war_spawn();
    new npc_highlord_demitrian();
    new npcs_rutgar_and_frankal();
    new go_wind_stone();
}