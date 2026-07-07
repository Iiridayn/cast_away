# Cast Away

A fishing companion addon for **Guild Wars 2**, built on the [Nexus](https://raidcore.gg/Nexus) addon framework.

Track every catchable fish, find fishing holes on an interactive map, and never miss the right time-of-day window again.

## AI Notice

This addon has been largely created using Claude. I understand that some folks have a moral, financial or political objection to creating software using an LLM. I just wanted to make a useful tool for the GW2 community, and this was the only way I could do it.

If an LLM creating software upsets you, then perhaps this repo isn't for you. Move on, and enjoy your day.

## Screenshots

![Cast Away main window](screenshots/main.png)
![Cast Away map window](screenshots/map.png)

## Features

- **307 fish** — Core through Visions of Eternity.
- **Interactive map** — 3,660+ fishing holes across 54 maps with GW2 waypoints and player location marker
- **Smart navigation** — selecting a fish jumps to the map in its region with the most matching holes for that fish's hole type.
- **"Here" + "Now" filters** — instantly show fish catchable in your current zone, optionally narrowed to the current day/night phase.
- **Grouped database view** — browse all fish flat or switch to collection groupings, each with a caught/total progress bar.
- **Achievement tracking** — per-collection progress driven by optional Hoard & Seek / Events:Alerts integration.
- **Day/night tracker** — scrolling Tyrian-time bar with phase countdown.
- **Favourite fish alerts** — notifies you before a favourite's time window opens.
- **Fish details** — bait, time, hole type, recommended power, water type, collection, catch status, fillet price, and bonus drops.

## Installation

1. Install [Nexus](https://raidcore.gg/Nexus) into your Guild Wars 2 directory.
2. Drop `CastAway.dll` into `<GW2>\addons\`.
3. Launch the game — Cast Away appears as a window button in the Nexus quick-access.

## Building from source

Cross-compiles from Linux to Windows via MinGW. Requires CMake ≥ 3.20.

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

Output: `build/CastAway.dll`.

## Optional companions

- **[Hoard & Seek](https://raidcore.gg)** — drives the catch-progress tracking.
- **[Events:Alerts](https://raidcore.gg)** — enables real-time optimistic updates when you reel a fish in.

Neither is required — Cast Away gracefully falls back to API-only data if they're missing.

## Known imprecisions

- **Fractured Fish, Balance Fish, and Bastion Fish (Horn of Maguuma) are unavoidably region-level.** Each is confirmed via its own wiki page's "Found in the waters of X" text to genuinely span 2 maps (Fractured Fish: Skywatch Archipelago + Inner Nayos; Balance Fish and Bastion Fish: Amnytas + Inner Nayos) — a single `Fish::map` field can't encode "map A or map B", so these three show on every Horn of Maguuma map rather than just their real two.
- **Newer zones can retroactively add catches for older fish, and there may be more we haven't found.** A fish's "Found in the waters of X" text reflects its own expansion's data, not a guarantee against a *later* expansion adding it to a new zone's catch pool without that text ever being updated — confirmed via `Fish::extraRegions` for 34 fish so far: 5 Shiverpeak Mountains fish (Snow Crab, Boreal Cod, King Salmon, Alabaster Oscar, Halibut) and 27 Kryta/Kaineng/Maguuma/Crystal Desert fish are also caught at Skywatch Archipelago's/Janthir's own named holes (cross-referenced by exact species-name match against those holes' own catch-tables — reliable for "is this older, already-confirmed fish also listed here", unlike trusting a hole's catch-table for a *new* fish's primary origin, which is not reliable), and Fugu Fish/Spotted Stingray (Seitung Province) are also caught at Shipwreck Strand (confirmed via that hole's catch-table and a direct in-game catch). There's likely more of this pattern in older regions we haven't cross-referenced yet.
- **Guild halls only recognize one upgrade tier per hall.** Gilded Hollow, Lost Precipice, Windswept Haven, and Isle of Reflection each span ~5-6 map IDs (one per upgrade tier), all with the same real fishing content per the wiki, but the marker pack only covers one tier per hall. World/Saltwater "Here" matches work on the covered tier but won't show on the others. Worth adding the rest as an explicit allowlist (like Thousand Seas Pavilion) if it turns out to matter in practice.

---
## License

MIT

## Third-party credits & licenses

- **Fishing-hole coordinates** — sourced from the *All-In-One* marker pack by **[Tekkit's Workshop](https://www.tekkitsworkshop.net/markers/all-in-one-marker-pack)**. The pack itself is free to download and the underlying coordinates are factual game-world data; credit and link are given here in gratitude for the curation effort.
- **Waypoint and POI metadata** — fetched live from the official **[Guild Wars 2 API](https://wiki.guildwars2.com/wiki/API:Main)** (`/v2/continents/1/floors/*/regions`).
- **Map tiles** — served by ArenaNet's tile service (`tiles.guildwars2.com`).
- **Icons** — fish, bait, fillet, bonus-drop, and waypoint icons rendered from `render.guildwars2.com`. All Guild Wars 2 assets &copy; ArenaNet / NCSOFT.
- **[Dear ImGui](https://github.com/ocornut/imgui)** — UI rendering. MIT License.
- **[nlohmann/json](https://github.com/nlohmann/json)** — JSON parsing. MIT License.
- **[Nexus](https://raidcore.gg/Nexus)** by Raidcore — addon framework providing the load/unload hooks, texture and MumbleLink APIs.

Guild Wars 2 &copy; ArenaNet, LLC. Cast Away is an unofficial fan-made addon and is not affiliated with or endorsed by ArenaNet or NCSOFT.
