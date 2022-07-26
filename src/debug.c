#include "global.h"
#include "gflib.h"
#include "data.h"
#include "battle.h"
#include "constants/items.h"
#include "mail_data.h"
#include "pokemon_storage_system.h"
#include "event_data.h"
#include "random.h"
#include "constants/moves.h"
#include "menu.h"
#include "new_menu_helpers.h"
#include "script.h"
#include "strings.h"
#include "party_menu.h"
#include "list_menu.h"
#include "overworld.h"
#include "pokemon.h"
#include "decompress.h"
#include "constants/songs.h"
#include "text_window.h"
#include "trig.h"
#include "m4a.h"
#include "graphics.h"
#include "scanline_effect.h"
#include "naming_screen.h"
#include "help_system.h"
#include "field_fadetransition.h"
#include "trade.h"
#include "util.h"
#include "constants/species.h"
#include "constants/moves.h"

void TryChangeFirstMonData(void)
{
    struct Pokemon* mon = &gPlayerParty[0];
    
    mon->box.moves[0] = MOVE_LIGHT_SCREEN;
    //mon->box.hiddenAbility = TRUE;
    CalculateMonStats(mon);

}