# Stat Formula

This document defines how `Pokemon::recomputeStats()` (`include/Pokemon/pokemon.h`) derives calculated stats from base stats, IVs, EVs, level, and nature. The formula targets the Generation 9 (Scarlet/Violet) main-series calculation. For when recalculation runs and how current HP is adjusted, see [the stat recalculation contract](./pokemon-ownership.md#stat-recalculation-contract).

## Formula

For each stat, with $B$ the base stat, $I$ the IV, $E$ the EV, $L$ the level, and $N$ the nature multiplier:

$$
\text{core} = \left\lfloor \frac{\left(2B + I + \left\lfloor \frac{E}{4} \right\rfloor\right) \times L}{100} \right\rfloor
$$

$$
\text{HP} = \text{core} + L + 10
$$

$$
\text{Stat} = \left\lfloor \left(\text{core} + 5\right) \times N \right\rfloor \quad \text{for Attack, Defense, Sp. Atk, Sp. Def, Speed}
$$

| Term        | Constant                          | Value |
| ----------- | --------------------------------- | ----- |
| $2$         | `CALCULATED_NUMERATOR_MULTIPLIER` | 2     |
| $4$         | `CALCULATED_EV_DIVISOR`           | 4     |
| $100$       | `CALCULATED_NUMERATOR_DIVISOR`    | 100   |
| HP offset   | `CALCULATED_HEALTH_OFFSET`        | 10    |
| Stat offset | `CALCULATED_STAT_OFFSET`          | 5     |

All constants live in `include/Configuration/constants.h`.

## Inputs and bounds

| Input     | Range                            | Enforced by                                                                                                        |
| --------- | -------------------------------- | ------------------------------------------------------------------------------------------------------------------ |
| Base stat | 1–255 per stat                   | `updateBaseStats()` and the base-stat setters clamp to `MIN_POKEMON_BASE_STAT_VALUE`/`MAX_POKEMON_BASE_STAT_VALUE` |
| Level     | 1–100                            | `updateLevel()` clamps to `MIN_POKEMON_LEVEL`/`MAX_POKEMON_LEVEL`                                                  |
| IV        | 0–31 per stat                    | `isValidPokemonIV()`; invalid values leave the stored IVs unchanged                                                |
| EV        | 0–252 per stat, 510 total        | `isValidPokemonEV()`/`isValidPokemonEVArray()`; invalid values leave the stored EVs unchanged                      |
| Nature    | 0.9, 1.0, or 1.1 per non-HP stat | `NATURE_STAT_WEAKNESS_MULTIPLIER`, `NATURE_STAT_BASE_MULTIPLIER`, `NATURE_STAT_BOOST_MULTIPLIER`                   |

## Rounding points

The formula rounds down at exactly three points, in this order:

1. **EV contribution** — $\lfloor E / 4 \rfloor$. Implemented as unsigned integer division; every 4 EVs add 1 point before level scaling (252 EVs → 63).
2. **Level scaling** — $\lfloor (\ldots) \times L / 100 \rfloor$. Implemented as unsigned integer division of the full product; the level is applied before dividing, never after.
3. **Nature** — $\lfloor (\text{core} + 5) \times N \rfloor$. Implemented as a `double` multiplication truncated by `static_cast<us>`. Truncation equals floor because every operand is non-negative.

The first two steps use no `std::floor`; integer division provides the rounding.

The nature step is applied **after** the `+ 5` offset, matching Gen 9. The `double` values of 1.1 and 0.9 are both stored marginally above their decimal values, so a product whose exact result is an integer (for example $50 \times 1.1 = 55$) never truncates to one below it.

## Nature and HP

Nature multipliers are stored per stat, including an HP slot, and `recomputeStats()` multiplies HP by that slot. Every built-in nature uses 1.0 for HP, so built-in natures never change HP. A custom nature with a non-neutral HP multiplier would change HP, which Gen 9 never does.

When `MAX_NATURES_PER_POKEMON` is greater than 1, each nature is applied in turn, truncating after each multiplication. This is a project extension; Gen 9 has exactly one nature. The default capacity is 1.

## Integer types

| Value                                     | Type              | Maximum reachable              |
| ----------------------------------------- | ----------------- | ------------------------------ |
| Base stat, IV, EV, level                  | `us` (`uint16_t`) | 255 / 31 / 252 / 100           |
| $I + \lfloor E/4 \rfloor$                 | `ui` (`uint32_t`) | 94                             |
| $(2B + I + \lfloor E/4 \rfloor) \times L$ | `ui` (`uint32_t`) | 60,400                         |
| Calculated stat                           | `us` (`uint16_t`) | 714 (HP), 669 (boosted non-HP) |

Maximums assume base 255, IV 31, EV 252, level 100. Every intermediate and result fits its type without overflow.

## Worked example

Base 100 in every stat, level 20, Hasty nature (+Speed, −Defense):

| Stat    | IV  | EV  | $\lfloor E/4 \rfloor$ | $(2B + I + \lfloor E/4 \rfloor) \times L / 100$ | Pre-nature   | Nature | Result |
| ------- | --- | --- | --------------------- | ----------------------------------------------- | ------------ | ------ | ------ |
| HP      | 0   | 30  | 7                     | 207 × 20 / 100 = 41.4 → 41                      | 41 + 20 + 10 | —      | 71     |
| Attack  | 15  | 52  | 13                    | 228 × 20 / 100 = 45.6 → 45                      | 50           | 1.0    | 50     |
| Defense | 30  | 85  | 21                    | 251 × 20 / 100 = 50.2 → 50                      | 55           | 0.9    | 49     |
| Sp. Atk | 20  | 47  | 11                    | 231 × 20 / 100 = 46.2 → 46                      | 51           | 1.0    | 51     |
| Sp. Def | 2   | 241 | 60                    | 262 × 20 / 100 = 52.4 → 52                      | 57           | 1.0    | 57     |
| Speed   | 5   | 50  | 12                    | 217 × 20 / 100 = 43.4 → 43                      | 48           | 1.1    | 52     |
