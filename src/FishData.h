#pragma once
#include <cstdint>
#include <cstring>
#include "HoleLocations.h"
#include <ctime>

// Bait types — index 0 = Any (no special bait)
enum class BaitType : uint8_t {
    Any         = 0,
    BorrowedBait= 1,
    FishEgg     = 2,
    FreshwaterMinnow = 3,
    GlowWorm    = 4,
    HaijuMinnow = 5,
    LavaBeetle  = 6,
    Leech       = 7,
    LightningBug= 8,
    Mackerel    = 9,
    Nightcrawler= 10,
    RamsHornSnail= 11,
    Sardine     = 12,
    Scorpion    = 13,
    Shrimpling  = 14,
    SparkflyLarva= 15,
};

enum class TimeOfDay : uint8_t {
    Any   = 0,
    Dawn  = 1,  // covers Dusk/Dawn
    Day   = 2,
    Dusk  = 3,
    Night = 4,
};

enum class WaterType : uint8_t {
    Freshwater = 0,
    Saltwater  = 1,
    Special    = 2,
};

struct Fish {
    const char* name;
    const char* map;           // GW2 region name
    const char* region;        // Expansion label
    BaitType    bait;
    TimeOfDay   time;
    WaterType   water;
    const char* collection;    // Achievement collection name
    uint32_t    achievementId; // 0 = TBD (Task 10)
    uint8_t     bitIndex;      // 0 = TBD (Task 10)
    uint32_t    itemId;        // GW2 item ID (0 if unknown)
    const char* iconUrl;       // render.guildwars2.com URL
    uint32_t    filletItemId;  // 0 if none
    const char* filletName;    // nullptr if none
    const char* filletIconUrl; // nullptr if none
    const char* masteryRequired; // nullptr if none
    const char* wikiSlug;
    HoleWater   holeType = HoleWater::Any;
};

struct Waypoint {
    const char* name;
    const char* chatLink;
    float       cont_x;
    float       cont_y;
};

struct FishingHole {
    const char* name;
    const char* map;
    uint32_t    mapId;
    float       game_x;
    float       game_z;
    uint16_t    waypointIdx;
    uint16_t    fishIds[16];
    uint8_t     fishCount;
};

struct FishingCollection {
    const char* name;
    uint32_t    achievementId;       // base "X Fisher" (catch each fish once)
    uint8_t     totalFish;
    const char* iconUrl;
    uint32_t    avidAchievementId;   // repeatable "Avid X Fisher" (0 if none exists)
};

extern const char*             BAIT_NAMES[];
extern const int               BAIT_COUNT;
extern const Fish              FISH_TABLE[];
extern const int               FISH_COUNT;
extern const Waypoint          WAYPOINT_TABLE[];
extern const int               WAYPOINT_COUNT;
extern const FishingHole       HOLE_TABLE[];
extern const int               HOLE_COUNT;
extern const FishingCollection COLLECTION_TABLE[];
extern const int               COLLECTION_COUNT;

const char* GetFishRarity(uint32_t itemId);

// Returns the recommended fishing power for a fish given its region (Fish.map)
// and holeType. 0 = unknown / not displayed.
int GetRecommendedPower(const char* fishMap, HoleWater holeType);

struct BaitInfo {
    uint32_t    itemId;
    const char* iconUrl;
};
// Returns icon/item info for a bait type, or nullptr for Any/BorrowedBait.
const BaitInfo* GetBaitInfo(BaitType b);

struct BonusItem {
    uint32_t    itemId;
    const char* name;
    const char* iconUrl;
    bool        isChance; // true = chance drop, false = guaranteed
};

// Returns count of bonus drops for a fish (keyed by fish itemId).
int GetBonusItemCount(uint32_t fishItemId);
// Returns bonus drop at given index, or nullptr if out of range.
const BonusItem* GetBonusItem(uint32_t fishItemId, int index = 0);

inline const char* TimeOfDayName(TimeOfDay t) {
    switch (t) {
        case TimeOfDay::Dawn:  return "Dawn";
        case TimeOfDay::Day:   return "Day";
        case TimeOfDay::Dusk:  return "Dusk";
        case TimeOfDay::Night: return "Night";
        default:               return "Any";
    }
}

inline const char* WaterTypeName(WaterType w) {
    switch (w) {
        case WaterType::Saltwater:  return "Saltwater";
        case WaterType::Special:    return "Special";
        default:                    return "Freshwater";
    }
}

// GW2 day-night cycle: 7200 sec (120 min). UTC 00:00 = Tyrian 00:00 (mid-Night).
// 1 Tyrian hour = 300 real seconds = 5 real minutes.
// Phase boundaries on the Tyrian clock:
//   Night 21:00–05:00
//   Dawn  05:00–06:00
//   Day   06:00–20:00
//   Dusk  20:00–21:00
// As cycle-seconds (0 = Tyrian midnight):
//   0    – 1500 Night (early)
//   1500 – 1800 Dawn
//   1800 – 6000 Day
//   6000 – 6300 Dusk
//   6300 – 7200 Night (late)

static const uint32_t TYRIAN_CYCLE   = 7200;
static const uint32_t TY_DAWN_START  = 1500;
static const uint32_t TY_DAY_START   = 1800;
static const uint32_t TY_DUSK_START  = 6000;
static const uint32_t TY_NIGHT_START = 6300;

inline uint32_t GetTyrianSeconds() {
    return (uint32_t)(time(nullptr) % TYRIAN_CYCLE);
}

// Tyrian clock hour (0..24). UTC 00:00 = Tyrian 00:00.
inline float GetTyrianHour() {
    return (float)GetTyrianSeconds() / 300.0f;
}

inline TimeOfDay GetCurrentTimeOfDay() {
    uint32_t s = GetTyrianSeconds();
    if (s < TY_DAWN_START)  return TimeOfDay::Night; // 00:00–05:00 Tyrian
    if (s < TY_DAY_START)   return TimeOfDay::Dawn;
    if (s < TY_DUSK_START)  return TimeOfDay::Day;
    if (s < TY_NIGHT_START) return TimeOfDay::Dusk;
    return TimeOfDay::Night;                         // 21:00–24:00 Tyrian
}

// Phase rotation order is identical across cycles (Night→Dawn→Day→Dusk→Night);
// only the boundary hours differ, so this just needs the current phase.
inline TimeOfDay GetNextPhaseFrom(TimeOfDay current) {
    switch (current) {
        case TimeOfDay::Night: return TimeOfDay::Dawn;
        case TimeOfDay::Dawn:  return TimeOfDay::Day;
        case TimeOfDay::Day:   return TimeOfDay::Dusk;
        case TimeOfDay::Dusk:  return TimeOfDay::Night;
        default:               return TimeOfDay::Day;
    }
}

inline TimeOfDay GetNextPhase() {
    return GetNextPhaseFrom(GetCurrentTimeOfDay());
}

inline uint32_t SecondsUntilNextSlot() {
    uint32_t s = GetTyrianSeconds();
    if (s < TY_DAWN_START)  return TY_DAWN_START  - s;
    if (s < TY_DAY_START)   return TY_DAY_START   - s;
    if (s < TY_DUSK_START)  return TY_DUSK_START  - s;
    if (s < TY_NIGHT_START) return TY_NIGHT_START - s;
    // Late night (6300–7200): next boundary is Dawn at 1500 of next cycle.
    return (TYRIAN_CYCLE - s) + TY_DAWN_START;
}

// ---------------------------------------------------------------------------
// Cantha cycle — End of Dragons and Visions of Eternity (Castora) maps run a
// different day/night cycle than core Tyria: day and night are equal length,
// offset from the core cycle's boundaries. Same 7200-second cycle length and
// epoch, just different phase cutoffs.
//   Night 20:00–07:00, Dawn 07:00–08:00, Day 08:00–19:00, Dusk 19:00–20:00
// As cycle-seconds:
//   0    – 2100 Night (early)
//   2100 – 2400 Dawn
//   2400 – 5700 Day
//   5700 – 6000 Dusk
//   6000 – 7200 Night (late)
// ---------------------------------------------------------------------------

static const uint32_t CA_DAWN_START  = 2100;
static const uint32_t CA_DAY_START   = 2400;
static const uint32_t CA_DUSK_START  = 5700;
static const uint32_t CA_NIGHT_START = 6000;

inline bool RegionUsesCanthaCycle(const char* region) {
    if (!region) return false;
    return strcmp(region, "End of Dragons") == 0 || strcmp(region, "Visions of Eternity") == 0;
}

// Draconis Mons (the Volcanic-hole half of "Ring of Fire Fisher") doesn't
// follow any cycle — it's permanently fixed at 09:00 Tyrian time (Day).
inline bool FishUsesCanthaCycle(const Fish& f) {
    return RegionUsesCanthaCycle(f.region);
}

inline TimeOfDay GetCurrentTimeOfDayForFish(const Fish& f) {
    if (f.holeType == HoleWater::Volcanic) return TimeOfDay::Day;
    uint32_t s = GetTyrianSeconds();
    TimeOfDay phase;
    if (FishUsesCanthaCycle(f)) {
        if (s < CA_DAWN_START)       phase = TimeOfDay::Night;
        else if (s < CA_DAY_START)   phase = TimeOfDay::Dawn;
        else if (s < CA_DUSK_START)  phase = TimeOfDay::Day;
        else if (s < CA_NIGHT_START) phase = TimeOfDay::Dusk;
        else                          phase = TimeOfDay::Night;
    } else {
        phase = GetCurrentTimeOfDay();
    }
    // Dawn-tagged fish ("covers Dusk/Dawn") are catchable during both twilight
    // transitions, so collapse Dusk into Dawn here — this function only exists
    // to drive fish-time matching, not to report the literal current phase.
    return (phase == TimeOfDay::Dusk) ? TimeOfDay::Dawn : phase;
}

// Seconds until the next Dawn-or-Dusk transition begins, or 0 if currently in
// one — shared by both cycles' twilight handling in SecondsUntilPhaseForFish.
inline uint32_t SecondsUntilTwilightForBounds(uint32_t s, uint32_t dawnStart, uint32_t dayStart,
                                               uint32_t duskStart, uint32_t nightStart) {
    bool inDawn = (s >= dawnStart && s < dayStart);
    bool inDusk = (s >= duskStart && s < nightStart);
    if (inDawn || inDusk) return 0;
    if (s < dawnStart) return dawnStart - s;
    if (s < duskStart) return duskStart - s;
    return (TYRIAN_CYCLE - s) + dawnStart;
}

// Which twilight transition (Dawn or Dusk) is currently active or coming up
// next — for display purposes, since Dawn-tagged fish are actually counting
// down to whichever one is nearer, and the UI shouldn't always say "Dawn"
// when it's really counting down to Dusk.
inline TimeOfDay TwilightPhaseForBounds(uint32_t s, uint32_t dawnStart, uint32_t dayStart,
                                         uint32_t duskStart, uint32_t nightStart) {
    if (s >= dawnStart && s < dayStart)   return TimeOfDay::Dawn; // currently in Dawn
    if (s >= duskStart && s < nightStart) return TimeOfDay::Dusk; // currently in Dusk
    if (s < dawnStart) return TimeOfDay::Dawn;
    if (s < duskStart) return TimeOfDay::Dusk;
    return TimeOfDay::Dawn; // wraps past Night into next cycle's Dawn
}

inline TimeOfDay TwilightPhaseForFish(const Fish& f) {
    uint32_t s = GetTyrianSeconds();
    return FishUsesCanthaCycle(f)
        ? TwilightPhaseForBounds(s, CA_DAWN_START, CA_DAY_START, CA_DUSK_START, CA_NIGHT_START)
        : TwilightPhaseForBounds(s, TY_DAWN_START, TY_DAY_START, TY_DUSK_START, TY_NIGHT_START);
}

// Seconds until the fish's currently-open window closes. Only meaningful when
// the window really is open right now (i.e. SecondsUntilPhaseForFish(f, f.time)
// == 0) — used to show "ends in X:XX" instead of a re-triggering countdown.
inline uint32_t SecondsUntilPhaseEndsForBounds(uint32_t s, TimeOfDay want,
                                                uint32_t dawnStart, uint32_t dayStart,
                                                uint32_t duskStart, uint32_t nightStart) {
    if (want == TimeOfDay::Day) return duskStart - s;
    if (want == TimeOfDay::Night)
        return (s >= nightStart) ? (TYRIAN_CYCLE - s) + dawnStart : dawnStart - s;
    // Dawn (twilight): currently in the Dawn sub-window or the Dusk sub-window.
    if (s >= dawnStart && s < dayStart) return dayStart - s;
    return nightStart - s;
}

inline uint32_t SecondsUntilPhaseEndsForFish(const Fish& f) {
    uint32_t s = GetTyrianSeconds();
    return FishUsesCanthaCycle(f)
        ? SecondsUntilPhaseEndsForBounds(s, f.time, CA_DAWN_START, CA_DAY_START, CA_DUSK_START, CA_NIGHT_START)
        : SecondsUntilPhaseEndsForBounds(s, f.time, TY_DAWN_START, TY_DAY_START, TY_DUSK_START, TY_NIGHT_START);
}

// Seconds until `want` starts, or 0 if `s` is already inside it. Naive
// "time until start" arithmetic mathematically never lands on exactly 0 — it
// jumps straight from a small countdown to a full-cycle-away value the
// instant the window opens — so a Day/Night-tagged favourite would get
// pruned from the notification almost immediately after its window opened
// instead of staying for the whole window, unless this checks "currently
// active" explicitly first.
inline uint32_t SecondsUntilPhaseForBounds(uint32_t s, TimeOfDay want,
                                            uint32_t dawnStart, uint32_t dayStart,
                                            uint32_t duskStart, uint32_t nightStart) {
    switch (want) {
        case TimeOfDay::Dawn:
            return SecondsUntilTwilightForBounds(s, dawnStart, dayStart, duskStart, nightStart);
        case TimeOfDay::Day:
            if (s >= dayStart && s < duskStart) return 0;
            return (s < dayStart) ? (dayStart - s) : (TYRIAN_CYCLE - s) + dayStart;
        case TimeOfDay::Dusk:
            if (s >= duskStart && s < nightStart) return 0;
            return (s < duskStart) ? (duskStart - s) : (TYRIAN_CYCLE - s) + duskStart;
        case TimeOfDay::Night:
            if (s >= nightStart || s < dawnStart) return 0;
            return nightStart - s;
        default:
            return 0;
    }
}

inline uint32_t SecondsUntilPhaseForFish(const Fish& f, TimeOfDay phase) {
    if (phase == TimeOfDay::Any) return 0;
    if (f.holeType == HoleWater::Volcanic)
        return (phase == TimeOfDay::Day) ? 0 : TYRIAN_CYCLE; // Draconis Mons: always/never

    uint32_t s = GetTyrianSeconds();
    return FishUsesCanthaCycle(f)
        ? SecondsUntilPhaseForBounds(s, phase, CA_DAWN_START, CA_DAY_START, CA_DUSK_START, CA_NIGHT_START)
        : SecondsUntilPhaseForBounds(s, phase, TY_DAWN_START, TY_DAY_START, TY_DUSK_START, TY_NIGHT_START);
}

// Map IDs for the regions that use the Cantha cycle, for callers that only
// have a MumbleLink map ID (not a specific Fish) to work with.
static const uint32_t CANTHA_CYCLE_MAPS[] = {
    1442, // Seitung Province
    1438, // New Kaineng City
    1452, // The Echovald Wilds
    1422, // Dragon's End
    1593, // Starlit Weald
    1595, // Shipwreck Strand
    1622, // Eternity's Garden
};
static const int CANTHA_CYCLE_MAP_COUNT =
    (int)(sizeof(CANTHA_CYCLE_MAPS) / sizeof(CANTHA_CYCLE_MAPS[0]));
// Permanently fixed at 09:00 Tyrian time — not on any cycle at all.
static const uint32_t DRACONIS_MONS_MAP_ID = 1195;
static const float    DRACONIS_MONS_FIXED_HOUR = 9.0f;

inline bool MapUsesCanthaCycle(uint32_t mapId) {
    for (int i = 0; i < CANTHA_CYCLE_MAP_COUNT; i++)
        if (CANTHA_CYCLE_MAPS[i] == mapId) return true;
    return false;
}

inline TimeOfDay GetCurrentTimeOfDayForMap(uint32_t mapId) {
    if (mapId == DRACONIS_MONS_MAP_ID) return TimeOfDay::Day;
    if (!MapUsesCanthaCycle(mapId)) return GetCurrentTimeOfDay();
    uint32_t s = GetTyrianSeconds();
    if (s < CA_DAWN_START)  return TimeOfDay::Night;
    if (s < CA_DAY_START)   return TimeOfDay::Dawn;
    if (s < CA_DUSK_START)  return TimeOfDay::Day;
    if (s < CA_NIGHT_START) return TimeOfDay::Dusk;
    return TimeOfDay::Night;
}

// Raw Tyrian clock hour is the same global value everywhere except Draconis
// Mons, which is frozen and never reflects the real clock at all.
inline float GetTyrianHourForMap(uint32_t mapId) {
    if (mapId == DRACONIS_MONS_MAP_ID) return DRACONIS_MONS_FIXED_HOUR;
    return GetTyrianHour();
}

inline uint32_t SecondsUntilNextSlotForMap(uint32_t mapId) {
    if (mapId == DRACONIS_MONS_MAP_ID) return 0; // never changes; callers should special-case display
    if (!MapUsesCanthaCycle(mapId)) return SecondsUntilNextSlot();
    uint32_t s = GetTyrianSeconds();
    if (s < CA_DAWN_START)  return CA_DAWN_START  - s;
    if (s < CA_DAY_START)   return CA_DAY_START   - s;
    if (s < CA_DUSK_START)  return CA_DUSK_START  - s;
    if (s < CA_NIGHT_START) return CA_NIGHT_START - s;
    return (TYRIAN_CYCLE - s) + CA_DAWN_START;
}

#define CAST_AWAY_ADDON_NAME "Cast Away"
