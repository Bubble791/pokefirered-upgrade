#ifndef GUARD_UTIL_H
#define GUARD_UTIL_H

#include "global.h"

#include "sprite.h"

extern const u8 gMiscBlank_Gfx[]; // unused in Emerald
extern const u32 gBitTable[];

u8 CreateInvisibleSpriteWithCallback(void (*)(struct Sprite *));
void StoreWordInTwoHalfwords(u16 *, unsigned);
void LoadWordFromTwoHalfwords(u16 *, unsigned *);
int CountTrailingZeroBits(u32 value);
u16 CalcCRC16(const u8 *data, u32 length);
u16 CalcCRC16WithTable(const u8 *data, u32 length);
u32 CalcByteArraySum(const u8 *data, u32 length);
void BlendPalette(u16 palOffset, u16 numEntries, u8 coeff, u16 blendColor);
void DoBgAffineSet(struct BgAffineDstData * dest, u32 texX, u32 texY, s16 srcX, s16 srcY, s16 sx, s16 sy, u16 alpha);

u8 GetMonType(struct Pokemon* mon, u8 typeId);
bool8 IsMonOfType(struct Pokemon* mon, u8 type);

u32 MathMax(u32 num1, u32 num2);
u32 MathMin(u32 num1, u32 num2);
u16 RandRange(u16 min, u16 max);
bool8 CheckTableForMove(u16 move, const u16 table[]);
u8 ViableMonCount(struct Pokemon* party);
bool8 CheckTableForSpecies(u16 species, const u16 table[]);
bool8 CheckTableForItem(u16 item, const u16 table[]);
void HealMon(struct Pokemon* mon);
bool8 CanEvolve(struct Pokemon* mon);
u8 FindMovePositionInMonMoveset(u16 move, struct Pokemon* mon);
u32 GetBaseStatsTotal(const u16 species);
bool8 CouldHaveEvolvedViaLevelUp(struct Pokemon* mon);
bool8 MoveInMonMoveset(u16 move, struct Pokemon* mon);
bool8 CheckTableForMoveEffect(u16 move, const u8 table[]);

#endif // GUARD_UTIL_H
