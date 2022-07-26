#ifndef GUARD_MEGA_H
#define GUARD_MEGA_H

#include "global.h"

const struct Evolution* CanMegaEvolve(u8 bank, bool8 CheckUBInstead);
u16 GetMegaSpecies(u16 species, u16 item, const u16* moves);
ability_t GetBankMegaFormAbility(u8 bank);
const u8* DoMegaEvolution(u8 bank);
const u8* DoPrimalReversion(u8 bank, u8 caseId);
void MegaRevert(struct Pokemon* mon);
void TryRevertMega(struct Pokemon* mon);
bool8 MegaEvolutionEnabled(u8 bank);
bool8 BankMegaEvolved(u8 bank, bool8 checkUB);
bool8 IsMega(u8 bank);
bool8 IsBluePrimal(u8 bank);
bool8 IsRedPrimal(u8 bank);
bool8 IsMegaSpecies(u16 species);
bool8 IsBluePrimalSpecies(u16 species);
bool8 IsRedPrimalSpecies(u16 species);
bool8 IsUltraNecrozma(u8 bank);
bool8 IsUltraNecrozmaSpecies(u16 species);
const u8* GetTrainerName(u8 bank);
u8* TryGetRivalNameByTrainerClass(u8 class);
void MegaRetrieveData(void);

#endif // GUARD_MEGA_H