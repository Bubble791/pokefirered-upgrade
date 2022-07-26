#ifndef GUARD_SET_Z_EFFECT_H
#define GUARD_SET_Z_EFFECT_H

void SetZEffect(void);
u16 GetTypeBasedZMove(u16 move, u8 bank);
u16 GetSpecialZMove(u16 move, u16 species, u16 item);
bool8 IsSpecialZCrystal(u16 item);
const u8* GetZMoveName(u16 move);
bool8 DoesZMoveUsageStopMegaEvolution(u8 bank);
bool8 DoesZMoveUsageStopDynamaxing(u8 bank);
u16 CanUseZMove(u8 bank, u8 moveIndex, u16 move);
u16 ReplaceWithZMoveRuntime(u8 bankAtk, u16 move);

#endif //GUARD_SET_Z_EFFECT_H