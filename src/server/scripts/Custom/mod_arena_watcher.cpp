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
#include "CreatureTextMgr.h"
#include "BattlegroundMgr.h"
#include "Battleground.h"
#include "ArenaTeamMgr.h"
#include "ArenaTeam.h"
#include "Player.h"
#include "Config.h"
#include "../../../scripts/QuantumCoreScripts/SystemScripts/world_fun_system.h"

enum WatcherData
{
    GOSSIP_OFFSET = GOSSIP_ACTION_INFO_DEF + 10,
};

enum WatcherTexts
{
    SAY_NOT_FOUND_BRACKET     = 0,
    SAY_ARENA_NOT_IN_PROGRESS = 1,
    SAY_TARGET_NOT_IN_WORLD   = 2,
    SAY_TARGET_NOT_IN_ARENA   = 3,
    SAY_TARGET_IS_GM          = 4,
};

enum WatcherStrings
{
    STRING_ARENA_2v2 = 15200,
    STRING_ARENA_3v3 = 15201,
    STRING_ARENA_5v5 = 15202,
    STRING_FOLLOW    = 15203,
};

bool ArenaWatcherEnable = false;
bool ArenaWatcherOnlyGM = false;
bool ArenaWatcherOnlyRated = false;
bool ArenaWatcherToPlayers = false;
bool ArenaWatcherSilence = false;
bool ArenaWatcherFly = false;
float ArenaWatcherSpeed = 3.0f;

struct ArenaWatcher
{
    time_t mutetime;
};

typedef std::map<uint32, ArenaWatcher> ArenaWatcherMap;
ArenaWatcherMap ArenaWatcherPlayers;

bool IsWatcher(uint32 guid)
{
    ArenaWatcherMap::iterator itr = ArenaWatcherPlayers.find(guid);
    if (itr != ArenaWatcherPlayers.end())
        return true;
    return false;
}

void ArenaWatcherStart(Player* player)
{
    player->SetGMVisible(false);
    uint32 guid = player->GetGUIDLow();

    if (IsWatcher(guid))
        return;

    ArenaWatcher data;
    data.mutetime = player->GetSession()->m_muteTime;
    ArenaWatcherPlayers[guid] = data;
}

void ArenaWatcherAfterTeleport(Player* player)
{
    player->SetMovement(MOVE_WATER_WALK);
    player->SetMovement(MOVE_UNROOT);

    if (ArenaWatcherSilence)
        player->GetSession()->m_muteTime = time(NULL) + 120 * MINUTE;

    if (ArenaWatcherFly)
    {
        player->SendMovementSetCanFly(true);
        player->SetCanFly(true);
    }

    player->SetSpeed(MOVE_WALK, ArenaWatcherSpeed, true);
    player->SetSpeed(MOVE_RUN, ArenaWatcherSpeed, true);
    player->SetSpeed(MOVE_SWIM, ArenaWatcherSpeed, true);
    player->SetSpeed(MOVE_FLIGHT, ArenaWatcherSpeed, true);
    player->setDeathState(CORPSE);
}

void ArenaWatcherEnd(Player* player)
{
    uint32 guid = player->GetGUIDLow();

    if (!IsWatcher(guid))
        return;

    if (ArenaWatcherSilence)
        player->GetSession()->m_muteTime = ArenaWatcherPlayers[guid].mutetime;


    ArenaWatcherMap::iterator itr = ArenaWatcherPlayers.find(guid);
    if (itr != ArenaWatcherPlayers.end())
    {
        ArenaWatcherPlayers.erase(itr);
        player->ResurrectPlayer(100.0f, false);
        player->SetGMVisible(true);
        player->SetGameMaster(false);
        player->SetAcceptWhispers(true);
        player->SendMovementSetCanFly(false);
        player->SetCanFly(false);
        player->SetSpeed(MOVE_WALK, 1.0f, true);
        player->SetSpeed(MOVE_RUN, 1.0f, true);
        player->SetSpeed(MOVE_SWIM, 1.0f, true);
        player->SetSpeed(MOVE_FLIGHT, 1.0f, true);
    }
}

class mod_ArenaWatcher_WorldScript : public WorldScript
{
public:
	mod_ArenaWatcher_WorldScript() : WorldScript("mod_ArenaWatcher_WorldScript") { }

    void OnConfigLoad(bool reload)
    {
        ArenaWatcherEnable = GetConfigSettings::GetBoolState("ArenaWatcher.Enable", false);
        ArenaWatcherOnlyGM = GetConfigSettings::GetBoolState("ArenaWatcher.Only.GM", false);
        ArenaWatcherOnlyRated = GetConfigSettings::GetBoolState("ArenaWatcher.Only.Rated", false);
        ArenaWatcherToPlayers = GetConfigSettings::GetBoolState("ArenaWatcher.To.Players", false);
        ArenaWatcherSilence = GetConfigSettings::GetBoolState("ArenaWatcher.Silence", false);
        ArenaWatcherFly = GetConfigSettings::GetBoolState("ArenaWatcher.Fly", false);
        ArenaWatcherSpeed = GetConfigSettings::GetFloatDefault("ArenaWatcher.Speed", 3.0f);

        if (!reload)
            ArenaWatcherPlayers.clear();
    }
};

class mod_ArenaWatcher_PlayerScript : public PlayerScript
{
public:
	mod_ArenaWatcher_PlayerScript() : PlayerScript("mod_ArenaWatcher_PlayerScript") { }

    void OnPlayerRemoveFromBattleground(Player* player, Battleground* /*bg*/)
    {
        if (!ArenaWatcherEnable)
            return;

        ArenaWatcherEnd(player);
    }

    void OnLogout(Player* player)
    {
        if (!ArenaWatcherEnable)
            return;

        ArenaWatcherEnd(player);
    }
};

class npc_arena_watcher : public CreatureScript
{
public:
	npc_arena_watcher() : CreatureScript("npc_arena_watcher") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (ArenaWatcherEnable && (!ArenaWatcherOnlyGM || player->IsGameMaster()))
        {
            uint8 arenasCount[MAX_ARENA_SLOT] = {0, 0, 0};

            for (uint8 bgTypeId = 0; bgTypeId < MAX_BATTLEGROUND_TYPE_ID; ++bgTypeId)
            {
                if (!BattlegroundMgr::IsArenaType(BattlegroundTypeId(bgTypeId)))
                    continue;

                BattlegroundSet arenas = sBattlegroundMgr->GetAllBattlegroundsWithTypeId(BattlegroundTypeId(bgTypeId));

				if (arenas.empty())
                    continue;

				for (BattlegroundSet::const_iterator itr = arenas.begin(); itr != arenas.end(); ++itr)
                {
                    if (!(itr->second->GetStatus() & 2))
                        continue;

                    if (ArenaWatcherOnlyRated && !itr->second->isRated())
                        continue;

                    ++arenasCount[ArenaTeam::GetSlotByType(itr->second->GetArenaType())];
                }
            }

            for (uint8 i = 0; i < MAX_ARENA_SLOT; ++i)
            {
                if (!arenasCount[i])
                    continue;

                char gossipTextFormat[50];
                snprintf(gossipTextFormat, 50, sObjectMgr->GetQuantumSystemTextForDBCLocale(STRING_ARENA_2v2 + i), arenasCount[i]);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossipTextFormat, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + ArenaTeam::GetTypeBySlot(i));
            }

            if (ArenaWatcherToPlayers)
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, sObjectMgr->GetQuantumSystemTextForDBCLocale(STRING_FOLLOW), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4, "", 0, true);
        }

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();

        if (!ArenaWatcherEnable && (!ArenaWatcherOnlyGM || player->IsGameMaster()))
            return true;

        if (action <= GOSSIP_OFFSET)
        {
            bool bracketExists = false;

            uint8 playerCount = action - GOSSIP_ACTION_INFO_DEF;

            for (uint8 bgTypeId = 0; bgTypeId < MAX_BATTLEGROUND_TYPE_ID; ++bgTypeId)
            {
                if (!BattlegroundMgr::IsArenaType(BattlegroundTypeId(bgTypeId)))
                    continue;

                BattlegroundSet arenas = sBattlegroundMgr->GetAllBattlegroundsWithTypeId(BattlegroundTypeId(bgTypeId));

                if (arenas.empty())
                    continue;

				for (BattlegroundSet::const_iterator itr = arenas.begin(); itr != arenas.end(); ++itr)
                {
                    Map* map = itr->second->FindBgMap();
                    if (!map)
                        continue;

                    if (!(itr->second->GetStatus() & 2)) {
                        continue;
                    }

                    if (itr->second->GetArenaType() != playerCount)
                        continue;

                    if (ArenaWatcherOnlyRated && !itr->second->isRated())
                        continue;

                    if (itr->second->isRated())
                    {
                        ArenaTeam* teamOne = sArenaTeamMgr->GetArenaTeamById(itr->second->GetArenaTeamIdByIndex(0));
                        ArenaTeam* teamTwo = sArenaTeamMgr->GetArenaTeamById(itr->second->GetArenaTeamIdByIndex(1));

                        if (teamOne && teamTwo)
                        {
                            char gossipTextFormat[100];
                            snprintf(gossipTextFormat, 100, "%s : %s (%u) vs. %s (%u)", map->GetMapName(), teamOne->GetName().c_str(), teamOne->GetRating(), teamTwo->GetName().c_str(), teamTwo->GetRating());
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossipTextFormat, GOSSIP_SENDER_MAIN + bgTypeId, itr->first + GOSSIP_OFFSET);
                        }
                    }
                    else
                    {
                        char gossipTextFormat[100];
                        snprintf(gossipTextFormat, 100, "[%u] %s : %u vs. %u", itr->first, map->GetMapName(), playerCount, playerCount);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossipTextFormat, GOSSIP_SENDER_MAIN + bgTypeId, itr->first + GOSSIP_OFFSET);
                    }

                    bracketExists = true;
                }
            }

            if (bracketExists)
                player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            else
            {
                sCreatureTextMgr->SendChat(creature, SAY_NOT_FOUND_BRACKET, player->GetGUID());
                player->PlayerTalkClass->ClearMenus();
                player->CLOSE_GOSSIP_MENU();
            }
        }
        else
        {
            uint32 arenaId = action - GOSSIP_OFFSET;
            uint32 bgTypeId = sender - GOSSIP_SENDER_MAIN;
            BattlegroundSet arenas = sBattlegroundMgr->GetAllBattlegroundsWithTypeId(BattlegroundTypeId(bgTypeId));

            if (arenas.empty())
                return false;

			if (arenas[arenaId])
            {
				Battleground* bg = arenas[arenaId];

                if (bg->GetStatus() == STATUS_NONE)
                {
                    sCreatureTextMgr->SendChat(creature, SAY_ARENA_NOT_IN_PROGRESS, player->GetGUID());
                    player->PlayerTalkClass->ClearMenus();
                    player->CLOSE_GOSSIP_MENU();
                    return false;
                }

                float x = 0.0f, y = 0.0f, z = 0.0f;
                switch (bg->GetMapId())
                {
                    case 617:
                        x = 1299.046f;
                        y = 784.825f;
                        z = 9.338f;
                        break;
                    case 618:
                        x = 763.5f;
                        y = -284;
                        z = 28.276f;
                        break;
                    case 572:
                        x = 1285.810547f;
                        y = 1667.896851f;
                        z = 39.957642f;
                        break;
                    case 562:
                        x = 6238.930176f;
                        y = 262.963470f;
                        z = 0.889519f;
                        break;
                    case 559:
                        x = 4055.504395f;
                        y = 2919.660645f;
                        z = 13.611241f;
                        break;
                    default:
                        player->PlayerTalkClass->ClearMenus();
                        player->CLOSE_GOSSIP_MENU();
                        return false;
                }
                player->SetBattlegroundId(bg->GetInstanceID(), bg->GetTypeID());
                player->SetBattlegroundEntryPoint();
                ArenaWatcherStart(player);
                player->TeleportTo(bg->GetMapId(), x, y, z, player->GetOrientation());
                ArenaWatcherAfterTeleport(player);
            }
        }
        return true;
    }

    bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, const char* targetName)
    {
        player->PlayerTalkClass->ClearMenus();
        player->CLOSE_GOSSIP_MENU();

        if (!ArenaWatcherToPlayers || !ArenaWatcherEnable || (ArenaWatcherOnlyGM && !player->IsGameMaster()) || !*targetName)
            return true;

        if (sender == GOSSIP_SENDER_MAIN)
        {
            switch (action)
            {
                case GOSSIP_ACTION_INFO_DEF + 4:
                {
                    if (Player* target = sObjectAccessor->FindPlayerByName(targetName))
                    {
                        if (!target->IsInWorld())
                            sCreatureTextMgr->SendChat(creature, SAY_TARGET_NOT_IN_WORLD, player->GetGUID());
                        else if (!target->InArena())
                            sCreatureTextMgr->SendChat(creature, SAY_TARGET_NOT_IN_ARENA, player->GetGUID());
                        else if (target->IsGameMaster())
                            sCreatureTextMgr->SendChat(creature, SAY_TARGET_IS_GM, player->GetGUID());
                        else
                        {
                            if (Battleground* bg = target->GetBattleground())
                            {
                                player->SetBattlegroundId(bg->GetInstanceID(), bg->GetTypeID());
                                player->SetBattlegroundEntryPoint();
                                float x, y, z;
                                target->GetContactPoint(player, x, y, z);
                                ArenaWatcherStart(player);
                                player->TeleportTo(target->GetMapId(), x, y, z, player->GetOrientation());
                                player->SetInFront(target);
                                ArenaWatcherAfterTeleport(player);
                            }
                        }
                    }
                    return true;
                }
            }
        }

        return false;
    }

	struct npc_arena_watcherAI : public QuantumBasicAI
    {
        npc_arena_watcherAI(Creature* creature) : QuantumBasicAI(creature){}

		uint32 BuffTimer;

        void Reset()
        {
			BuffTimer = 0.5*IN_MILLISECONDS;

			me->SetUInt32Value(UNIT_FIELD_BYTES_2, FIELD_BYTE_2_MELEE_WEAPON);
        }

        void UpdateAI(uint32 const diff)
        {
			if (!me->IsInCombatActive())
			{
				if (!me->HasAuraEffect(SPELL_CASCADE_OF_ROSES, 0))
					DoCast(me, SPELL_CASCADE_OF_ROSES, true);
			}

			if (BuffTimer <= diff && !me->IsInCombatActive())
			{
				DoCast(me, SPELL_BLESSING_OF_KINGS, true);
				DoCast(me, SPELL_THORNS, true);
				DoCast(me, SPELL_MARK_OF_THE_WILD, true);
				DoCast(me, SPELL_FLASK_OF_PURE_MOJO, true);
				BuffTimer = 10*MINUTE*IN_MILLISECONDS;
			}
			else BuffTimer -= diff;

            if (!UpdateVictim())
                return;

			DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_arena_watcherAI(creature);
    }
};

void AddSC_mod_arena_watcher()
{
    new mod_ArenaWatcher_WorldScript();
    new mod_ArenaWatcher_PlayerScript();
    new npc_arena_watcher();
}