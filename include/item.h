#ifndef GUARD_ITEM_H
#define GUARD_ITEM_H

#include "global.h"

typedef void (*ItemUseFunc)(u8);

struct Item
{
    u8 name[ITEM_NAME_LENGTH];
    u16 itemId;
    u16 price;
    u8 holdEffect;
    u8 holdEffectParam;
    const u8 *description;
    u8 importance;
    u8 exitsBagOnUse;
    u8 pocket;
    u8 type;
    ItemUseFunc fieldUseFunc;
    u8 battleUsage;
    ItemUseFunc battleUseFunc;
    u8 secondaryId;
};

struct BagPocket
{
    struct ItemSlot *itemSlots;
    u8 capacity;
};

extern const struct Item gItems[];
enum ChoiceQualities
{
	QUALITY_CHOICE_BAND,	//0
	QUALITY_CHOICE_SPECS,	//1
	QUALITY_CHOICE_SCARF,	//2
};


enum BallTypes
{
	BALL_TYPE_MASTER_BALL,	//0
	BALL_TYPE_ULTRA_BALL,	//1
	BALL_TYPE_GREAT_BALL,	//2
	BALL_TYPE_POKE_BALL,	//3
	BALL_TYPE_SAFARI_BALL,	//4
	BALL_TYPE_NET_BALL,		//5
	BALL_TYPE_DIVE_BALL,	//6
	BALL_TYPE_NEST_BALL,	//7
	BALL_TYPE_REPEAT_BALL,	//8
	BALL_TYPE_TIMER_BALL,	//9
	BALL_TYPE_LUXURY_BALL,	//10
	BALL_TYPE_PREMIER_BALL,	//11
	BALL_TYPE_DUSK_BALL,	//12
	BALL_TYPE_HEAL_BALL,	//13
	BALL_TYPE_QUICK_BALL,	//14
	BALL_TYPE_CHERISH_BALL,	//15
	BALL_TYPE_PARK_BALL,	//16
	BALL_TYPE_FAST_BALL,	//17
	BALL_TYPE_LEVEL_BALL,	//18
	BALL_TYPE_LURE_BALL,	//19
	BALL_TYPE_HEAVY_BALL,	//20
	BALL_TYPE_LOVE_BALL,	//21
	BALL_TYPE_FRIEND_BALL,	//22
	BALL_TYPE_MOON_BALL,	//23
	BALL_TYPE_SPORT_BALL,	//24
	BALL_TYPE_BEAST_BALL,	//25
	BALL_TYPE_DREAM_BALL,	//26
	BALL_TYPE_DYNAMAX_BALL, //27 - Don't give to player!

	BALL_TYPE_RANDOM = 0xFF,
};

enum ItemType //Sorted in this order
{
	ITEM_TYPE_FIELD_USE,
	ITEM_TYPE_HEALTH_RECOVERY,
	ITEM_TYPE_STATUS_RECOVERY,
	ITEM_TYPE_PP_RECOVERY,
	ITEM_TYPE_STAT_BOOST_DRINK,
	ITEM_TYPE_STAT_BOOST_WING,
	ITEM_TYPE_EVOLUTION_STONE,
	ITEM_TYPE_EVOLUTION_ITEM,
	ITEM_TYPE_BATTLE_ITEM,
	ITEM_TYPE_FLUTE,
	ITEM_TYPE_STAT_BOOST_HELD_ITEM,
	ITEM_TYPE_HELD_ITEM,
	ITEM_TYPE_GEM,
	ITEM_TYPE_PLATE,
	ITEM_TYPE_MEMORY,
	ITEM_TYPE_DRIVE,
	ITEM_TYPE_INCENSE,
	ITEM_TYPE_MEGA_STONE,
	ITEM_TYPE_Z_CRYSTAL,
	ITEM_TYPE_NECTAR,
	ITEM_TYPE_SELLABLE,
	ITEM_TYPE_RELIC,
	ITEM_TYPE_SHARD,
	ITEM_TYPE_FOSSIL,
	ITEM_TYPE_MAIL,
};

//Make sure these two values are always correct!
#define BALL_TYPE_MASTER_BALL 0
#define BALL_TYPE_POKE_BALL 3
#define BALL_TYPE_DYNAMAX_BALL 27

extern struct BagPocket gBagPockets[];

void GetBerryCountString(u8* dst, const u8* berryName, u32 quantity);
void CopyItemName(u16 itemId, u8 *string);
void CopyItemNameHandlePlural(u16 itemId, u8 *string, u32 quantity);
bool8 IsBagPocketNonEmpty(u8 pocket);
bool8 CheckBagHasItem(u16 itemId, u16 count);
bool8 CheckBagHasSpace(u16 itemId, u16 count);
bool8 RemoveBagItem(u16 itemId, u16 count);
u8 GetPocketByItemId(u16 itemId);
void ClearItemSlots(struct ItemSlot *itemSlots, u8 b);
u8 CountUsedPCItemSlots(void);
bool8 CheckPCHasItem(u16 itemId, u16 count);
bool8 AddPCItem(u16 itemId, u16 count);
void SwapRegisteredBike(void);
const u8 *ItemId_GetName(u16 itemId);
u16 ItemId_GetId(u16 itemId);
u16 ItemId_GetPrice(u16 itemId);
u8 ItemId_GetHoldEffect(u16 itemId);
u8 ItemId_GetHoldEffectParam(u16 itemId);
const u8 *ItemId_GetDescription(u16 itemId);
bool32 ItemId_CopyDescription(u8 *a, u32 itemId, u32 c);
u8 ItemId_GetImportance(u16 itemId);
u8 ItemId_GetUnknownValue(u16 itemId);
u8 ItemId_GetPocket(u16 itemId);
u8 ItemId_GetType(u16 itemId);
ItemUseFunc ItemId_GetFieldFunc(u16 itemId);
u8 ItemId_GetBattleUsage(u16 itemId);
ItemUseFunc ItemId_GetBattleFunc(u16 itemId);
u8 ItemId_GetSecondaryId(u16 itemId);
u16 itemid_get_market_price(u16 itemId);
void ClearBag(void);
void ClearPCItemSlots(void);
void TrySetObtainedItemQuestLogEvent(u16 itemId);
bool8 AddBagItem(u16 itemId, u16 amount);
u8 itemid_get_x19(u16 itemId);

void SortPocketAndPlaceHMsFirst(struct BagPocket * pocket);
u16 BagGetItemIdByPocketPosition(u8 pocketId, u16 itemId);
u16 BagGetQuantityByPocketPosition(u8 pocketId, u16 itemId);
u16 BagGetQuantityByItemId(u16 item);
bool8 itemid_is_unique(u16 itemId);
void BagPocketCompaction(struct ItemSlot * slots, u8 capacity);
u16 GetPcItemQuantity(u16 *);
void SetBagPocketsPointers(void);

void ItemPcCompaction(void);
void RemovePCItem(u16 itemId, u16 quantity);
void SortAndCompactBagPocket(struct BagPocket * pocket);
u8 CountItemsInPC(void);
void ApplyNewEncryptionKeyToBagItems_(u32 newKey);
bool8 HasAtLeastOneBerry(void);

bool8 IsBerry(u16 item);
bool8 IsMegaStone(u16 item);
bool8 IsPrimalOrb(u16 item);
bool8 IsZCrystal(u16 item);
bool8 IsTypeZCrystal(u16 item, u8 moveType);

#endif // GUARD_ITEM_H
