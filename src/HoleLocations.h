#pragma once
#include <cstdint>

// Generated from Tekkit's Workshop ALL-IN-ONE marker pack (tw_core_fishing.xml).
// Each entry is one in-game fishing-hole position.
// game_x / game_z match MumbleLink fAvatarPosition[0]/[2] (metres).

enum class HoleWater : uint8_t {
    Boreal = 0,
    Cavern = 1,
    Channel = 2,
    Coastal = 3,
    Deep = 4,
    Desert = 5,
    Freshwater = 6,
    Grotto = 7,
    Lake = 8,
    Lakeboreal = 9,
    Lakecoastal = 10,
    Lakenoxious = 11,
    Lakeriver = 12,
    Noxious = 13,
    Offshore = 14,
    Polluted = 15,
    Quarry = 16,
    River = 17,
    Saltwater = 18,
    Shinota = 19,
    Shore = 20,
    Volcanic = 21,
    Volcanicnoxious = 22,
    Wreckage = 23,
    // SotO/Janthir Wilds zones use per-map/per-area named "school" holes
    // instead of the marker pack's terrain-type holes — no coordinate data
    // exists for them (see HOLE_LOCATION_TABLE), so these only drive the
    // "Hole" UI label, not map markers or MapHasHoleType restriction.
    //
    // IMPORTANT: this data is sourced from each fish's own wiki page, not the
    // hole-type pages' catch-table listings (e.g. what species "Nayosian
    // Fish" claims to catch) — those are NOT reliable, listing a much
    // broader regional pool than what's actually tied to that hole. Two
    // fields matter on a fish's own page:
    //   - "Found in the waters of X[, Y]" (article prose) lists every map it
    //     was confirmed on AS OF THAT FISH'S OWN EXPANSION — trust it for the
    //     maps it names (including staying region-level when it names 2+),
    //     but it is NOT a guarantee against a LATER expansion retroactively
    //     adding the fish to a new zone's catch pool without that prose ever
    //     being updated (this has happened before, e.g. older Shiverpeaks
    //     fish becoming catchable in Janthir Wilds zones) — an older fish
    //     still tagged HoleWater::Any/single-map elsewhere in this table may
    //     need re-auditing if a newer region turns out to also catch it.
    //   - The infobox "Fishing Hole" field is a separate, additional
    //     restriction on top of the map: "Any"/"Any, Open Water" means no
    //     further restriction (matches any hole on the confirmed map(s));
    //     a specific named hole or water-category (e.g. "Fractured Desert
    //     Fish", "Saltwater / Brackish Water") narrows it further. A
    //     "(Higher Chance)"-qualified hole (e.g. "Any, Open Water, Dream
    //     Fish (Higher Chance)") is treated as the required/mandatory hole,
    //     the same convention already used for favored bait and time of day.
    FracturedFreshwater = 24, // Skywatch Archipelago
    FracturedLake       = 25, // Skywatch Archipelago (Stargaze Ridge)
    FracturedChannel    = 26, // Skywatch Archipelago (Jade Mech Habitation Zone 03)
    FracturedDesert     = 27, // Skywatch Archipelago (Skyward Marches)
    AstralFish          = 28, // Amnytas (Bastion of the Natural/Knowledge/Strength)
    SpireFish           = 29, // Amnytas (Bastion of the Obscure/Celestial/Balance)
    Brackish            = 30, // Janthir Wilds water-category (Lowland Shore / Janthir Syntri)
    Nayosian            = 31, // Inner Nayos (Higher Chance hole for some fish)
    Dreamfish           = 32, // Inner Nayos (Higher Chance hole for some fish)
    SaltwaterTropical   = 33, // Shipwreck Strand (also catches some Seitung Province fish)
    Any      = 255,
};

struct HoleLocation {
    uint32_t  mapId;
    float     game_x;
    float     game_z;
    HoleWater water;
};

extern const HoleLocation HOLE_LOCATION_TABLE[];
extern const int          HOLE_LOCATION_COUNT;

const char* HoleWaterName(HoleWater w);
