#include "Chat.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "DatabaseEnv.h"
#include "ScriptLoader.h"
#include "ScriptedCreature.h"
#include "CreatureAI.h"

#define JOIN_QUE              "Join que"
#define LEAVE_QUE             "Leave que"
#define CURRENT_STATUS        "Que status"
#define GET_CLASS             "Get class"
#define MENU_ID               123

enum Options {
    JOIN_QUE_ACTION = 1,
    LEAVE_QUE_ACTION = 2,
    QUE_STATUS_ACTION = 3,
    GET_CLASS_ACTION = 4
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

int index = 0;

class FirstLogin : public PlayerScript {
public:
    FirstLogin() : PlayerScript("FirstLogin") {}

    void OnLogin(Player* player, bool firstLogin) override {
        auto result = CharacterDatabase.PQuery("SELECT gossip_menu FROM characters WHERE guid=49");
        bool value;
        if(result) {
            value = result->Fetch()[0].GetUInt8();
        }

        if(value) {
            player->Say("inside", LANG_UNIVERSAL);
            if(player->GetClass() == 9) {
                player->Say("Not sending gossip", LANG_UNIVERSAL);
            } else {
                SendGossipMenu(player);
            }
        }
    }

    void SendGossipMenu(Player* player) {
        player->PlayerTalkClass->GetGossipMenu().SetMenuId(MENU_ID);

        // Join que gossip
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, JOIN_QUE, GOSSIP_SENDER_MAIN, JOIN_QUE_ACTION);
        SendGossipMenuFor(player, JOIN_QUE_ACTION, player->GetGUID());

        // Leave que gossip
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, LEAVE_QUE, GOSSIP_SENDER_MAIN, LEAVE_QUE_ACTION);
        SendGossipMenuFor(player, LEAVE_QUE_ACTION, player->GetGUID());

        // Get current status
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, CURRENT_STATUS, GOSSIP_SENDER_MAIN, QUE_STATUS_ACTION);
        SendGossipMenuFor(player, QUE_STATUS_ACTION, player->GetGUID());

        // Get class
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, GET_CLASS, GOSSIP_SENDER_MAIN, GET_CLASS_ACTION);
        SendGossipMenuFor(player, GET_CLASS_ACTION, player->GetGUID());
    }

    void OnGossipSelect(Player* player, uint32 menu_id, uint32 /*sender*/, uint32 action) override {
        if(menu_id != MENU_ID) { // Not the menu coded here? stop.
            return;
        }

        ClearGossipMenuFor(player);
        switch(action) {
        case JOIN_QUE_ACTION:
            index++;
            player->Say(JOIN_QUE, LANG_UNIVERSAL);
            ClearGossipMenuFor(player);
            CloseGossipMenuFor(player);
            if(index < 10) {
                SendGossipMenu(player);
            }
            break;
        case LEAVE_QUE_ACTION:
            index++;
            player->Say(LEAVE_QUE, LANG_UNIVERSAL);
            ClearGossipMenuFor(player);
            CloseGossipMenuFor(player);
            if(index < 10) {
                SendGossipMenu(player);
            }
            break;
        case QUE_STATUS_ACTION:
            index++;
            player->Say(CURRENT_STATUS, LANG_UNIVERSAL);
            player->Say(std::to_string(index), LANG_UNIVERSAL);
            ClearGossipMenuFor(player);
            CloseGossipMenuFor(player);
            if(index < 10) {
                SendGossipMenu(player);
            }
            break;
        case GET_CLASS_ACTION:
            player->Say(std::to_string(player->GetClass()), LANG_UNIVERSAL);
            ClearGossipMenuFor(player);
            CloseGossipMenuFor(player);
            break;
        }
    }
};

void AddSC_FirstLogin() {
    new FirstLogin();
}
