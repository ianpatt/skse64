# SKSE64 Performance Optimizations

This branch contains performance optimizations for SKSE64. Works with any modlist size - benefits scale with plugin count.

## Summary of Changes

All changes are **backward compatible** with no API or save format modifications.

### Data Structure Optimizations

Converted ordered containers to unordered equivalents for O(1) operations:

| Location | Change | Impact |
|----------|--------|--------|
| `PapyrusEvents.h` | `std::map` → `std::unordered_map` for event dispatch | O(log n) → O(1) event lookups |
| `PapyrusEvents.h` | `std::set` → `std::unordered_set` for event registration | O(log n) → O(1) register/unregister |
| `PluginManager.cpp` | Added `std::unordered_map` for plugin name lookups | O(n) → O(1) plugin lookups |
| `Serialization.cpp` | Added `std::unordered_map` for UID lookups | O(n²) → O(n) save/load |
| `ScaleformCallbacks.h` | `std::map` → `std::unordered_map` | O(log n) → O(1) |
| `Hooks_Scaleform.cpp` | `std::map` → `std::unordered_map` | O(log n) → O(1) |
| `Hooks_Scaleform.cpp` | `std::list` → `std::vector` | Better cache locality |
| `PapyrusQuest.cpp` | `std::map` → `std::unordered_map` | O(log n) → O(1) |
| `PapyrusRace.cpp` | `std::map` → `std::unordered_map` | O(log n) → O(1) |
| `PapyrusHeadPart.cpp` | `std::map` → `std::unordered_map` | O(log n) → O(1) |
| `PapyrusKeyword.cpp` | `std::map` → `std::unordered_map` | O(log n) → O(1) |
| `PapyrusObjectReference.cpp` | `std::map` → `std::unordered_map` | O(log n) → O(1) |
| `PapyrusActor.cpp` | `std::set` → `std::unordered_set` | O(log n) → O(1) |
| `PapyrusGameData.cpp` | `std::set` → `std::unordered_set` | O(log n) → O(1) |
| `PapyrusObjects.h` | `std::map` → `std::unordered_map` | O(log n) → O(1) |

### Memory Alignment

Added `alignas(64)` cache-line alignment to frequently accessed globals to reduce false sharing:

- Event registration globals (`PapyrusEvents.cpp`)
- Plugin manager globals (`PluginManager.cpp`)
- UI queue and locks (`Hooks_UI.cpp`)
- Input hook data (`Hooks_DirectInput8Create.cpp`)
- Task queue (`Hooks_Threads.cpp`)
- Critical sections in Papyrus caches

### Memory Pre-allocation

- Increased `PluginManager` vector reserve from 5 to 128
- Increased listener resize increment from 5 to 32
- Increased task memory pools from 10 to 32 entries
- Added string reserves for path concatenation

### Hash Function

Added `std::hash` specialization for `BSFixedString` in `HashUtil.h`:
```cpp
namespace std {
    template<>
    struct hash<BSFixedString> {
        size_t operator()(const BSFixedString& str) const {
            return hash<const char*>()(str.data);
        }
    };
}
```

This enables using `BSFixedString` as a key in `std::unordered_map`.

## Expected Benefits

These optimizations improve:

1. **Frame time consistency** - Reduced microstutters from O(log n) lookups during event dispatch
2. **1% and 0.1% lows** - More consistent worst-case frame times
3. **Scalability** - O(1) lookups benefit all modlist sizes; improvements scale with plugin count
4. **Save/load performance** - O(n) instead of O(n²) for UID matching

**Note:** Average FPS improvements are unlikely to be significant. The main benefit is reduced variance in frame times (fewer stutters). Works with any modlist size - small, medium, or large.

## Testing

Tested with:
- 176 SKSE plugins loaded
- All plugins load correctly
- No errors in SKSE logs
- Game runs stable

## Compatibility

- Drop-in replacement for SKSE 2.0.20
- No API changes
- No save format changes
- All existing SKSE plugins remain compatible

## Files Changed

Core changes:
- `skse64/PapyrusEvents.h` - Event dispatch and registration containers
- `skse64/PapyrusEvents.cpp` - Global alignment
- `skse64/PluginManager.h` - Added hash map member
- `skse64/PluginManager.cpp` - Plugin lookup optimization, alignment
- `skse64/Serialization.cpp` - UID hash map for save/load
- `skse64/HashUtil.h` - BSFixedString hash function
- `skse64/Hooks_UI.cpp` - Alignment
- `skse64/Hooks_DirectInput8Create.cpp` - Alignment
- `skse64/Hooks_Threads.cpp` - Alignment, task queue
- `skse64/Hooks_Scaleform.cpp` - Container conversions
- `skse64/ScaleformCallbacks.h` - Container conversion
- `skse64/InternalTasks.cpp` - Memory pool sizes, alignment
- `skse64/GlobalLocks.cpp` - Alignment
- Various Papyrus*.cpp files - Cache container conversions, alignment

## License

Same license as SKSE64. See `skse64_license.txt`.
