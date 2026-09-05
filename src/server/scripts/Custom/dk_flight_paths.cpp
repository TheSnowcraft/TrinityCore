#include "ScriptMgr.h"
#include "Player.h"
#include "DBCStores.h"

class CustomMapFlightPathsDK : public PlayerScript
{
public:
    CustomMapFlightPathsDK() : PlayerScript("CustomMapFlightPathsDK") {}

    void OnLogin(Player* player, bool /*firstLogin*/) override
    {
        UnlockCustomFlightPaths(player);
    }

    void OnUpdateZone(Player* player, uint32 /*newZone*/, uint32 /*newArea*/) override
    {
        UnlockCustomFlightPaths(player);
    }

private:
    void UnlockCustomFlightPaths(Player* player)
    {
        // 1. Fast Skip: Only process Death Knights
        if (player->GetClass() != CLASS_DEATH_KNIGHT)
            return;

        // CHANGE THIS: Your exact custom Map ID
        uint32 const targetMapId = 726;

        // 2. Fast Skip: Only process if physically on the custom map
        if (player->GetMapId() != targetMapId)
            return;

        // 3. Quest Requirement Placeholder
        bool const requireQuest = false;
        uint32 const targetQuestId = 12345;

        if (requireQuest)
        {
            if (player->GetQuestStatus(targetQuestId) != QUEST_STATUS_REWARDED)
                return;
        }

        TeamId team = player->GetTeamId();

        // Safe loop layout using the proper TrinityCore 3.3.5a lookup format
        for (uint32 i = 0; i < sTaxiNodesStore.GetNumRows(); ++i)
        {
            TaxiNodesEntry const* node = sTaxiNodesStore.LookupEntry(i);
            if (!node)
                continue;

            // FIX: Changed node->map_id to node->ContinentID
            if (node->ContinentID == targetMapId)
            {
                // Faction filtering logic
                if (team == TEAM_HORDE && node->MountCreatureID[0] == 0)
                    continue;

                if (team == TEAM_ALLIANCE && node->MountCreatureID[1] == 0)
                    continue;

                // Directly sets the bitmask natively in memory
                if (!player->m_taxi.IsTaximaskNodeKnown(node->ID))
                {
                    player->m_taxi.SetTaximaskNode(node->ID);
                }
            }
        }
    }
};

void AddSC_CustomMapFlightPathsDK()
{
    new CustomMapFlightPathsDK();
}
