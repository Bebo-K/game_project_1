export module LocalPlayer;

import Player;
import Array;
import ClientEntity;


namespace  LocalPlayer {

    //Cache
    export struct Cache {
        int             my_slot_id;
        int             my_save_id;
        Array<Player>   players;
        int             current_players;

        Cache():players() {
            my_slot_id=-1;
            my_save_id=0;
            current_players=0;
        }
        ~Cache() {
            for(int i=0;i<players.length;i++){players[i]->Clear();}
        }
    };

    Cache local_player_cache = Cache();

    export Cache* GetCache() {
        return &local_player_cache;
    }

    export Player* Me() {
        if(local_player_cache.my_slot_id<0)return nullptr;
        return local_player_cache.players[local_player_cache.my_slot_id];
    }

    export bool IsPlayerEntity(ClientEntity* entity) {
        Player* me = Me();
        if (!me)return false;
        return me->entity_id == entity->entity_id;
    }

}