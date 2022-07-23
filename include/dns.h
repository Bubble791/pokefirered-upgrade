#ifndef GUARD_DNS_UTILS_H
#define GUARD_DNS_UTILS_H

#define DNS_PAL_EXCEPTION   FALSE
#define DNS_PAL_ACTIVE      TRUE

struct LightingColour {
    u8 group;
    u8 mapnum;
    u8 paletteNum;
    u8 colourNum;
    u32 lightColour;
};

enum
{
    SEASON_SPRING = 0,
    SEASON_SUMMER,
    SEASON_AUTUMN,
    SEASON_WINTER,
};

struct DnsPalExceptions {
    bool8 pal[32];
};

void DnsTransferPlttBuffer(void *src, void *dest);
void DnsApplyFilters();
u8 GetDnsTimeLapse(u8 hour);
bool8 IsNight(void);

bool8 IsDayTime(void);
bool8 IsOnlyDayTime(void);
bool8 IsNightTime(void);
bool8 IsMorning(void);
bool8 IsEvening(void);
u32 GetMinuteDifference(u32 startYear, u8 startMonth, u8 startDay, u8 startHour, u8 startMin, u32 endYear, u8 endMonth, u8 endDay, u8 endHour, u8 endMin);
u32 GetHourDifference(u32 startYear, u8 startMonth, u8 startDay, u8 startHour, u32 endYear, u8 endMonth, u8 endDay, u8 endHour);
u32 GetDayDifference(u32 startYear, u8 startMonth, u8 startDay, u32 endYear, u8 endMonth, u8 endDay);
u32 GetMonthDifference(u32 startYear, u8 startMonth, u32 endYear, u8 endMonth);
u32 GetYearDifference(u32 startYear, u32 endYear);
u32 GetDaysSinceTimeInValue(u32 var);

#endif /* GUARD_DNS_UTILS_H */