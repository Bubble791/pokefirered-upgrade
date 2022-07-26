#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_scripts.h"
#include "battle_string_ids.h"
#include "battle_controllers.h"
#include "battle_message.h"
#include "util.h"
#include "item.h"
#include "random.h"
#include "pokemon.h"
#include "battle_ai_script_commands.h"
#include "menu.h"
#include "malloc.h"
#include "string_util.h"
#include "window.h"
#include "decompress.h"
#include "sound.h"
#include "constants/abilities.h"
#include "constants/battle_ai.h"
#include "constants/battle_move_effects.h"
#include "constants/hold_effects.h"
#include "constants/moves.h"
#include "constants/pokemon.h"

static const u8 ElectricTerrainSetString[] = _("An electric current runs across\nthe battlefield!");
static const u8 GrassyTerrainSetString[] = _("Grass grew to cover the\nbattlefield!");
static const u8 MistyTerrainSetString[] = _("Mist swirls around the\nbattlefield!");
static const u8 PsychicTerrainSetString[] = _("The battlefield got weird!");

const u16 gWeatherAbilityStrings[] =
{
	STRINGID_STARTEDTORAIN, 
	STRINGID_SANDSTORMBREWED, 
	STRINGID_SUNLIGHTGOTBRIGHT, 
	STRINGID_STARTEDHAIL, 
	STRINGID_CUSTOMSTRING
};

const bool8 gMoldBreakerIgnoredAbilities[] =
{
	[ABILITY_BATTLE_ARMOR] =	TRUE,
	[ABILITY_CLEAR_BODY] =		TRUE,
	[ABILITY_DAMP] =			TRUE,
	[ABILITY_DRYSKIN] =			TRUE,
	[ABILITY_FILTER] =			TRUE,
	[ABILITY_FLASH_FIRE] =		TRUE,
	[ABILITY_FLOWERGIFT] =		TRUE,
	[ABILITY_HEATPROOF] =		TRUE,
	[ABILITY_HYPER_CUTTER] =	TRUE,
	[ABILITY_IMMUNITY] =		TRUE,
	[ABILITY_INNER_FOCUS] =		TRUE,
	[ABILITY_INSOMNIA] =		TRUE,
	[ABILITY_KEEN_EYE] =		TRUE,
	[ABILITY_LEAFGUARD] =		TRUE,
	[ABILITY_LEVITATE] =		TRUE,
	[ABILITY_LIGHTNING_ROD] =	TRUE,
	[ABILITY_LIMBER] =			TRUE,
	[ABILITY_MAGMA_ARMOR] =		TRUE,
	[ABILITY_MARVEL_SCALE] =	TRUE,
	[ABILITY_MOTORDRIVE] =		TRUE,
	[ABILITY_OBLIVIOUS] =		TRUE,
	[ABILITY_OWN_TEMPO] =		TRUE,
	[ABILITY_SAND_VEIL] =		TRUE,
	[ABILITY_SHELL_ARMOR] =		TRUE,
	[ABILITY_SHIELD_DUST] =		TRUE,
	[ABILITY_SIMPLE] =			TRUE,
	[ABILITY_SNOWCLOAK] =		TRUE,
	[ABILITY_SOLIDROCK] =		TRUE,
	[ABILITY_SOUNDPROOF] =		TRUE,
	[ABILITY_STICKY_HOLD] =		TRUE,
	[ABILITY_STORMDRAIN] =		TRUE,
	[ABILITY_STURDY] =			TRUE,
	[ABILITY_SUCTION_CUPS] =	TRUE,
	[ABILITY_TANGLEDFEET] =		TRUE,
	[ABILITY_THICK_FAT] =		TRUE,
	[ABILITY_UNAWARE] =			TRUE,
	[ABILITY_VITAL_SPIRIT] =	TRUE,
	[ABILITY_VOLT_ABSORB] =		TRUE,
	[ABILITY_WATER_ABSORB] =	TRUE,
	[ABILITY_WATER_VEIL] =		TRUE,
	[ABILITY_WHITE_SMOKE] =		TRUE,
	[ABILITY_WONDER_GUARD] =	TRUE,
	[ABILITY_BIGPECKS] =		TRUE,
	[ABILITY_CONTRARY] =		TRUE,
	[ABILITY_FRIENDGUARD] =		TRUE,
	[ABILITY_HEAVYMETAL] =		TRUE,
	[ABILITY_LIGHTMETAL] =		TRUE,
	[ABILITY_MAGICBOUNCE] =		TRUE,
	[ABILITY_MULTISCALE] =		TRUE,
	[ABILITY_SAPSIPPER] =		TRUE,
	[ABILITY_TELEPATHY] =		TRUE,
	[ABILITY_WONDERSKIN] =		TRUE,
	[ABILITY_AROMAVEIL] =		TRUE,
	[ABILITY_BULLETPROOF] =		TRUE,
	[ABILITY_FLOWERVEIL] =		TRUE,
	[ABILITY_FURCOAT] =			TRUE,
	[ABILITY_OVERCOAT] =		TRUE,
	[ABILITY_SWEETVEIL] =		TRUE,
	[ABILITY_DAZZLING] =		TRUE,
	[ABILITY_DISGUISE] =		TRUE,
	[ABILITY_FLUFFY] =			TRUE,
	[ABILITY_QUEENLYMAJESTY] =	TRUE,
	[ABILITY_WATERBUBBLE] =		TRUE,
	[ABILITY_PORTALPOWER] =		TRUE,
	[ABILITY_MIRRORARMOR] =		TRUE,
	[ABILITY_PUNKROCK] =		TRUE,
	[ABILITY_ICESCALES] =		TRUE,
	[ABILITY_ICEFACE] =			TRUE,
	[ABILITY_PASTELVEIL] =		TRUE,
};

u8 CalcMovePowerForForewarn(u16 move)
{
	u8 power;

	if (gBattleMoves[move].effect == EFFECT_OHKO)
	{
		power = 160;
	}
	else
	{
		switch (move) {
			case MOVE_COUNTER:
			case MOVE_METALBURST:
			case MOVE_MIRROR_COAT:
				power = 120;
				break;

			case MOVE_CRUSHGRIP:
			case MOVE_DRAGON_RAGE:
			case MOVE_ELECTROBALL:
			case MOVE_ENDEAVOR:
			case MOVE_FINALGAMBIT:
			case MOVE_FLAIL:
			case MOVE_FRUSTRATION:
			case MOVE_GRASSKNOT:
			case MOVE_GYROBALL:
			case MOVE_HEATCRASH:
			case MOVE_HEAVYSLAM:
			case MOVE_HIDDEN_POWER:
			case MOVE_LOW_KICK:
			case MOVE_NATURALGIFT:
			case MOVE_NIGHT_SHADE:
			case MOVE_PSYWAVE:
			case MOVE_RETURN:
			case MOVE_REVERSAL:
			case MOVE_SEISMIC_TOSS:
			case MOVE_SONIC_BOOM:
			case MOVE_TRUMPCARD:
			case MOVE_WRINGOUT:
				power = 80;
				break;

			default:
				power = gBattleMoves[move].power;
		}
	}
	return power;
}

u8 ActivateWeatherAbility(u16 flags, u16 item, u8 bank, u8 animArg, u8 stringIndex, bool8 moveTurn)
{
	#ifdef INFINITE_WEATHER
		gBattleWeather = flags;
		gWishFutureKnock.weatherDuration = 0;
		++item; //So it compiles warning free
	#else
		gBattleWeather = (flags & ~(WEATHER_PERMANENT_ANY)); //Remove any weather permanence
		if (ITEM_EFFECT(bank) == item)
			gWishFutureKnock.weatherDuration = 8;
		else
			gWishFutureKnock.weatherDuration = 5;
	#endif

	gBattleCommunication[MULTISTRING_CHOOSER] = stringIndex;
	gBattleScripting.animArg1 = animArg;

	if (moveTurn)
	{
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_NewWeatherAbilityActivatesCall;
	}
	else
	{
		BattleScriptPushCursorAndCallback(BattleScript_NewWeatherAbilityActivates);
	}

	gBattleScripting.battler = bank;
	return TRUE;
}

bool8 AllMainStatsButOneAreMinned(u8 bank)
{
	u8 i, counter;
	for (i = STAT_ATK, counter = 0; i <= STAT_SPDEF; ++i) //No Acc of Evsn
	{
		if (STAT_STAGE(bank, i) > 0)
		{
			if (++counter > 1)
				return FALSE;
		}
	}

	return TRUE;
}

void ApplyFlashFireBoost(void)
{
	gBattleResources->flags->flags[gBattleScripting.battler] |= RESOURCE_FLAG_FLASH_FIRE;
}

void EmitDataTransfer(u8 bufferId, void* dst, u16 size, void* data)
{
	s32 i;

	sBattleBuffersTransferData[0] = CONTROLLER_DATATRANSFER;
	sBattleBuffersTransferData[1] = (u32)(dst);
	sBattleBuffersTransferData[2] = ((u32)(dst) & 0x0000FF00) >> 8;
	sBattleBuffersTransferData[3] = ((u32)(dst) & 0x00FF0000) >> 16;
	sBattleBuffersTransferData[4] = ((u32)(dst) & 0xFF000000) >> 24;
	sBattleBuffersTransferData[5] = size;
	sBattleBuffersTransferData[6] = (size & 0xFF00) >> 8;
	for (i = 0; i < size; i++)
		sBattleBuffersTransferData[7 + i] = *(u8*)(data++);
	PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, size + 7);
}

bool8 ImmunityAbilityCheck(u8 bank, u32 status, const u8* string)
{

	if (gBattleMons[bank].status1 & status)
	{
		StringCopy(gBattleTextBuff1, string);
		gBattleMons[bank].status1 = 0;
		gBattleMons[bank].status2 &= ~(STATUS2_NIGHTMARE);
		BattleScriptPushCursorAndCallback(BattleScript_AbilityCuredStatusEnd3);
		gBattleScripting.battler = bank;
		gActiveBattler = bank;
		BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
		MarkBattlerForControllerExec(gActiveBattler);
		return TRUE;
	}
	return FALSE;
}

s8 PriorityCalc(u8 bank, u8 action, u16 move)
{
	u8 priority = 0;

	if (!BATTLER_ALIVE(bank))
		return 0;

	if (action == B_ACTION_USE_MOVE)
	{
		if (gProtectStructs[bank].noValidMoves)
			move = MOVE_STRUGGLE;

		priority = gBattleMoves[move].priority;

		if (move != MOVE_BIDE && gBattleMons[bank].status2 & STATUS2_BIDE)
			priority = 1;

		switch (ABILITY(bank)) {
			case ABILITY_PRANKSTER:
				if (SPLIT(move) == SPLIT_STATUS)
					++priority;
				break;

			case ABILITY_GALEWINGS:
				if (GetMoveTypeSpecial(bank, move) == TYPE_FLYING)
				{
					#ifndef OLD_GALE_WINGS
						if (BATTLER_MAX_HP(bank))
					#endif
							++priority;
				}
				break;

			case ABILITY_TRIAGE:
				if (gBattleMoves[move].flags & FLAG_TRIAGE_AFFECTED)
					priority += 3;
		}
		
		if (move == MOVE_GRASSYGLIDE && gTerrainType == GRASSY_TERRAIN && CheckGrounding(bank))
			++priority;
	}

	return priority;
}

u8 TryActivateTerrainAbility(u8 terrain, u8 anim, u8 bank)
{
	u8 effect = 0;

	if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_TERRAIN)
		return effect; //Can't be removed

	if (gTerrainType != terrain)
	{
		gTerrainType = terrain;
		#ifdef INFINITE_TERRAIN
			gBattleStruct->TerrainTimer = 0;
		#else
			if (ITEM_EFFECT(bank) == HOLD_EFFECT_TERRAIN_EXTENDER)
				gBattleStruct->TerrainTimer = 8;
			else
				gBattleStruct->TerrainTimer = 5;
		#endif

		switch (terrain) {
			case ELECTRIC_TERRAIN:
				gBattleStringLoader = ElectricTerrainSetString;
				break;
			case GRASSY_TERRAIN:
				gBattleStringLoader = GrassyTerrainSetString;
				break;
			case MISTY_TERRAIN:
				gBattleStringLoader = MistyTerrainSetString;
				break;
			case PSYCHIC_TERRAIN:
				gBattleStringLoader = PsychicTerrainSetString;
				break;
		}

		gBattleScripting.animArg1 = anim;
		gBattleScripting.battler = bank;
		*((u32*) gSeedHelper) = (u32) gBattleStringLoader; //Backup String
		BattleScriptPushCursorAndCallback(BattleScript_TerrainFromAbility);
		++effect;
	}
	return effect;
}

//Ability Pop-Up//////////////////////////////////////////////////////////////////////////////////////
//Credit to the Emerald Battle Engine Upgrade 2.0 by DizzyEgg for the original code.

// for sprite
#define tOriginalX	  data[0]
#define tHide		   data[1]
#define tFrames		 data[2]
#define tRightToLeft	data[3]
#define tBattlerId	  data[4]
#define tIsMain		 data[5]

// for task
#define tSpriteId1	  data[6]
#define tSpriteId2	  data[7]

static const u8 Ability_Pop_UpTiles[] = INCBIN_U8("graphics/battle_interface/Ability_Pop_Up.4bpp");
static const u16 Ability_Pop_UpPal[] = INCBIN_U16("graphics/battle_interface/Ability_Pop_Up.gbapal");

static const struct SpriteSheet sSpriteSheet_AbilityPopUp = {Ability_Pop_UpTiles, sizeof(Ability_Pop_UpTiles), ANIM_TAG_ABILITY_POP_UP};
static const struct SpritePalette sSpritePalette_AbilityPopUp = {Ability_Pop_UpPal,ANIM_TAG_ABILITY_POP_UP};

static void SpriteCb_AbilityPopUp(struct Sprite *sprite);
static void Task_FreeAbilityPopUpGfx(u8 taskId);

static const struct OamData sOamData_AbilityPopUp =
{
	.y = 0,
	.affineMode = 0,
	.objMode = 0,
	.mosaic = 0,
	.bpp = 0,
	.shape = ST_OAM_H_RECTANGLE,
	.x = 0,
	.matrixNum = 0,
	.size = 3,
	.tileNum = 0,
	.priority = 0,
	.paletteNum = 0,
	.affineParam = 0,
};

static const union AnimCmd sSpriteAnim_AbilityPopUp1[] =
{
	ANIMCMD_FRAME(0, 0),
	ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable_AbilityPopUp1[] =
{
	sSpriteAnim_AbilityPopUp1
};

static const struct SpriteTemplate sSpriteTemplate_AbilityPopUp1 =
{
	.tileTag = ANIM_TAG_ABILITY_POP_UP,
	.paletteTag = ANIM_TAG_ABILITY_POP_UP,
	.oam = &sOamData_AbilityPopUp,
	.anims = sSpriteAnimTable_AbilityPopUp1,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCb_AbilityPopUp
};

static const union AnimCmd sSpriteAnim_AbilityPopUp2[] =
{
	ANIMCMD_FRAME(32, 0),
	ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable_AbilityPopUp2[] =
{
	sSpriteAnim_AbilityPopUp2
};

static const struct SpriteTemplate sSpriteTemplate_AbilityPopUp2 =
{
	.tileTag = ANIM_TAG_ABILITY_POP_UP,
	.paletteTag = ANIM_TAG_ABILITY_POP_UP,
	.oam = &sOamData_AbilityPopUp,
	.anims = sSpriteAnimTable_AbilityPopUp2,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCb_AbilityPopUp
};

#define ABILITY_POP_UP_POS_X_DIFF 64
#define ABILITY_POP_UP_POS_X_SLIDE 68

static const s16 sAbilityPopUpCoordsDoubles[MAX_BATTLERS_COUNT][2] =
{
	{29, 80}, // player left
	{186, 19}, // opponent left
	{29, 97}, // player right
	{186, 36}, // opponent right
};

static const s16 sAbilityPopUpCoordsSingles[MAX_BATTLERS_COUNT][2] =
{
	{29, 97}, // player
	{186, 57}, // opponent
};

static u8* AddTextPrinterAndCreateWindowOnAbilityPopUp(const u8* str, u32 x, u32 y, u32 color1, u32 color2, u32 color3, u32* windowId)
{
	u8 color[3] = {color1, color2, color3};
	struct WindowTemplate winTemplate = {0};
	winTemplate.width = 8;
	winTemplate.height = 2;

	*windowId = AddWindow(&winTemplate);
	FillWindowPixelBuffer(*windowId, PIXEL_FILL(color1));

	AddTextPrinterParameterized4(*windowId, 0, x, y, 0, 0, color, -1, str);
	return (u8*)(GetWindowAttribute(*windowId, WINDOW_TILE_DATA));
}

static void TextIntoAbilityPopUp(void *dest, u8 *windowTileData, s32 arg2, bool32 arg3)
{
	CpuCopy32(windowTileData + 256, dest + 256, arg2 * 32);
	if (arg2 > 0)
	{
		do
		{
			if (arg3)
				CpuCopy32(windowTileData + 16, dest + 16, 16);
			else
				CpuCopy32(windowTileData + 20, dest + 20, 12);
			dest += 32, windowTileData += 32;
			arg2--;
		} while (arg2 != 0);
	}
}

#define MAX_CHARS_PRINTED 12

static void PrintOnAbilityPopUp(const u8* str, u8* spriteTileData1, u8* spriteTileData2, u32 x1, u32 x2, u32 y, u32 color1, u32 color2, u32 color3)
{
	u32 windowId, i;
	u8 *windowTileData;
	u8 text1[MAX_CHARS_PRINTED];
	u8 text2[MAX_CHARS_PRINTED];

	for (i = 0; i < MAX_CHARS_PRINTED; i++)
	{
		text1[i] = str[i];
		if (text1[i] == EOS)
			break;
	}
	text1[i] = EOS;

	windowTileData = AddTextPrinterAndCreateWindowOnAbilityPopUp(text1, x1, y, color1, color2, color3, &windowId);
	TextIntoAbilityPopUp(spriteTileData1, windowTileData, 8, (y == 0));
	RemoveWindow(windowId);

	if (i == MAX_CHARS_PRINTED)
	{
		for (i = 0; i < MAX_CHARS_PRINTED; i++)
		{
			text2[i] = str[MAX_CHARS_PRINTED + i];
			if (text2[i] == EOS)
				break;
		}
		text2[i] = EOS;

		windowTileData = AddTextPrinterAndCreateWindowOnAbilityPopUp(text2, x2, y, color1, color2, color3, &windowId);
		TextIntoAbilityPopUp(spriteTileData2, windowTileData, 3, (y == 0));
		RemoveWindow(windowId);
	}
}

static void PrintBattlerOnAbilityPopUp(u8 battlerId, u8 spriteId1, u8 spriteId2)
{
	int i;
	u8 lastChar;
	u8* textPtr;
	u8 monName[POKEMON_NAME_LENGTH + 3] = {0};
	u8* nick = GetIllusionPartyData(battlerId)->box.nickname;

	for (i = 0; i < POKEMON_NAME_LENGTH; ++i)
	{
		monName[i] = nick[i];

		if (nick[i] == 0xFF || i + 1 == POKEMON_NAME_LENGTH) //End of string
			break;
	}

	textPtr = monName + i + 1;

	if (*(textPtr - 1) == 0xFF)
		--textPtr;

	lastChar = *(textPtr - 1);

	//Make the string say "[NAME]'s" instead of "[NAME]"
	textPtr[0] = 0xB4; //'
	++textPtr;
	if (lastChar != CHAR_S && lastChar != CHAR_s) //Proper grammar for names ending in "s"
	{
		textPtr[0] = CHAR_s; //s
		++textPtr;
	}
	textPtr[0] = EOS;

	PrintOnAbilityPopUp((const u8*) monName,
						(void*)(OBJ_VRAM0) + (gSprites[spriteId1].oam.tileNum * 32),
						(void*)(OBJ_VRAM0) + (gSprites[spriteId2].oam.tileNum * 32),
						5, 0,
						0,
						2, 7, 1);
}

static void PrintAbilityOnAbilityPopUp(u32 ability, u8 spriteId1, u8 spriteId2)
{
	const u8* abilityName = gAbilityNames[ability];

	PrintOnAbilityPopUp(abilityName,
						(void*)(OBJ_VRAM0) + (gSprites[spriteId1].oam.tileNum * 32) + 256,
						(void*)(OBJ_VRAM0) + (gSprites[spriteId2].oam.tileNum * 32) + 256,
						5, 1,
						4,
						7, 9, 1);
}

#define PIXEL_COORDS_TO_OFFSET(x, y)(			\
/*Add tiles by X*/								\
((y / 8) * 32 * 8)								\
/*Add tiles by X*/								\
+ ((x / 8) * 32)								\
/*Add pixels by Y*/								\
+ ((((y) - ((y / 8) * 8))) * 4)					\
/*Add pixels by X*/								\
+ ((((x) - ((x / 8) * 8)) / 2)))

static const u16 sOverwrittenPixelsTable[][2] =
{
	{PIXEL_COORDS_TO_OFFSET(0, 0), 5},
	{PIXEL_COORDS_TO_OFFSET(0, 1), 5},
	{PIXEL_COORDS_TO_OFFSET(0, 2), 5},
	{PIXEL_COORDS_TO_OFFSET(0, 3), 5},
	{PIXEL_COORDS_TO_OFFSET(0, 4), 5},
	{PIXEL_COORDS_TO_OFFSET(0, 5), 5},
	{PIXEL_COORDS_TO_OFFSET(0, 6), 5},
	{PIXEL_COORDS_TO_OFFSET(0, 7), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 8), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 9), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 10), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 11), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 12), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 13), 8},

	{PIXEL_COORDS_TO_OFFSET(8, 13), 8},
	{PIXEL_COORDS_TO_OFFSET(16, 13), 8},
	{PIXEL_COORDS_TO_OFFSET(24, 13), 8},
	{PIXEL_COORDS_TO_OFFSET(32, 13), 8},
	{PIXEL_COORDS_TO_OFFSET(40, 13), 8},
	{PIXEL_COORDS_TO_OFFSET(48, 13), 8},
	{PIXEL_COORDS_TO_OFFSET(56, 13), 8},

	{PIXEL_COORDS_TO_OFFSET(0, 14), 8},
	{PIXEL_COORDS_TO_OFFSET(8, 14), 8},
	{PIXEL_COORDS_TO_OFFSET(16, 14), 8},
	{PIXEL_COORDS_TO_OFFSET(24, 14), 8},
	{PIXEL_COORDS_TO_OFFSET(32, 14), 8},
	{PIXEL_COORDS_TO_OFFSET(40, 14), 8},
	{PIXEL_COORDS_TO_OFFSET(48, 14), 8},
	{PIXEL_COORDS_TO_OFFSET(56, 14), 8},

	{PIXEL_COORDS_TO_OFFSET(0, 15), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 16), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 17), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 18), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 19), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 20), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 21), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 22), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 23), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 24), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 25), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 26), 3},

//Second Row Of Image
	{PIXEL_COORDS_TO_OFFSET(0, 45), 8},
	{PIXEL_COORDS_TO_OFFSET(0, 46), 8},
	{PIXEL_COORDS_TO_OFFSET(0, 47), 8},
	{PIXEL_COORDS_TO_OFFSET(0, 48), 8},
	{PIXEL_COORDS_TO_OFFSET(8, 45), 8},
	{PIXEL_COORDS_TO_OFFSET(8, 46), 8},
	{PIXEL_COORDS_TO_OFFSET(8, 47), 8},
	{PIXEL_COORDS_TO_OFFSET(8, 48), 8},
	{PIXEL_COORDS_TO_OFFSET(16, 45), 8},
	{PIXEL_COORDS_TO_OFFSET(16, 46), 8},
	{PIXEL_COORDS_TO_OFFSET(16, 47), 8},
	{PIXEL_COORDS_TO_OFFSET(16, 48), 8},
};

static inline void CopyPixels(u8 *dest, const u8 *src, u32 pixelCount)
{
	u32 i = 0;

	if (pixelCount & 1)
	{
		while (pixelCount != 0)
		{
			dest[i] &= ~(0xF);
			dest[i] |= (src[i] & 0xF);
			if (--pixelCount != 0)
			{
				dest[i] &= ~(0xF0);
				dest[i] |= (src[i] & 0xF0);
				pixelCount--;
			}
			i++;
		}
	}
	else
	{
		for (i = 0; i < pixelCount / 2; i++)
			dest[i] = src[i];
	}
}

static void RestoreOverwrittenPixels(u8 *tiles)
{
	u32 i;
	u8 *buffer = malloc(sizeof(Ability_Pop_UpTiles) * 2);

	CpuCopy32(tiles, buffer, sizeof(Ability_Pop_UpTiles));

	for (i = 0; i < ARRAY_COUNT(sOverwrittenPixelsTable); i++)
	{
		CopyPixels(buffer + sOverwrittenPixelsTable[i][0],
				   Ability_Pop_UpTiles + sOverwrittenPixelsTable[i][0],
				   sOverwrittenPixelsTable[i][1]);
	}

	CpuCopy32(buffer, tiles, sizeof(Ability_Pop_UpTiles));
	Free(buffer);
}

void AnimTask_LoadAbilityPopUp(u8 taskId)
{
	const s16 (*coords)[2];
	u8 spriteId1, spriteId2, battlerPosition, destroyerTaskId;
	u8 ability = gAbilityPopUpHelper; //Preceded by transfer of proper ability

	LoadCompressedSpriteSheetUsingHeap(&gBattleAnimPicTable[ANIM_TAG_ABILITY_POP_UP - ANIM_SPRITES_START]);
	LoadCompressedSpritePaletteUsingHeap(&gBattleAnimPaletteTable[ANIM_TAG_ABILITY_POP_UP - ANIM_SPRITES_START]);

	gBattleStruct->activeAbilityPopUps |= gBitTable[gBattleAnimAttacker];
	battlerPosition = GetBattlerPosition(gBattleAnimAttacker);

	if (IS_DOUBLE_BATTLE)
		coords = sAbilityPopUpCoordsDoubles;
	else
		coords = sAbilityPopUpCoordsSingles;

	if ((battlerPosition & BIT_SIDE) == B_SIDE_PLAYER)
	{
		spriteId1 = CreateSprite(&sSpriteTemplate_AbilityPopUp1,
								coords[battlerPosition][0] - ABILITY_POP_UP_POS_X_SLIDE,
								coords[battlerPosition][1], 0);
		spriteId2 = CreateSprite(&sSpriteTemplate_AbilityPopUp2,
								coords[battlerPosition][0] - ABILITY_POP_UP_POS_X_SLIDE + ABILITY_POP_UP_POS_X_DIFF,
								coords[battlerPosition][1], 0);

		gSprites[spriteId1].tOriginalX = coords[battlerPosition][0];
		gSprites[spriteId2].tOriginalX = coords[battlerPosition][0] + ABILITY_POP_UP_POS_X_DIFF;

		gSprites[spriteId1].tRightToLeft = TRUE;
		gSprites[spriteId2].tRightToLeft = TRUE;
	}
	else
	{
		spriteId1 = CreateSprite(&sSpriteTemplate_AbilityPopUp1,
								coords[battlerPosition][0] + ABILITY_POP_UP_POS_X_SLIDE,
								coords[battlerPosition][1], 0);
		spriteId2 = CreateSprite(&sSpriteTemplate_AbilityPopUp2,
								coords[battlerPosition][0] + ABILITY_POP_UP_POS_X_SLIDE + ABILITY_POP_UP_POS_X_DIFF,
								coords[battlerPosition][1], 0);

		gSprites[spriteId1].tOriginalX = coords[battlerPosition][0];
		gSprites[spriteId2].tOriginalX = coords[battlerPosition][0] + ABILITY_POP_UP_POS_X_DIFF;

		gSprites[spriteId1].tRightToLeft = FALSE;
		gSprites[spriteId2].tRightToLeft = FALSE;
	}

	gBattleStruct->abilityPopUpIds[gBattleAnimAttacker][0] = spriteId1;
	gBattleStruct->abilityPopUpIds[gBattleAnimAttacker][1] = spriteId2;

	destroyerTaskId = CreateTask(Task_FreeAbilityPopUpGfx, 5);
	gTasks[destroyerTaskId].tSpriteId1 = spriteId1;
	gTasks[destroyerTaskId].tSpriteId2 = spriteId2;

	gSprites[spriteId1].tIsMain = TRUE;
	gSprites[spriteId1].tBattlerId = gBattleAnimAttacker;
	gSprites[spriteId2].tBattlerId = gBattleAnimAttacker;

	StartSpriteAnim(&gSprites[spriteId1], 0);
	StartSpriteAnim(&gSprites[spriteId2], 0);

	PrintBattlerOnAbilityPopUp(gBattleAnimAttacker, spriteId1, spriteId2);
	PrintAbilityOnAbilityPopUp(ability, spriteId1, spriteId2);
	RestoreOverwrittenPixels((void*)(OBJ_VRAM0) + (gSprites[spriteId1].oam.tileNum * 32));

	DestroyAnimVisualTask(taskId);
}

static void SpriteCb_AbilityPopUp(struct Sprite* sprite)
{
	if (!sprite->tHide) // Show
	{
		if (sprite->tIsMain && ++sprite->tFrames == 4)
			PlaySE(0x80); //Also could be 0xBA
		if ((!sprite->tRightToLeft && (sprite->x -= 4) <= sprite->tOriginalX)
		||   (sprite->tRightToLeft && (sprite->x += 4) >= sprite->tOriginalX))
		{
			sprite->x = sprite->tOriginalX;
			sprite->tHide = TRUE;
			sprite->tFrames = 1;
		}
	}
	else // Hide
	{
		if (sprite->tFrames == 0)
		{
			if ((!sprite->tRightToLeft && (sprite->x += 4) >= sprite->tOriginalX + ABILITY_POP_UP_POS_X_SLIDE + 16)
			||   (sprite->tRightToLeft && (sprite->x -= 4) <= sprite->tOriginalX - ABILITY_POP_UP_POS_X_SLIDE - 16))
			{
				gBattleStruct->activeAbilityPopUps &= ~(gBitTable[sprite->tBattlerId]);
				DestroySprite(sprite);
			}
		}
	}
}

static void Task_FreeAbilityPopUpGfx(u8 taskId)
{
	u8 bank;

	if (!gSprites[gTasks[taskId].tSpriteId1].inUse
		&& !gSprites[gTasks[taskId].tSpriteId2].inUse
		&& !gBattleStruct->activeAbilityPopUps)
	{
		bank = gSprites[gTasks[taskId].tSpriteId1].tBattlerId;

		gBattleStruct->abilityPopUpIds[bank][0] = 0;
		gBattleStruct->abilityPopUpIds[bank][1] = 0;
		FreeSpriteTilesByTag(ANIM_TAG_ABILITY_POP_UP);
		FreeSpritePaletteByTag(ANIM_TAG_ABILITY_POP_UP);
		DestroyTask(taskId);
	}
}

void AnimTask_DestroyAbilityPopUp(u8 taskId)
{
	gSprites[gBattleStruct->abilityPopUpIds[gBattleAnimAttacker][0]].tFrames = 0;
	gSprites[gBattleStruct->abilityPopUpIds[gBattleAnimAttacker][1]].tFrames = 0;
	DestroyAnimVisualTask(taskId);
}

void TransferAbilityPopUpHelper(void)
{
	if (gBattleControllerExecFlags)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}

	TransferAbilityPopUp(gBattleScripting.battler, CopyAbility(gBattleScripting.battler));
	gLastUsedAbility = gAbilityPopUpHelper;
}

void TransferAbilityPopUpHelperAsTrace(void)
{
	if (gBattleControllerExecFlags)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}

	TransferAbilityPopUp(gBattleScripting.battler, ABILITY_TRACE);
}

void TransferAbilityPopUpHelperAsImposter(void)
{
	if (gBattleControllerExecFlags)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}

	TransferAbilityPopUp(gBattleScripting.battler, ABILITY_IMPOSTER);
}

void TransferAbilityPopUpHelperAsWanderingSpirit(void)
{
	if (gBattleControllerExecFlags)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}

	TransferAbilityPopUp(gBattleScripting.battler, ABILITY_WANDERING_SPIRIT);
}

void TransferAbilityPopUp(u8 bank, u8 ability)
{
	gActiveBattler = bank;
	gAbilityPopUpHelper = ability;

	EmitDataTransfer(0, &gAbilityPopUpHelper, 1, &gAbilityPopUpHelper);
	MarkBattlerForControllerExec(gActiveBattler);
}

void TryRemoveIntimidateAbilityPopUp(void)
{
	if (gBattleStruct->intimidateActive > 0)
	{
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_AbilityPopUpRevert - 5;
		gBattleScripting.battler = gBattleStruct->intimidateActive - 1;
		gBattleStruct->intimidateActive -= 10; //Intimidate is still active but pop-up is hidden
	}
}

void TryHideActiveAbilityPopUps(void)
{
	u8 i;

	for (i = 0; i < gBattlersCount; ++i)
	{
		if (gBattleStruct->activeAbilityPopUps & gBitTable[i])
		{
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_AbilityPopUpRevert - 5;
			gBattleScripting.battler = i;
		}
	}
}

void RemoveIntimidateActive(void)
{
	gBattleStruct->intimidateActive = 0;
}

void TryReactiveIntimidatePopUp(void)
{
	if (gBattleStruct->intimidateActive < 0) //Pop-up was hidden due to ability like Clear Body
	{
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_AbilityPopUp - 5;
		gBattleStruct->intimidateActive += 10;
		gBattleScripting.battler = gBattleStruct->intimidateActive - 1;
	}
}

void LoadIntimidateBankIntoTarget(void)
{
	if (gBattleStruct->intimidateActive > 0)
		gBattlerTarget = gBattleStruct->intimidateActive - 1;
	else if (gBattleStruct->intimidateActive < 0)
		gBattlerTarget = (gBattleStruct->intimidateActive + 10) - 1;
}

//Switch-in abilities that affect a knocked out target
//after using U-Turn/Volt Switch are delayed until a
//new Pokemon is sent out.
void SetSkipCertainSwitchInAbilities(void)
{
	if (BATTLER_ALIVE(FOE(gBattlerAttacker)))
		return;

	if (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(FOE(gBattlerAttacker))))
		return;

	//Only set the bit if no enemies are alive on the field
	gBattleStruct->skipCertainSwitchInAbilities = TRUE;
}
