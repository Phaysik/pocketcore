# Configuration Boundary

This document defines the boundary between compile-time storage capacities in
`include/Configuration/constants.h` and runtime semantic rules. A storage
capacity describes the shape of an object or registry and must remain a
compile-time constant. A semantic rule describes what a battle permits and
belongs in `PocketCore::Ruleset::RulesetPolicy` when it is intended to vary by
ruleset.

## Audit

| Constant                                | Classification               | Owning storage or semantic rule           | Runtime policy                           |
| --------------------------------------- | ---------------------------- | ----------------------------------------- | ---------------------------------------- |
| `MAX_TYPES`                             | Registry capacity            | Type registry and type-chart arrays       | None                                     |
| `MAX_ABILITIES`                         | Registry capacity            | Ability registry                          | None                                     |
| `MAX_ITEMS`                             | Registry capacity            | Item registry                             | None                                     |
| `MAX_MOVES`                             | Registry capacity            | Move registry                             | None                                     |
| `MAX_STATUSES`                          | Registry capacity            | Status registry                           | None                                     |
| `MAX_NATURES`                           | Registry capacity            | Nature registry                           | None                                     |
| `MAX_POKEMON`                           | Registry capacity            | Pokemon registry                          | None                                     |
| `MAX_LEARNSETS`                         | Registry capacity            | Learnset registry                         | None                                     |
| `MAX_WEATHERS`                          | Registry capacity            | Weather registry                          | None                                     |
| `MAX_TERRAINS`                          | Registry capacity            | Terrain registry                          | None                                     |
| `MAX_MULTIPLIERS`                       | Registry capacity            | Multiplier registry                       | None                                     |
| `MAX_EFFECTS`                           | Registry capacity            | Effect registry                           | None                                     |
| `MAX_TYPES_PER_POKEMON`                 | Physical storage capacity    | Pokemon type array                        | None                                     |
| `MAX_ABILITIES_PER_POKEMON`             | Physical storage capacity    | Pokemon ability array                     | None                                     |
| `MAX_ABILITY_POOL_PER_POKEMON`          | Physical storage capacity    | Pokemon ability-pool array                | None                                     |
| `MAX_ITEMS_PER_POKEMON`                 | Physical storage capacity    | Pokemon item array                        | None                                     |
| `MAX_MOVES_PER_POKEMON`                 | Physical storage capacity    | Pokemon move and PP arrays                | None                                     |
| `MAX_NATURES_PER_POKEMON`               | Physical storage capacity    | Pokemon nature arrays                     | None                                     |
| `MAX_NON_VOLATILE_STATUSES_PER_POKEMON` | Physical storage capacity    | Pokemon non-volatile status array         | `RulesetPolicy::mMaxNonVolatileStatuses` |
| `MAX_VOLATILE_STATUSES_PER_POKEMON`     | Physical storage capacity    | Volatile-status storage                   | `RulesetPolicy::mMaxVolatileStatuses`    |
| `MAX_ACTIVE_WEATHERS_ON_FIELD`          | Physical storage capacity    | Battle-state weather array                | `RulesetPolicy::mMaxWeathers`            |
| `MAX_ACTIVE_TERRAINS_ON_FIELD`          | Physical storage capacity    | Battle-state terrain storage              | `RulesetPolicy::mMaxTerrains`            |
| `MAX_ACTIVE_SLOTS_PER_SIDE`             | Physical storage capacity    | Battle-state active-side storage          | `RulesetPolicy::mMaxSideSize`            |
| `MAX_SUPPRESSION_RULES_PER_TRIGGER`     | Physical storage capacity    | Effect-trigger suppression array          | None                                     |
| `MAX_STAT_STAGES`                       | Physical storage capacity    | Stat-stage cache index domain             | None                                     |
| `MAX_STAGE_CACHE`                       | Physical storage capacity    | Stat-stage multiplier cache               | None                                     |
| `MAX_CRITICAL_HIT_VALUE`                | Runtime semantic restriction | Critical-hit random-roll upper bound      | None                                     |
| `MAX_ACCURACY_HIT_VALUE`                | Runtime semantic restriction | Accuracy random-roll upper bound          | None                                     |
| `RANDOMIZATION_MULTIPLIER_MAX_VALUE`    | Runtime semantic restriction | Random damage multiplier upper bound      | None                                     |
| `MAX_IV_STAT_VALUE`                     | Runtime semantic restriction | Persistent Pokemon individual-value bound | None                                     |

The remaining `MIN_*` and value constants in `constants.h` are also runtime mechanics, but they are outside this `MAX_*` capacity audit. They remain compile-time constants because they describe global mechanics or persistent Pokemon data invariants rather than battle-specific policy choices.

## Rules

1. Registry capacities and fixed-array extents remain compile-time constants.
2. A runtime rule must not use a registry capacity or unrelated storage extent as its gameplay limit.
3. A policy field that limits a fixed storage array must be validated against that array's capacity. The policy selects the legal semantic limit; it does not resize the storage.
4. New configurable battle rules belong in `RulesetPolicy`, with validation, defaults, and tests added at that boundary. Global mechanics, persistent Pokemon data invariants, and cache-shaping bounds do not belong there.

`RulesetPolicy` currently follows these rules for active side size, active weathers, active terrains, and status limits. The static arrays remain sized by the corresponding constants, while the policy fields select the permitted runtime values.
Stat stages remain bounded by the cache domain, and individual values remain bounded by the Pokemon data model, independently of battle rulesets.
