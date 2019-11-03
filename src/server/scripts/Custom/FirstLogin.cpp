#include "Chat.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "DatabaseEnv.h"
#include "ScriptLoader.h"
#include "ScriptedCreature.h"
#include "CreatureAI.h"
#include <iostream>

#define JOIN_QUE              "Join que"
#define LEAVE_QUE             "Leave que"
#define CURRENT_STATUS        "Que status"
#define GET_CLASS             "Get class"
#define HEALING_GEAR          "Choose Healing gear"
#define DPS_GEAR              "Choose DPS gear"
#define TANK_GEAR             "Choose Tanking gear"

#define MENU_ID               2522

enum Options {
    HEALING_ACTION = 1,
    DPS_ACTION = 2,
    TANK_ACTION = 3,
    JOIN_QUE_ACTION = 5,
    LEAVE_QUE_ACTION = 6,
    QUE_STATUS_ACTION = 7,
    GET_CLASS_ACTION = 8
};

/*
Classes uint8 values:

    CLASS_NONE = 0, // SKIP
    CLASS_WARRIOR = 1, // TITLE Warrior
    CLASS_PALADIN = 2, // TITLE Paladin
    CLASS_HUNTER = 3, // TITLE Hunter
    CLASS_ROGUE = 4, // TITLE Rogue
    CLASS_PRIEST = 5, // TITLE Priest
    CLASS_DEATH_KNIGHT = 6, // TITLE Death Knight
    CLASS_SHAMAN = 7, // TITLE Shaman
    CLASS_MAGE = 8, // TITLE Mage
    CLASS_WARLOCK = 9, // TITLE Warlock
    CLASS_DRUID = 11 // TITLE Druid
*/

/*
HEALER/TANK/DPS:
paladin - 2
druid - 11

HEALER/DPS:
priest - 5
shaman - 7

TANK/DPS:
warrior - 1
death knight - 6

DPS:
mage
rogue
hunter
warlock
*/

class FirstLogin : public PlayerScript {
private:
    std::string DB_NAME = "gossip_menu";
    std::string DB_TABLE = "characters";
public:
    FirstLogin() : PlayerScript("FirstLogin") {}

    void OnLogin(Player* player, bool firstLogin) override {
        //auto result = CharacterDatabase.PQuery("SELECT column FROM table WHERE guid=%u", player->GetGUID().GetCounter());
        //auto result = CharacterDatabase.PQuery("SELECT %s FROM %t WHERE guid=%p", "gossip_menu", "characters", player->GetGUID().GetCounter());
        //auto result = CharacterDatabase.PQuery("SELECT %s FROM characters WHERE guid=%u", "gossip_menu", player->GetGUID().GetCounter());
        std::cout << "SELECT %s FROM characters WHERE guid=%u", "gossip_menu", std::to_string(player->GetGUID().GetCounter());
        auto result = CharacterDatabase.PQuery("SELECT " +DB_NAME +" FROM " +DB_TABLE +" WHERE guid=" +std::to_string(player->GetGUID().GetCounter()));
        bool value;
        if(result) {
            value = result->Fetch()[0].GetUInt8();
        }
        player->Say(std::to_string(value), LANG_UNIVERSAL);   

        if(value) {
            uint8 playerClass = player->GetClass();

            if(playerClass == 2 || playerClass == 11) {
                SendGossipMenuDHT(player);
                SetGossipMenuFalse(player);
            } else if(playerClass == 5 || playerClass == 7) {
                SendGossipMenuHD(player);
                SetGossipMenuFalse(player);
            } else if(playerClass == 1 || playerClass == 6) {
                SendGossipMenuTD(player);
                SetGossipMenuFalse(player);
            }
        }
    }

    void SendGossipMenuDHT(Player* player) {
        player->PlayerTalkClass->GetGossipMenu().SetMenuId(MENU_ID);
        ClearGossipMenuFor(player);

        // DPS gear
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, DPS_GEAR, GOSSIP_SENDER_MAIN, DPS_ACTION);
        SendGossipMenuFor(player, DPS_ACTION, player->GetGUID());

        // Healing gear
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, HEALING_GEAR, GOSSIP_SENDER_MAIN, HEALING_ACTION);
        SendGossipMenuFor(player, HEALING_ACTION, player->GetGUID());

        // Tank gear
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, TANK_GEAR, GOSSIP_SENDER_MAIN, TANK_ACTION);
        SendGossipMenuFor(player, TANK_ACTION, player->GetGUID());
    }

    void SendGossipMenuHD(Player* player) {
        player->PlayerTalkClass->GetGossipMenu().SetMenuId(MENU_ID);
        ClearGossipMenuFor(player);

        // DPS gear
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, DPS_GEAR, GOSSIP_SENDER_MAIN, DPS_ACTION);
        SendGossipMenuFor(player, DPS_ACTION, player->GetGUID());

        // Healing gear
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, HEALING_GEAR, GOSSIP_SENDER_MAIN, HEALING_ACTION);
        SendGossipMenuFor(player, HEALING_ACTION, player->GetGUID());
    }

    void SendGossipMenuTD(Player* player) {
        player->PlayerTalkClass->GetGossipMenu().SetMenuId(MENU_ID);
        ClearGossipMenuFor(player);
        // DPS gear
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, DPS_GEAR, GOSSIP_SENDER_MAIN, DPS_ACTION);
        SendGossipMenuFor(player, DPS_ACTION, player->GetGUID());

        // Tank gear
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, TANK_GEAR, GOSSIP_SENDER_MAIN, TANK_ACTION);
        SendGossipMenuFor(player, TANK_ACTION, player->GetGUID());
    }

    void SetGossipMenuFalse(Player* player) {
        CharacterDatabase.PQuery("UPDATE " +DB_TABLE +" SET " +DB_NAME +"=0 WHERE guid = " +std::to_string(player->GetGUID()));
        
    }

    void OnGossipSelect(Player* player, uint32 menu_id, uint32 /*sender*/, uint32 action) override {
        if(menu_id != MENU_ID) { // Not the menu coded here? stop.
            return;
        }

        ClearGossipMenuFor(player);
        switch(action) {
        case DPS_ACTION:
            ClearGossipMenuFor(player);
            CloseGossipMenuFor(player);

            break;
        case HEALING_ACTION:
            ClearGossipMenuFor(player);
            CloseGossipMenuFor(player);
            break;
        case TANK_ACTION:
            ClearGossipMenuFor(player);
            CloseGossipMenuFor(player);
            break;
        }
    }
};

void AddSC_FirstLogin() {
    new FirstLogin();
}
