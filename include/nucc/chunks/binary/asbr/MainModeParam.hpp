#ifndef KOJO_NUCC_ASBR_MAINMODEPARAM
#define KOJO_NUCC_ASBR_MAINMODEPARAM

#include "../binary_data.hpp"
#include "utils.hpp"

namespace nucc {
    namespace ASBR {
        
class MainModeParam : public Binary_Data {
public:
    struct Entry {
        enum class Panel_Type {
            NORMAL = 0,
            EXTRA = 1,
            BOSS = 2
        };

        enum class Special_Rule {
            BLANK               = -1,   // blank
            NONE                = 0,    // No special rules are in place for this battle.
            ONE_ROUND           = 1,    // 1-round battle.
            THREE_ROUND         = 2,    // 3-round battle.
            FIVE_ROUND          = 3,    // 5-round battle.
            COUNT_30            = 4,    // Win within a count of 30.
            COUNT_60            = 5,    // Win within a count of 60.
            COUNT_90            = 6,    // Win within a count of 90.
            PL_HH_REFILL        = 7,    // Player's HH Gauge continuously refills.
            OPP_HH_REFILL       = 8,    // Opponent's HH Gauge continuously refills.
            PL_HEALTH_REFILL    = 9,    // Player's health continuously refills.
            OPP_HEALTH_REFILL   = 10,   // Opponent's health continuously refills.
            PL_HEALTH_DRAIN     = 11,   // Player's health continuously drains.
            OPP_HEALTH_DRAIN    = 12,   // Opponent's health continuously drains.
            PL_NO_GUARD         = 13,   // Player can't guard.
            OPP_NO_GUARD        = 14,   // Opponent can't guard.
            PL_ATK_DOWN         = 15,   // Player's attack decreased.
            OPP_ATK_DOWN        = 16,   // Opponent's attack decreased.
            PL_ATK_UP           = 17,   // Player's attack increased.
            OPP_ATK_UP          = 18,   // Opponent's attack increased. 
            PL_DEF_DOWN         = 19,   // Player's defense decreased.
            OPP_DEF_DOWN        = 20,   // Opponent's defense decreased.
            PL_DEF_UP           = 21,   // Player's defense increased.
            OPP_DEF_UP          = 22,   // Opponent's defense increased.
            PL_HP_10_PERCENT    = 23,   // Player starts with 10% health.
            OPP_HP_10_PERCENT   = 24,   // Opponent starts with 10% health.
            PL_HP_30_PERCENT    = 25,   // Player starts with 30% health.
            OPP_HP_30_PERCENT   = 26,   // Opponent starts with 30% health.
            PL_HP_50_PERCENT    = 27,   // Player starts with 50% health.
            OPP_HP_50_PERCENT   = 28,   // Opponent starts with 50% health.
            PL_HH_HAMON         = 29,   // Player's HH Gauge only fills by Ripple Breath.
            OPP_HH_HAMON        = 30,   // Opponent's HH Gauge only fills by Ripple Breath.
            NO_GEIL_3HRS01      = 229,  // Hol Horse can't use J. Geil moves.
            ULTIMATE_2KRS01     = 230,  // Kars starts in Ultimate Thing Mode.
            ENRAGED_4JSK01      = 231,  // Josuke Higashikata 4 starts in Enraged Mode.
            REQUIEM_5GRN01      = 232,  // Giorno starts in Gold Experience Requiem.
            CMOON_6PCI01        = 233,  // Pucci starts in C-MOON.
            MIH_6PCI01          = 234,  // Pucci starts in Made in Heaven.
            CORPSE_7VTN01       = 235,  // Valentine starts with 3 corpse parts.
            ONLY_SM_7DIO01      = 236,  // Diego starts in Scary Monsters state (does not return to normal).
            NO_HAMON            = 237,  // Player cannot use Ripple (pushing 'S' will have no effect).
            MAX_MAGNET_3MRA01   = 238,  // Mariah starts with a maxed out Magnetization Gauge that does not decrease.
            INF_FLIGHT_3PSP01   = 239,  // Pet Shop's Flight Gauge does not decrease.
            LOVE_DELUXE_4YKK01  = 240,  // Yukako Yamagishi starts in Love Deluxe state (HH Gauge does not decrease).
            PERMANENT_AGE       = 241,  // Opponent's Aging Gauge does not decrease.
            NO_HH_STAND_ON      = 242,  // Player's HH Gauge does not fill while their Stand is ON.
            PL_ASS_ATK_DOWN     = 243,  // Player's Assist attack decreased.
            PL_ASS_ATK_UP       = 244,  // Player's Assist attack increased.
            OP_ASS_ATK_DOWN     = 245,  // Opponent's Assist attack decreased.
            OP_ASS_ATK_UP       = 246,  // Opponent's Assist attack increased.
            PL_HAMON_DMG_DOWN   = 247,  // Player's Ripple damage decreased.
            PL_HAMON_DMG_UP     = 248,  // Player's Ripple damage increased.
            OPP_HAMON_DMG_DOWN  = 249,  // Opponent's Ripple damage decreased.
            OPP_HAMON_DMG_UP    = 250,  // Opponent's Ripple damage increased.
            PL_VAMP_DMG_DOWN    = 251,  // Player's Vampirism damage decreased.
            PL_VAMP_DMG_UP      = 252,  // Player's Vampirism damage increased.
            PL_VAMP_HEAL_DOWN   = 253,  // Player's Vampirism recovery rate decreased.
            PL_VAMP_HEAL_UP     = 254,  // Player's Vampirism recovery rate increased.
            PL_FIRE_DMG_DOWN    = 255,  // Player's Fire damage decreased.
            PL_FIRE_DMG_UP      = 256,  // Player's Fire damage increased.
            NO_SNAIL_PUCCI      = 257,  // Pucci is immune to becoming a snail.
            OPP_VAMP_DMG_DOWN   = 302,  // Opponent's Vampirism damage decreased.
            OPP_VAMP_DMG_UP     = 303,  // Opponent's Vampirism damage increased.
            OPP_VAMP_HEAL_DOWN  = 304,  // Opponent's Vampirism recovery rate decreased.
            OPP_VAMP_HEAL_UP    = 305,  // Opponent's Vampirism recovery rate increased.
            OPP_FIRE_DMG_DOWN   = 306,  // Opponent's Fire damage decreased.
            OPP_FIRE_DMG_UP     = 307   // Opponent's Fire damage increased.
        };

        enum class Mission_Condition {
            BLANK                           = -1,   // blank
            NO_LOSSES                       = 32,   // Win without losing any rounds
            FIRST_ATTACK                    = 33,   // Land the First Attack
            COUNTERATTACK                   = 34,   // Land a counterattack
            HIT_COMBO_5                     = 35,   // Land a 5 hit combo
            HIT_COMBO_10                    = 36,   // Land a 10 hit combo
            HIT_COMBO_20                    = 37,   // Land a 20 hit combo
            HIT_COMBO_30                    = 38,   // Land a 30 hit combo
            HIT_COMBO_40                    = 39,   // Land a 40 hit combo
            HIT_COMBO_50                    = 40,   // Land a 50 hit combo
            HIT_COMBO_100                   = 41,   // Land a 100 hit combo
            HIT_COMBO_200                   = 42,   // Land a 200 hit combo
            HIT_COMBO_300                   = 43,   // Land a 300 hit combo
            STYLISH_EVADE                   = 44,   // Land a Stylish Evade
            TAUNT                           = 45,   // Execute a taunt
            STAND_ON                        = 46,   // Turn your Stand on
            DOWN_OPPONENT                   = 47,   // Down your opponent
            WIN_TIME_UP                     = 48,   // Win via Time Up
            LAUNCH_RUSH_MODE                = 49,   // Launch Rush Mode
            WIN_RUSH_MODE                   = 50,   // Win in Rush Mode
            USE_HHA                         = 51,   // Use a HHA
            USE_GHA                         = 52,   // Use a GHA
            LAND_GHA                        = 53,   // Land a GHA
            LAND_HHA                        = 54,   // Land a HHA
            WIN_HHA                         = 55,   // Win with a HHA
            WIN_GHA                         = 56,   // Win with a GHA
            ATK_OPP_STAND                   = 57,   // Attack the opponent's Stand
            ATK_STAND_ON                    = 58,   // Do a normal attack with Stand on
            POW_1ZPL01                      = 59,   // [Zeppeli] Land the "Pow!" throw
            ZOOM_PUNCH                      = 60,   // Land the "Zoom Punch" special
            HAMON_BREATH                    = 62,   // Use the "Ripple Breathing" style
            USE_SEAT_JUMP_1ZPL01            = 63,   // [Zeppeli] Use the "Seated Jump" special
            LAND_IMPUDENT_FOOL_1DIO01       = 64,   // [Dio1] Land the "You impudent fool!" throw
            LAND_FROZEN_TRACKS_1DIO01       = 65,   // [Dio1] Land the "Frozen in your tracks!" skill
            LAND_LUCK_PLUCK_1JNT01          = 66,   // [Jonathan] Land the "Sword of Luck and Pluck" skill
            USE_MERE_DOG_1DIO01             = 67,   // [Dio1] Use the "You are a mere dog!" special
            LAND_VAPOR_FREEZE_1DIO01        = 68,   // [Dio1] Land the "Vaporizing Freeze" skill
            LAND_HAMON_SKILL                = 69,   // Land a Ripple-enhanced skill
            LAND_NEXT_LINE_2JSP01           = 70,   // [Joseph2] Land the "Your next line is:" skill
            LAND_CLACKER_BOOMERANG_2JSP01   = 72,   // [Joseph2] Land the "Clacker Boomerang" skill
            LAND_DONT_GET_IT_2JSP01         = 73,   // [Joseph2] Land the "Don't you get it?" skill
            LAND_SMASH_YOU_GOOD_2JSP01      = 74,   // [Joseph2] Land the "I'll smash you good!" skill
            USE_HAMON_GUARD_2JSP01          = 76,   // [Joseph2] Use the "Ripple Guard" style
            USE_HEAT_MODE_2ESD01            = 77,   // [Esidisi] Use the "Heat Control Mode" style
            LAND_HOW_COULD_YOU_2ESD01       = 78,   // [Esidisi] Land the "How could youuuuuu!" skill
            LAND_BURNING_KING_2ESD01        = 79,   // [Esidisi] Land the "Burning King Mode" skill
            LAND_BUBBLE_CUTTER_2CSR01       = 80,   // [Caesar] Land the "Bubble Cutter" skill
            LAND_GLIDING_CUTTER_2CSR01      = 81,   // [Caesar] Land the "Gliding Bubble Cutter" skill
            LAND_CAT_STANCE_2CSR01          = 82,   // [Caesar] Land the "Cat Stance" skill
            USE_VAPOR_BARRIER_2WMU01        = 83,   // [Wamuu] Use the "Vapor Barrier" style
            ATK_BENT_BACKWARDS_2WMU01       = 84,   // [Wamuu] Attack from the "He bent backwards!" skill
            LAND_SNAKE_MUFFLER_2LSA01       = 85,   // [Lisa] Land the "Snake Muffler" skill
            LAND_VICTORY_EVERYTHING_2KRS01  = 86,   // [Kars] Land the "Victory is everything!" skill
            USE_BRILLIANT_BLADES_2KRS01     = 87,   // [Kars] Use the "Brilliant Bone Blades" style
            USE_NIGERUNDAYO_2JSP01          = 89,   // [Joseph2] Use the "I'm so outta here!" special
            USE_PREDATOR_WINGS_2KRS01       = 90,   // [Kars] Use the "Predator Wings" style
            LAND_YOU_IDIOT_2KRS01           = 91,   // [Kars] Land the "You IDIOT!" throw
            LAND_SUPREME_GENES_2KRS01       = 92,   // [Kars] Land the "The power of supreme genes!" skill
            LAND_MY_HAMON_2KRS01            = 93,   // [Kars] Land the "Feast your eyes on my Ripple!" skill
            LAND_INCREDIBLE_POWER_3JTR01    = 94,   // [Jotaro3] Land the "This incredible power...!" throw
            LAND_RED_BIND_3AVD01            = 95,   // [Avdol] Land the "Red Bind!" throw
            LAND_THE_HARD_WAY_3AVD01        = 96,   // [Avdol] Land the "Must I do this the hard way?" skill
            WIN_MY_STAND_JUDGE_3JTR01       = 97,   // [Jotaro3] Win with the "My Stand will be the judge!" skill
            LAND_NO_MATCH_3KKY01            = 99,   // [Kakyoin] Land the "No Match for My Stand" throw
            LAND_EMERALD_SPLASH_3KKY01      = 100,  // [Kakyoin] Land the "Emerald Splash" skill
            LAND_CROSSFIRE_HURRICANE_3AVD01 = 101,  // [Avdol] Land the "Crossfire Hurricane" skill
            WIN_CROSSFIRE_VARIATION_3AVD01  = 102,  // [Avdol] Win with the "Crossfire Hurricane Variation" skill
            BLOCK_REAP_AS_YOU_SOW_3PLN01    = 103,  // [Polnareff] Block shots with "Reap as you sow!" skill
            LAND_BURN_TO_CINDERS_3AVD01     = 104,  // [Avdol] Land the "I'll burn you to cinders!" skill
            LAND_J_GEIL_3HHS01              = 105,  // [Hol Horse] Land the "You're up, J. Geil!" skill
            LAND_BULLETS_ARE_STAND_3HHS01   = 106,  // [Hol Horse] Land the "Even my bullets are a Stand!" skill
            LAND_SLICE_AND_DICE_3PLN01      = 107,  // [Polnareff] Land the "Slices and dices!" skill
            WIN_SHISH_KABOB_3PLN01          = 108,  // [Polnareff] Win with the "You shall be shish kabob!" skill
            USE_SAND_KICK_3PLN01            = 109,  // [Polnareff] Use the "Sand Kick" special
            USE_GUN_BEATS_SWORD_3HHS01      = 110,  // [Hol Horse] Use the "Gun beats sword" special
            LAND_SHISH_KABOB_3PLN01         = 111,  // [Polnareff] Land the "You shall be shish kabob!" skill
            USE_BE_RIGHT_BACK_3HHS01        = 112,  // [Hol Horse] Use the "I'll be right back!" special
            LAND_HIEROPHANT_BARRIER_3KKY01  = 113,  // [Kakyoin] Land the "Hierophant's Barrier" skill
            LAND_THIS_IS_THE_WORLD_3DIO01   = 114,  // Land the "This is... The World!" skill
            USE_STOP_TIME_3DIO01            = 115,  // [DIO3] Use the "Stop, time!" style
            LAND_IN_FRONT_OF_ME_3DIO01      = 116,  // [DIO3] Land the "Right in front of me?!" skill
            LAND_ORAAA_3JTR01               = 117,  // [Jotaro3] Land the "ORAAA!" skill
            LAND_ONE_MORE_3DIO01            = 119,  // [DIO3] Land the "And one more just in case!" skill
            LAND_HURRY_UP_AND_DIE_3PLN01    = 120,  // [Polnareff] Land the "Hurry up and die!" skill
            USE_SO_CLOSE_3DIO01             = 121,  // [DIO3] Use the "So close!" special
            CANCEL_TIMESTOP_3JTR01          = 122,  // [DIO3] Use the "So close!" special
            USE_THIS_GREATEST_HIGH_3DIO01   = 123,  // [DIO3] Use the "Now THIS is the greatest high!" style
            USE_SUITS_ME_WELL_3DIO01        = 124,  // [DIO3] Use the "Oh, this suits me well!" skill
            LAND_NASTY_HABIT_3IGY01         = 125,  // [Iggy] Land the "A nasty habit" skill
            USE_FIGHTING_ON_RUN_3JSP01      = 126,  // [Joseph3] Use the "Fighting on the run!" style
            USE_CUNNING_TACTICIAN_3JSP01    = 127,  // [Joseph3] Use the "Cunning Tactician" skill
            USE_HES_GONE_3DIO01             = 128,  // [DIO3] Use the "He's... gone!" skill
            LAND_DIO_DOPPELGANGER_3IGY01    = 129,  // [Iggy] Land the "Dio Doppelganger" skill
            LAND_WRETCHED_BEAST_3VNI01      = 130,  // [Ice] Land the "You Wretched Beast!" skill
            LAND_HOW_DARE_YOU_3VNI01        = 131,  // [Ice] Land the "How dare you!" skill
            LAND_PACK_FOR_HELL_3PLN01       = 132,  // [Polnareff] Land the "Start packing for hell" skill
            LAND_MISERABLE_CUR_3PLN01       = 133,  // [Polnareff] Land the "You miserable cur!" skill
            LAND_KISS_MY_SWORD_3PLN01       = 134,  // [Polnareff] Land the "Kiss my sword!" skill
            LAND_START_WITH_LEGS_3VNI01     = 135,  // [Ice] Land the "Start with the legs!" skill
            LAND_VICIOUS_ARM_3VNI01         = 136,  // [Ice] Land the "Vicious arm!" skill
            LAND_GAOOHHHN_4OKY01            = 137,  // [Okuyasu] Land the "Gaoohhhn!" skill
            LAND_GOT_YA_RUNT_4OKY01         = 138,  // [Okuyasu] Land the "Got ya, runt!" skill
            LAND_SCRAPE_YOU_AWAY_4OKY01     = 139,  // [Okuyasu] Land the "I'll scrape you away!" skill
            LAND_GET_ELECTRIC_4OTI01        = 140,  // [Akira] Land the "Time to get electric, baby!" throw
            USE_DRAG_YOU_IN_4OTI01          = 141,  // [Akira] Use the "I'll drag you in!" skill
            USE_CHARGE_4OTI01               = 142,  // [Akira] Use the "Charge!" special
            LAND_DORA_4JSK01                = 143,  // [Josuke4] Land the "DORA!" skill
            LAND_DORARARARARARARA_4JSK01    = 144,  // [Josuke4] Land the "DORARARARARARARA!" skill
            MAX_ELECTRICITY_4OTI01          = 145,  // [Akira] Reach maximum electric power
            USE_RIGHT_ROCKOUT_4OTI01        = 146,  // [Akira] Use the "Right hand rockout" special
            USE_WHACK_A_MOLE_4OTI01         = 147,  // [Akira] Use the "Whack-A-Mole Champion" skill
            USE_SIZZLE_4KOI01               = 148,  // [Koichi] Use the "Sizzle!" skill
            LAND_SAFETY_LOCK_4RHN01         = 149,  // [Rohan] Land the "Safety Lock" skill
            USE_KICKING_YOUR_ASS_4JSK01     = 150,  // [Josuke4] Use the "I'm not done kicking your ass!" special
            LAND_MANGA_HERO_4RHN01          = 152,  // [Rohan] Land "What would I do if I were a manga hero?" skill
            LAND_NORMAL_MODE_HIT            = 154,  // Land a hit in normal mode
            LAND_ANYTHING_I_WANT_4SGI01     = 155,  // [Shigechi] Land the "I can get anything I want!" skill
            LAND_HARD_WAY_4SGI01            = 156,  // [Shigechi] Land the "Learn the hard way!" skill
            LAND_DONT_UNDERESTIMATE_4SGI01  = 157,  // [Shigechi] Land the "Don't underestimate Harvest!" skill
            LAND_MOVE_AND_REGRET_4SGI01     = 158,  // [Shigechi] Land the "Move and you'll regret it!" skill
            USE_DONT_COME_CLOSER_4SGI01     = 159,  // [Shigechi] Use the "Don't come any closer!" skill
            LAND_PROTECT_THEM_4SGI01        = 160,  // [Shigechi] Land the "I'll protect them!" skill
            LAND_BLOWN_TO_PIECES_4KIR01     = 161,  // [Kira] Land the "Blown to pieces!" skill
            LAND_HA_4KIR01                  = 162,  // [Kira] Land the "Ha!" skill
            SHA_THREE_FREEZE_4KOI01         = 163,  // [Koichi] Hit Sheer Heart Attack with "Echoes, 3 Freeze!"
            USE_AIR_PRESSURE_4KOI01         = 164,  // [Koichi] Use the "This air pressure?!" skill
            LAND_HOW_ITS_DONE_4KIR01        = 165,  // [Kira] Land the "Show you how it's done!" throw
            LAND_FATE_DEFEATED_4KIR01       = 166,  // [Kira] Land the "It's your fate to be defeated" special
            LAND_YOU_BASTARD_4KIR01         = 167,  // [Kira] Land the "You bastard!" skill
            LAND_CALM_AND_COOL_4KIR01       = 168,  // [Kira] Land the "Calm and cool" skill
            LAND_DORA_SPL_4JSK01            = 169,  // [Josuke4] Land the "DORA...!" special
            BLOCK_HEALS_QUICK_4JSK01        = 170,  // [Josuke4] Block a projectile with "He heals pretty quick!" skill
            USE_AS_YOU_WERE_4JSK01          = 171,  // [Josuke4] Use the "As you were!" skill
            LAND_IMPACT_BOMB_4KSK01         = 172,  // [Kosaku] Land the "Impact Ignition Bomb" skill
            LAND_FIGHT_IT_OUT_4KSK01        = 173,  // [Kosaku] Land the "I'll have to fight it out!" skill
            USE_TRAJECTORY_CHANGE_4KSK01    = 174,  // [Kosaku] Use the "Trajectory Change" skill
            LAND_TASTE_OF_PAIN_5GRN01       = 175,  // [Giorno] Land the "Savor the taste of pain!" HHA
            LAND_NOT_MY_ARM_5BCT01          = 176,  // [Bruno] Land the "This isn't my arm" skill
            LAND_INTERROGATION_5BCT01       = 177,  // [Bruno] Land the "From interrogation to torture" throw
            LAND_GOT_YOU_5BCT01             = 178,  // [Bruno] Land the "I've got you!" special
            USE_CLOSE_ZIPPER_5BCT01         = 179,  // [Bruno] Use the "Close, zipper!" skill
            USE_ERASING_TIME_5DVL01         = 180,  // [Diavolo] Use the "Erasing time" style
            LAND_ESCAPE_DESTRUCTION_5GRN01  = 181,  // [Giorno] Land the "Can you escape destruction?" skill
            LAND_NEVER_REACH_TRUTH_5GRN01   = 182,  // [Giorno] Land the "You will never reach the truth!" skill
            LAND_USELESS_USELESS_5GRN01     = 183,  // [Giorno] Land the "Useless, useless, useless, useless!" skill
            WIN_USELESS_5GRN01              = 184,  // [Giorno] Win with the "USELESS!" skill
            LAND_THE_END_5DVL01             = 185,  // [Diavolo] Land the "This is the end!" skill
            WIN_HANDCUFF_DEATHMATCH_6JLN01  = 186,  // [Jolyne] Win in the "Handcuff Deathmatch" style
            KEEP_DISC                       = 187,  // Win without losing your DISC
            GET_CMOON_6PCI01                = 188,  // [Pucci Final] Enter C-MOON via maxing out The Green Baby Gauge
            LAND_PIERCED_FINGER_6PCI01      = 189,  // [Pucci Final] Land the "Pierced through the finger" special
            USE_WONT_CUT_MY_STRING_6JLN01   = 190,  // [Jolyne] Use the "I won't cut my string yet!" skill
            USE_STRINGING_FOR_RIDE_6JLN01   = 191,  // [Jolyne] Use the "Stringing me for a ride!" special
            LAND_ONE_PUNCH_6PCI01           = 192,  // [Pucci Final] Land the "Only need one punch" skill
            LAND_LOST_BOTH_ARMS_6PCI01      = 193,  // [Pucci Final] Land the "Now you've lost both arms!" skill
            LAND_GO_TO_HELL_6ANS01          = 194,  // [Anasui] Land the "Go to hell!" skill
            GET_MIH_6PCI01                  = 195,  // [Pucci Final] Complete "Made in Heaven"
            LAND_HERE_IT_COMES_6PCI01       = 196,  // [Pucci Final] Land the "Here it comes!" skill
            USE_STRING_BARRIER_6JLN01       = 197,  // [Jolyne] Use the "String Barrier" skill
            USE_WHERE_IS_HE_6PCI01          = 198,  // [Pucci Final] Use the "Where is he?!" skill
            USE_THE_KNIVES_6PCI01           = 199,  // [Pucci Final] Use the "The knives!" skill
            LEVEL_3_CORPSE                  = 200,  // Bring the "Holy Corpse" style to Level 3
            USE_NEXT_DIMENSION_7VTN01       = 201,  // [Valentine] Use the "Hiding in the next dimension" skill
            USE_BACK_FROM_OTHER_SIDE_7VTN01 = 202,  // [Valentine] Use the "Back from the other side" special
            LAND_BRING_DOWN_HAMMER_2JSP01   = 203,  // [Joseph2] Land the "Time to bring down the hammer" skill
            LAND_REBUFF_OVERDRIVE_2JSP01    = 204,  // [Joseph2] Land the "Rebuff Overdrive" special
            USE_SEE_WITH_WIND_2WMU01        = 205,  // [Wamuu] Use the "I shall see with the wind" style
            LAND_OWN_ILLUSION_2WMU01        = 206,  // [Wamuu] Land the "Victim of your own delusion" skill
            LAND_HOMING_MISSILE_4JSK01      = 207,  // [Josuke4] Land "Meet my homing missile!" from the HHA
            USE_TIME_ERASED_5DVL01          = 208,  // [Diavolo] Use the "Time has been erased..." skill
            SINGLE_ROUND_ASSIST_1           = 209,  // Call 1 Assist in a single round
            SINGLE_ROUND_ASSIST_3           = 210,  // Call 3 Assists in a single round
            ASSIST_FINAL_BLOW               = 211,  // Land the final blow with an Assist
            MAX_MAGNETISM_3MRA01            = 212,  // [Mariah] Reach Level 3 with "Magnetization Gauge" style
            USE_MY_PRECIOUS_HAIR_4YKK01     = 213,  // [Yukako] Use the "My precious hair! It's turned completely white!" style
            WIN_DIRECT_CONTACT_5PRS01       = 215,  // [Prosciutto] Win with the "Direct contact is fastest" skill
            DEFLECT_GENTLY_WHEEPS_5GAC01    = 216,  // [Ghiaccio] Deflect a projectile with the "White Album Gently Weeps!" HHA
            USE_OVERCOME_WEAKNESS_6FFS01    = 217,  // [F.F.] Use the "I've overcome my weakness" style
            ARENA_GIMMICK                   = 218,  // Set off an arena gimmick
            DRAMATIC_FINISH                 = 219,  // Win with a Dramatic Finish
            LAND_HERMIT_PURPLE_3JSP01       = 220,  // [Joseph3] Land the "Hermit Purple!" skill
            TOUCH_3MRA01_SKILL              = 221,  // Make contact with the "If you're not allowed to touch something, it makes you want to touch it even more..." skill
            LAND_BREAK_INTO_PIECES_4JTR01   = 222,  // [Jotaro4] Land the "I'm going to break you into pieces" skill
            USE_SPTW_4JTR01                 = 223,  // [Jotaro4] Use the "Star Platinum, The World!" style 
            WIN_BREAK_YOUR_FACE_4JTR01      = 224,  // [Jotaro4] Win with the "I'm gonna break it... And by 'it', I mean your face." HHA
            LAND_FWOOSH_4KOI01              = 225,  // [Koichi] Land the grab "Fwoosh."
            USE_QUICK_RELOAD_5MST01         = 226,  // [Mista] Use the "Quick Reload" skill
            WIN_TRUE_RESOLVE_5MST01         = 227,  // [Mista] Win with the "My true resolve starts here!" GHA
            USE_TURNED_IT_SOFT_5TRS01       = 258,  // [Trish] Use the "I've turned it soft!" skill
            USE_STICK_IT_6EMS01             = 259,  // [Ermes] Use the "Here's where ya stick it!" skill
            LAND_CALL_THEM_TUSKS_7JNY01     = 260,  // [Johnny] Land the "I'll call them Tusks!" skill
            LAND_HA_HA_SUCKER_3IGY01        = 261,  // [Iggy] Land the "Ha ha, sucker!" skill
            USE_SAND_DOME_3IGY01            = 262,  // [Iggy] Use the "Sand Dome" special
            WIN_SERVES_YOU_RIGHT_3IGY01     = 263,  // [Iggy] Win with the "Serves you right!" GHA
            LAND_RIPPLE_FOCUS_2LSA01        = 264,  // [Lisa Lisa] Land the "Ripple Focus!" throw
            LAND_GO_NUTS_1SDW01             = 265,  // [Speedwagon] Land the "Go nuts with those knives, boys!" skill
            LAND_TO_THE_BONE_1SDW01         = 266,  // [Speedwagon] Land the "Ha ha! The blade went right to the bone!" GHA
            WIN_HERMIT_HAMON_3JSP01         = 267,  // [Joseph3] Win with the "Hermit Purple and Ripple!" GHA
            LAND_BLOW_YOU_AWAY_2JSP01       = 269,  // [Joseph2] Land the "This'll blow you away!" HHA
            USE_ULTIMATE_THING_2KRS01       = 270,  // [Kars] Use the "Kars, the Ultimate Thing, is born!" GHA
            WIN_DROOLING_OVER_3MRA01        = 272,  // [Mariah] Win with the "What are you drooling over?" HHA
            LAND_CAW_CAW_3PSP01             = 273,  // [Pet Shop] Land the "Caw caw!" skill
            USE_SKRAAAAAW_3PSP01            = 274,  // [Pet Shop] Use the "SKRAAAAAW" GHA
            USE_FLIGHT_MODE_3PSP01          = 275,  // [Pet Shop] Use the "Flight Mode" style
            WIN_ROAD_ROLLER_3DIO01          = 276,  // [DIO3] Win with the "It's a road roller!" GHA 
            LAND_IN_MY_GRASP_3KKI01         = 277,  // [Kakyoin] Land the "You're already in my grasp!" skill
            LAND_LORD_DIO_3VNI01            = 278,  // [Ice] Land the "Lord DIO!" skill
            USE_ANYTHING_BOMB_4KRA01        = 279,  // [Kira] Use the "Anything can be a bomb" skill
            LAND_ASK_YOUR_NAME_4KSK01       = 280,  // [Kosaku] Land the "Can I ask your name?" throw
            WIN_YOUR_REALITY_MINE_4RHN01    = 281,  // [Rohan] Win with the "Your reality will be mine!" HHA
            LAND_ARE_YOU_SERIOUS_5GCH01     = 282,  // [Ghiaccio] Land the "What the hell! What the hell! Are you serious?" throw
            USE_HAZE_DISTORTION_5FGO01      = 283,  // [Fugo] Use the "Purple Haze Distortion" special
            LAND_TIME_RESUMES_5DVL01        = 284,  // [Diavolo] Land the "The passage of time resumes!" skill
            USE_STROKE_OF_FATE_5BCT01       = 285,  // [Bucciarati] Use the "A tiny stroke of fate that heaven allowed" skill
            WIN_ALREADY_DONE_5PRS01         = 286,  // [Prosciutto] Win with the "Once we decide that we're gonna kill someone, the deed's already been done!" GHA
            LAND_PEEL_IT_OFF_6EMS01         = 287,  // [Ermes] Land the "Peel it off and it merges back into one!" skill
            WIN_1000_THROWS_6JLN01          = 288,  // [Jolyne] Win with the "1,000 throws!" GHA
            LAND_I_ORDER_YOU_6PCI02         = 289,  // [Pucci] Land the "I order you!" skill
            LAND_USHAAAAAAAA_7DIO01         = 290,  // [Diego] Land the "USHAAAAAAAA!" skill
            LAND_USELESS_USELESS_7DIO01     = 291,  // [Diego] Land the "Useless, useless!" skill
            WIN_MORE_SAPIENT_6FFS01         = 292,  // [F.F.] Win with the "I'm certainly more sapient than you!" GHA
            FLASH_CANCEL                    = 293,  // Execute a Flash Cancel
            USE_MAKE_IT_RAIN_6FFS01         = 294,  // [F.F.] Use the "Get Weather Report to make it rain." HHA
            LAND_FOLLOW_UP_SLIP_6WET01      = 295,  // [Weather] Land a follow-up attack from the "Don't slip up, or you're done..." skill
            USE_HEAVY_WEATHER_6WET01        = 296,  // [Weather] Use the "The Devil's Rainbow! It's Heavy Weather!" HHA
            USE_WEATHER_REPORT_6WET01       = 297,  // [Weather] Use the "Weather Report" style
            LAND_TORRIENTAL_6WET01          = 298,  // [Weather] Land the "Torriental Downpour Warning" skill
            LAND_SUSPENSION_6ANS01          = 299,  // [Anasui] Land the "Meat and Bone Suspension" skill
            LAND_280_WIND_SPEED_6WET01      = 300,  // [Weather] Land the "Wind speed of 280 km/h!" skill
            WIN_CLOUD_SUIT_6WET01           = 301   // [Weather] Win with the "Cloud Suit" GHA
        };

        std::uint32_t index;
        std::uint64_t part;
        std::string panel_id; // key
        std::uint64_t page; // auto-calculated
        std::string boss_panel; // auto-calculated

        struct {
            std::string up{""};
            std::string down{""};
            std::string left{""};
            std::string right{""};
        } adjacent_panels;

        Panel_Type type{Panel_Type::NORMAL};
        std::uint32_t display_difficulty;
        std::uint32_t cpu_level;
        std::uint32_t gold_reward;
        std::string stage_id;

        struct Character{
            std::string id;
            std::string assist_id{""};
            std::string btlst_id;
            std::string btlwin_id;
        };
        Character player, enemy;
        std::uint64_t first_speaker;

        Special_Rule special_rule[4];

        struct Secret_Mission {
            Mission_Condition condition{Mission_Condition::BLANK};
            std::int32_t unk{0}; // need to figure this one out
            std::string reward_id{""};
            std::uint64_t gold_reward{0};
        };
        Secret_Mission secret_mission[4]; // 4th doesn't work in ASBR

        std::uint32_t unk0{2};
        std::uint32_t unk1{0};
        std::uint32_t unk2{2};

        std::string key() {
            return panel_id;
        }
    };

    std::uint32_t version;
    std::uint32_t entry_count;
    std::uint64_t first_pointer;
    std::map<std::string, Entry> entries;

    MainModeParam(void* input, size_t size_input = -1) {
        load(input, size_input);
    }

    int load(void* input, size_t size_input = -1) {
        if (input == nullptr) return error_handler({
            nucc::Status_Code::POINTER_NULL,
            "Attempted to load MainModeParam chunk data, but received null input.",
            "Ensure the provided data is being passed correctly."
        });
        storage.load(input, 0, size_input);

        version = storage.read<std::uint32_t>(kojo::endian::little);
        entry_count = storage.read<std::uint32_t>(kojo::endian::little);
        first_pointer = storage.read<std::uint64_t>(kojo::endian::little);
        storage.change_pos(first_pointer - 8);

        Entry entry_buffer;
        for (int i = 0; i < entry_count; i++) {
            entry_buffer.part                   = storage.read<std::uint64_t>(kojo::endian::little);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.panel_id               = storage.read<std::string>(0, ptr_buffer64 - 8);
            entry_buffer.page                   = storage.read<std::uint64_t>(kojo::endian::little);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.boss_panel             = storage.read<std::string>(0, ptr_buffer64 - 8);
            storage.change_pos(16); // 2 unk
            entry_buffer.gold_reward            = storage.read<std::uint32_t>(kojo::endian::little);
            entry_buffer.type                   = (Entry::Panel_Type)storage.read<std::uint32_t>(kojo::endian::little);

            // Adjacent panels
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.adjacent_panels.up     = storage.read<std::string>(0, ptr_buffer64 - 8);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.adjacent_panels.down   = storage.read<std::string>(0, ptr_buffer64 - 8);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.adjacent_panels.left   = storage.read<std::string>(0, ptr_buffer64 - 8);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.adjacent_panels.right  = storage.read<std::string>(0, ptr_buffer64 - 8);

            entry_buffer.display_difficulty     = storage.read<std::uint32_t>(kojo::endian::little);
            entry_buffer.cpu_level              = storage.read<std::uint32_t>(kojo::endian::little);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.stage_id               = storage.read<std::string>(0, ptr_buffer64 - 8);
            storage.change_pos(8); // unk
            entry_buffer.first_speaker          = storage.read<std::uint64_t>(kojo::endian::little);

            // Player
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.player.id              = storage.read<std::string>(0, ptr_buffer64 - 8);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.player.assist_id       = storage.read<std::string>(0, ptr_buffer64 - 8);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.player.btlst_id        = storage.read<std::string>(0, ptr_buffer64 - 8);
            storage.change_pos(8); // unk
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.player.btlwin_id       = storage.read<std::string>(0, ptr_buffer64 - 8);

            // Enemy
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.enemy.id               = storage.read<std::string>(0, ptr_buffer64 - 8);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.enemy.assist_id        = storage.read<std::string>(0, ptr_buffer64 - 8);
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.enemy.btlst_id         = storage.read<std::string>(0, ptr_buffer64 - 8);
            storage.change_pos(8); // unk
            ptr_buffer64                        = storage.read<std::uint64_t>(kojo::endian::little);
            entry_buffer.enemy.btlwin_id        = storage.read<std::string>(0, ptr_buffer64 - 8);

            // Special rules
            entry_buffer.special_rule[0]    = (Entry::Special_Rule)storage.read<std::uint32_t>(kojo::endian::little);
            entry_buffer.special_rule[1]    = (Entry::Special_Rule)storage.read<std::uint32_t>(kojo::endian::little);
            entry_buffer.special_rule[2]    = (Entry::Special_Rule)storage.read<std::uint32_t>(kojo::endian::little);
            entry_buffer.special_rule[3]    = (Entry::Special_Rule)storage.read<std::uint32_t>(kojo::endian::little);

            // Secret missions
            for (int i = 0; i < 4; i++) {
                entry_buffer.secret_mission[i].condition    = (Entry::Mission_Condition)storage.read<std::uint32_t>(kojo::endian::little);
                entry_buffer.secret_mission[i].unk          = storage.read<std::int32_t>(kojo::endian::little);
                ptr_buffer64                                = storage.read<std::uint64_t>(kojo::endian::little);
                entry_buffer.secret_mission[i].reward_id    = storage.read<std::string>(0, ptr_buffer64 - 8);
                entry_buffer.secret_mission[i].gold_reward  = storage.read<std::uint64_t>(kojo::endian::little);
            }

            entry_buffer.unk0                                   = storage.read<std::uint32_t>(kojo::endian::little);
            entry_buffer.unk1                                   = storage.read<std::uint32_t>(kojo::endian::little);
            entry_buffer.unk2                                   = storage.read<std::uint32_t>(kojo::endian::little);

            entry_buffer.index                                  = storage.read<std::uint32_t>(kojo::endian::little);

            entries[entry_buffer.key()] = entry_buffer;
        }
    
        return 0;
    }
    int load(nlohmann::ordered_json input) {
        if (input.is_null()) return 0;

        str_to_Panel_Type["NORMAL"]  = Entry::Panel_Type::NORMAL;
        str_to_Panel_Type["EXTRA"]   = Entry::Panel_Type::EXTRA;
        str_to_Panel_Type["BOSS"]    = Entry::Panel_Type::BOSS;
        load_stage_reflist();
        load_special_rules_reflist();
        load_mission_conditions_reflist();

        for (const auto& [key, value] : input.items()) {
            Entry entry_buffer;

            entry_buffer.panel_id = key;
            if (value.contains("Part")) entry_buffer.part = value["Part"];
                else return error_handler({
                    nucc::Status_Code::JSON_MISSING_FIELD,
                    std::format("JSON data for entry \"{}\" does not contain necessary field \"Part\".", key),
                    "Add the \"Part\" field depending on which Part the panel belongs to."
                });
            if (value.contains("Boss Panel")) entry_buffer.boss_panel = value["Boss Panel"];
                else entry_buffer.boss_panel = entry_buffer.panel_id.substr(0, 9) + "08";
            if (value.contains("Page")) entry_buffer.page = value["Page"];
                else entry_buffer.page = std::stoi(entry_buffer.panel_id.substr(6, 2));
            if (value.contains("Index")) entry_buffer.index = value["Index"];
                else entry_buffer.index = entry_buffer.page * std::stoi(entry_buffer.panel_id.substr(9, 2));

            // Adjacent Panels
            if (value.contains("Adjacent Panels")) {
                auto& adjacent_panels = value["Adjacent Panels"];
                if (adjacent_panels.contains("Up"))
                    entry_buffer.adjacent_panels.up     = adjacent_panels["Up"];
                if (adjacent_panels.contains("Down"))
                    entry_buffer.adjacent_panels.down   = adjacent_panels["Down"];
                if (adjacent_panels.contains("Left"))
                    entry_buffer.adjacent_panels.left   = adjacent_panels["Left"];
                if (adjacent_panels.contains("Right"))
                    entry_buffer.adjacent_panels.right  = adjacent_panels["Right"];
            } else {
                return error_handler({
                    nucc::Status_Code::JSON_MISSING_FIELD,
                    std::format("JSON data for entry \"{}\" does not contain necessary field \"Adjacent Panels\".", key),
                    "Add the \"Adjacent Panels\" JSON object, even if its contents are empty."
                });
            }

            if (value.contains("Type")) {
                if (str_to_Panel_Type.contains(value["Type"]))
                    entry_buffer.type = str_to_Panel_Type[value["Type"]];
            }
            if (value.contains("Stars")) entry_buffer.display_difficulty = value["Stars"];
                else return error_handler({
                    nucc::Status_Code::JSON_MISSING_FIELD,
                    std::format("JSON data for entry \"{}\" does not contain necessary field \"Stars\".", key),
                    "Add the \"Stars\" field with a value from 1-5."
                });
            if (value.contains("CPU Level")) entry_buffer.cpu_level = value["CPU Level"];
                else entry_buffer.cpu_level = entry_buffer.display_difficulty;
            if (value.contains("Gold Reward")) entry_buffer.gold_reward = value["Gold Reward"];
                else return error_handler({
                    nucc::Status_Code::JSON_MISSING_FIELD,
                    std::format("JSON data for entry \"{}\" does not contain necessary field \"Gold Reward\".", key),
                    "Add the \"Gold Reward\" field."
                });
            if (value.contains("Stage")) entry_buffer.stage_id = get_stage_ref(value["Stage"]);
                else return error_handler({
                    nucc::Status_Code::JSON_MISSING_FIELD,
                    std::format("JSON data for entry \"{}\" does not contain necessary field \"Stage\".", key),
                    "Add the \"Stage\" field."
                });

            // Player Information
            if (value.contains("Player Information")) {
                auto& player_information = value["Player Information"];
                if (player_information.contains("Character"))
                    entry_buffer.player.id = character_id_reflist[player_information["Character"]];
                    else return error_handler({
                        nucc::Status_Code::JSON_MISSING_FIELD,
                        std::format("JSON data for entry \"{}\" does not contain necessary field \"Character\" within object \"Player Information\".", key),
                        "Add the \"Character\" field with the name or ID of a playable character."
                    });
                if (player_information.contains("Assist"))
                    entry_buffer.player.assist_id = character_id_reflist[player_information["Assist"]];
                if (player_information.contains("Start Dialogue"))
                    entry_buffer.player.btlst_id = player_information["Start Dialogue"];
                    else return error_handler({
                        nucc::Status_Code::JSON_MISSING_FIELD,
                        std::format("JSON data for entry \"{}\" does not contain necessary field \"Start Dialogue\" within object \"Player Information\".", key),
                        "Add the \"Start Dialogue\" field."
                    });
                if (player_information.contains("Win Dialogue"))
                    entry_buffer.player.btlwin_id = player_information["Win Dialogue"];
                    else return error_handler({
                        nucc::Status_Code::JSON_MISSING_FIELD,
                        std::format("JSON data for entry \"{}\" does not contain necessary field \"Win Dialogue\" within object \"Player Information\".", key),
                        "Add the \"Win Dialogue\" field."
                    });
            } else {
                return error_handler({
                    nucc::Status_Code::JSON_MISSING_FIELD,
                    std::format("JSON data for entry \"{}\" does not contain necessary field \"Player Information\".", key),
                    "Add the \"Player Information\" object with its contents included as well."
                });
            }

            // Enemy Information
            if (value.contains("Enemy Information")) {
                auto& enemy_information = value["Enemy Information"];
                if (enemy_information.contains("Character"))
                    entry_buffer.enemy.id = character_id_reflist[enemy_information["Character"]];
                    else return error_handler({
                        nucc::Status_Code::JSON_MISSING_FIELD,
                        std::format("JSON data for entry \"{}\" does not contain necessary field \"Character\" within object \"Enemy Information\".", key),
                        "Add the \"Character\" field with the name or ID of a playable character."
                    });
                if (enemy_information.contains("Assist"))
                    entry_buffer.enemy.assist_id = character_id_reflist[enemy_information["Assist"]];
                if (enemy_information.contains("Start Dialogue"))
                    entry_buffer.enemy.btlst_id = enemy_information["Start Dialogue"];
                    else return error_handler({
                        nucc::Status_Code::JSON_MISSING_FIELD,
                        std::format("JSON data for entry \"{}\" does not contain necessary field \"Start Dialogue\" within object \"Enemy Information\".", key),
                        "Add the \"Start Dialogue\" field."
                    });
                if (enemy_information.contains("Win Dialogue"))
                    entry_buffer.enemy.btlwin_id = enemy_information["Win Dialogue"];
                    else return error_handler({
                        nucc::Status_Code::JSON_MISSING_FIELD,
                        std::format("JSON data for entry \"{}\" does not contain necessary field \"Win Dialogue\" within object \"Enemy Information\".", key),
                        "Add the \"Win Dialogue\" field."
                    });
            } else {
                return error_handler({
                    nucc::Status_Code::JSON_MISSING_FIELD,
                    std::format("JSON data for entry \"{}\" does not contain necessary field \"Enemy Information\".", key),
                    "Add the \"Enemy Information\" object with its contents included as well."
                });
            }

            if (value.contains("First To Speak")) {
                if (value["First To Speak"] == "Enemy") entry_buffer.first_speaker = 1;
                    else entry_buffer.first_speaker = 0;
            } else {
                return error_handler({
                    nucc::Status_Code::JSON_MISSING_FIELD,
                    std::format("JSON data for entry \"{}\" does not contain necessary field \"First To Speak\".", key),
                    "Add the \"First To Speak\" field."
                });
            }

            // Special Rules
            if (value.contains("Special Rules")) {
                for (int i = 1; i <= 4; i++) {
                    if (value["Special Rules"].contains(std::format("Rule {}", i)))
                        entry_buffer.special_rule[i] = get_special_rule_ref(value["Special Rules"][std::format("Rule {}", i)]);
                        else entry_buffer.special_rule[i] = Entry::Special_Rule::BLANK;
                }
            } else {
                entry_buffer.special_rule[0] = Entry::Special_Rule::NONE;
                entry_buffer.special_rule[1] = Entry::Special_Rule::NONE;
                entry_buffer.special_rule[2] = Entry::Special_Rule::BLANK;
                entry_buffer.special_rule[3] = Entry::Special_Rule::BLANK;
            }

            // Secret Missions
            if (value.contains("Secret Missions")) {
                for (int i = 1; i <= 4; i++) {
                    std::string mission_title = std::format("Mission {}", i);
                    if (value["Secret Missions"].contains(mission_title)) {
                        auto& mission = value["Secret Missions"][mission_title];
                        if (mission.contains("Condition"))
                            entry_buffer.secret_mission[i - 1].condition = get_mission_condition_ref(mission["Condition"]);
                            else return error_handler({
                                nucc::Status_Code::JSON_MISSING_FIELD,
                                std::format("JSON data for entry \"{}\" does not contain necessary field \"Condition\" within object \"{}\".", key, mission_title),
                                "Add the \"Condition\" field."
                            });
                        if (mission.contains("Reward"))
                            entry_buffer.secret_mission[i - 1].reward_id = mission["Reward"];
                        if (mission.contains("Gold Reward"))
                            entry_buffer.secret_mission[i - 1].gold_reward = mission["Gold Reward"];
                            else return error_handler({
                                nucc::Status_Code::JSON_MISSING_FIELD,
                                std::format("JSON data for entry \"{}\" does not contain necessary field \"Gold Reward\" within object \"{}\".", key, mission_title),
                                "Add the \"Gold Reward\" field."
                            });
                    }
                }
            } else {
                return error_handler({
                    nucc::Status_Code::JSON_MISSING_FIELD,
                    std::format("JSON data for entry \"{}\" does not contain necessary field \"Secret Missions\".", key),
                    "Add the \"Secret Missions\" object with its contents included as well."
                });
            }

            entries[entry_buffer.key()] = entry_buffer;
            return 0;
        }
    
        return 0;
    }

    size_t size() {
        size_t size_buffer = entries.size() * 328 + 8 + first_pointer;
        size_t size_buffer2;

        for (auto& [key, entry] : entries) {
            if ((size_buffer2 = entry.panel_id.size()) > 0)                       size_buffer += ceiling(size_buffer2 + 1, 8);
            if ((size_buffer2 = entry.boss_panel.size()) > 0)                     size_buffer += ceiling(size_buffer2 + 1, 8);
            if ((size_buffer2 = entry.adjacent_panels.up.size()) > 0)             size_buffer += ceiling(size_buffer2 + 1, 8);
            if ((size_buffer2 = entry.adjacent_panels.down.size()) > 0)           size_buffer += ceiling(size_buffer2 + 1, 8);
            if ((size_buffer2 = entry.adjacent_panels.left.size()) > 0)           size_buffer += ceiling(size_buffer2 + 1, 8);
            if ((size_buffer2 = entry.adjacent_panels.right.size()) > 0)          size_buffer += ceiling(size_buffer2 + 1, 8);
            if ((size_buffer2 = entry.stage_id.size()) > 0)                       size_buffer += ceiling(size_buffer2 + 1, 8);
            if ((size_buffer2 = entry.player.id.size()) > 0)                      size_buffer += ceiling(size_buffer2 + 1, 8);
            if ((size_buffer2 = entry.player.assist_id.size()) > 0)               size_buffer += ceiling(size_buffer2 + 1, 8);
            if ((size_buffer2 = entry.player.btlst_id.size()) > 0)                size_buffer += ceiling(size_buffer2 + 1, 8);
            if ((size_buffer2 = entry.player.btlwin_id.size()) > 0)               size_buffer += ceiling(size_buffer2 + 1, 8);
            if ((size_buffer2 = entry.enemy.id.size()) > 0)                       size_buffer += ceiling(size_buffer2 + 1, 8);
            if ((size_buffer2 = entry.enemy.assist_id.size()) > 0)                size_buffer += ceiling(size_buffer2 + 1, 8);
            if ((size_buffer2 = entry.enemy.btlst_id.size()) > 0)                 size_buffer += ceiling(size_buffer2 + 1, 8);
            if ((size_buffer2 = entry.enemy.btlwin_id.size()) > 0)                size_buffer += ceiling(size_buffer2 + 1, 8);
            if ((size_buffer2 = entry.secret_mission[0].reward_id.size()) > 0)    size_buffer += ceiling(size_buffer2 + 1, 8);
            if ((size_buffer2 = entry.secret_mission[1].reward_id.size()) > 0)    size_buffer += ceiling(size_buffer2 + 1, 8);
            if ((size_buffer2 = entry.secret_mission[2].reward_id.size()) > 0)    size_buffer += ceiling(size_buffer2 + 1, 8);
            if ((size_buffer2 = entry.secret_mission[3].reward_id.size()) > 0)    size_buffer += ceiling(size_buffer2 + 1, 8);
        }

        return size_buffer;
    }
    void clear() {
        version = 1000;
        entry_count = 0;
        first_pointer = 8;
        entries.clear();
    }

    std::uint64_t* write_to_bin() {
        storage.clear();
        
        version = 1000;
        entry_count = entries.size();
        storage.write<std::uint32_t>(version, kojo::endian::little);
        storage.write<std::uint32_t>(entry_count, kojo::endian::little);
        storage.write<std::uint64_t>(first_pointer, kojo::endian::little);

        str_tracker.clear();
        last_pos = 8 + first_pointer; // Size of header
        ptr_buffer64 = 328 * entry_count;
        for (auto& [key, entry] : entries) {
            storage.write<std::uint64_t>(entry.part, kojo::endian::little);
            write_offset_str(entry.panel_id);
            storage.write<std::uint64_t>(entry.page, kojo::endian::little);
            write_offset_str(entry.boss_panel);
            storage.write<std::uint64_t>(0, kojo::endian::little); // unk
            storage.write<std::uint64_t>(0, kojo::endian::little); // unk
            storage.write<std::uint32_t>(entry.gold_reward, kojo::endian::little);
            storage.write<std::uint32_t>((int)entry.type, kojo::endian::little);

            write_offset_str(entry.adjacent_panels.up);
            write_offset_str(entry.adjacent_panels.down);
            write_offset_str(entry.adjacent_panels.left);
            write_offset_str(entry.adjacent_panels.right);

            storage.write<std::uint32_t>(entry.display_difficulty, kojo::endian::little);
            storage.write<std::uint32_t>(entry.cpu_level, kojo::endian::little);
            write_offset_str(entry.stage_id);
            storage.write<std::uint64_t>(1, kojo::endian::little); // unk
            storage.write<std::uint64_t>(entry.first_speaker, kojo::endian::little);

            write_offset_str(entry.player.id);
            write_offset_str(entry.player.assist_id);
            write_offset_str(entry.player.btlst_id);
            storage.write<std::uint64_t>(0, kojo::endian::little); // unk
            write_offset_str(entry.player.btlwin_id);
            write_offset_str(entry.enemy.id);
            write_offset_str(entry.enemy.assist_id);
            write_offset_str(entry.enemy.btlst_id);
            storage.write<std::uint64_t>(0, kojo::endian::little); // unk
            write_offset_str(entry.enemy.btlwin_id);

            for (int i = 1; i <= 4; i++) {
              storage.write<std::uint32_t>((int)entry.special_rule[i], kojo::endian::little);
            }

            for (int i = 0; i < 4; i++) {
                storage.write<std::uint32_t>((int)entry.secret_mission[i].condition, kojo::endian::little);
                storage.write<std::int32_t>(entry.secret_mission[i].unk, kojo::endian::little);
                write_offset_str(entry.secret_mission[i].reward_id);
                storage.write<std::uint64_t>(entry.secret_mission[i].gold_reward, kojo::endian::little);
            }

            storage.write<std::uint32_t>(entry.unk0, kojo::endian::little);
            storage.write<std::uint32_t>(entry.unk1, kojo::endian::little);
            storage.write<std::uint32_t>(entry.unk2, kojo::endian::little);

            storage.write<std::uint32_t>(entry.index, kojo::endian::little);
        }
        for (auto& str : str_tracker) {
            storage.write<std::string>(str);
            storage.align_by(8);
        }

        return (std::uint64_t*)storage.data();
    }
    std::string write_to_json() {
        nlohmann::ordered_json json;
        json["Type"] = "MainModeParam";
        json["Version"] = 1000;
        
        for (auto& [key, value] : entries) {
            auto& entry = json[value.panel_id];

            entry["Index"] = value.index;
            entry["Part"] = value.part;
            entry["Adjacent Panels"]["Up"] = value.adjacent_panels.up;
            entry["Adjacent Panels"]["Down"] = value.adjacent_panels.down;
            entry["Adjacent Panels"]["Left"] = value.adjacent_panels.left;
            entry["Adjacent Panels"]["Right"] = value.adjacent_panels.right;

            switch (value.type) {
                case Entry::Panel_Type::NORMAL  : entry["Type"] = "NORMAL"; break;
                case Entry::Panel_Type::EXTRA   : entry["Type"] = "EXTRA"; break;
                case Entry::Panel_Type::BOSS    : entry["Type"] = "BOSS"; break;
            }

            entry["Difficulty Stars"] = value.display_difficulty;
            entry["CPU Level"] = value.cpu_level;
            entry["Gold Reward"] = value.gold_reward;
            entry["Stage"] = value.stage_id;
        }

        return json.dump(2);
    }

private:
    std::unordered_map<std::string, Entry::Panel_Type> str_to_Panel_Type;
    std::unordered_map<std::string, Entry::Special_Rule> str_to_Special_Rule;
    std::unordered_map<std::string, Entry::Mission_Condition> str_to_Mission_Condition;
    std::unordered_map<std::string, std::string> stage_reflist;

    void load_stage_reflist() {
        stage_reflist["dio's castle"]                   = "STAGE_1_DIOCASTLE";

        stage_reflist["the battlefield"]                = "STAGE_2_COLOSSEO";
        stage_reflist["battlefield"]                    = "STAGE_2_COLOSSEO";

        stage_reflist["dio's mansion"]                  = "STAGE_3_DIOCASTLE";
        stage_reflist["cairo streets"]                  = "STAGE_3_CAIRO";
        stage_reflist["cairo overpass"]                 = "STAGE_3_CAIROCITY_BRIDGE";
        stage_reflist["cairo bridge"]                   = "STAGE_3_CAIROCITY_BRIDGE";

        stage_reflist["morioh town"]                    = "STAGE_4_MOT";
        stage_reflist["boyoyoing cape"]                 = "STAGE_4_BOYOYONH_CAPE";
        stage_reflist["kira estate"]                    = "STAGE_4_KIH";
        stage_reflist["kira house"]                     = "STAGE_4_KIH";
        stage_reflist["kira residence"]                 = "STAGE_4_KIH";

        stage_reflist["naples station"]                 = "STAGE_5_NEAPOLIS";
        stage_reflist["naples train station"]           = "STAGE_5_NEAPOLIS";
        stage_reflist["rome streets"]                   = "STAGE_5_ROMA";
        stage_reflist["colosseum"]                      = "STAGE_5_COLOSSEUM";

        stage_reflist["green dolphin street prison"]    = "STAGE_6_GD";
        stage_reflist["green dolphin"]                  = "STAGE_6_GD";
        stage_reflist["everglades"]                     = "STAGE_6_WETLANDS";
        stage_reflist["wetlands"]                       = "STAGE_6_WETLANDS";
        stage_reflist["kennedy space center"]           = "STAGE_6_KSC";
        stage_reflist["kennedy space centre"]           = "STAGE_6_KSC";

        stage_reflist["rocky mountains village"]        = "STAGE_7_ROCKIES_VILLAGE";
        stage_reflist["philadelphia coastline"]         = "STAGE_7_PSS";
        stage_reflist["philadelphia seaside"]           = "STAGE_7_PSS";

        stage_reflist["wall eyes"]                      = "STAGE_8_KBM";
    }
    std::string get_stage_ref(std::string ref) {
        std::string copy{ref};
        for (auto& ch : copy)
            ch = std::tolower(ch);
        if (!stage_reflist.contains(copy)) return ref;
        return stage_reflist[copy];
    }

    void load_special_rules_reflist() {
        str_to_Special_Rule["BLANK"]                = Entry::Special_Rule::BLANK;
        str_to_Special_Rule["NONE"]                 = Entry::Special_Rule::NONE;
        str_to_Special_Rule["ONE_ROUND"]            = Entry::Special_Rule::ONE_ROUND;
        str_to_Special_Rule["THREE_ROUND"]          = Entry::Special_Rule::THREE_ROUND;
        str_to_Special_Rule["FIVE_ROUND"]           = Entry::Special_Rule::FIVE_ROUND;
        str_to_Special_Rule["COUNT_30"]             = Entry::Special_Rule::COUNT_30;
        str_to_Special_Rule["COUNT_60"]             = Entry::Special_Rule::COUNT_60;
        str_to_Special_Rule["COUNT_90"]             = Entry::Special_Rule::COUNT_90;
        str_to_Special_Rule["PL_HH_REFILL"]         = Entry::Special_Rule::PL_HH_REFILL;
        str_to_Special_Rule["OPP_HH_REFILL"]        = Entry::Special_Rule::OPP_HH_REFILL;
        str_to_Special_Rule["PL_HEALTH_REFILL"]     = Entry::Special_Rule::PL_HEALTH_REFILL;
        str_to_Special_Rule["OPP_HEALTH_REFILL"]    = Entry::Special_Rule::OPP_HEALTH_REFILL;
        str_to_Special_Rule["PL_HEALTH_DRAIN"]      = Entry::Special_Rule::PL_HEALTH_DRAIN;
        str_to_Special_Rule["OPP_HEALTH_DRAIN"]     = Entry::Special_Rule::OPP_HEALTH_DRAIN;
        str_to_Special_Rule["PL_NO_GUARD"]          = Entry::Special_Rule::PL_NO_GUARD;
        str_to_Special_Rule["OPP_NO_GUARD"]         = Entry::Special_Rule::OPP_NO_GUARD;
        str_to_Special_Rule["PL_ATK_DOWN"]          = Entry::Special_Rule::PL_ATK_DOWN;
        str_to_Special_Rule["OPP_ATK_DOWN"]         = Entry::Special_Rule::OPP_ATK_DOWN;
        str_to_Special_Rule["PL_ATK_UP"]            = Entry::Special_Rule::PL_ATK_UP;
        str_to_Special_Rule["OPP_ATK_UP"]           = Entry::Special_Rule::OPP_ATK_UP;
        str_to_Special_Rule["PL_DEF_DOWN"]          = Entry::Special_Rule::PL_DEF_DOWN;
        str_to_Special_Rule["OPP_DEF_DOWN"]         = Entry::Special_Rule::OPP_DEF_DOWN;
        str_to_Special_Rule["PL_DEF_UP"]            = Entry::Special_Rule::PL_DEF_UP;
        str_to_Special_Rule["OPP_DEF_UP"]           = Entry::Special_Rule::OPP_DEF_UP;
        str_to_Special_Rule["PL_HP_10_PERCENT"]     = Entry::Special_Rule::PL_HP_10_PERCENT;
        str_to_Special_Rule["OPP_HP_10_PERCENT"]    = Entry::Special_Rule::OPP_HP_10_PERCENT;
        str_to_Special_Rule["PL_HP_30_PERCENT"]     = Entry::Special_Rule::PL_HP_30_PERCENT;
        str_to_Special_Rule["OPP_HP_30_PERCENT"]    = Entry::Special_Rule::OPP_HP_30_PERCENT;
        str_to_Special_Rule["PL_HP_50_PERCENT"]     = Entry::Special_Rule::PL_HP_50_PERCENT;
        str_to_Special_Rule["OPP_HP_50_PERCENT"]    = Entry::Special_Rule::OPP_HP_50_PERCENT;
        str_to_Special_Rule["PL_HH_HAMON"]          = Entry::Special_Rule::PL_HH_HAMON;
        str_to_Special_Rule["OPP_HH_HAMON"]         = Entry::Special_Rule::OPP_HH_HAMON;
        str_to_Special_Rule["NO_GEIL_3HRS01"]       = Entry::Special_Rule::NO_GEIL_3HRS01;
        str_to_Special_Rule["ULTIMATE_2KRS01"]      = Entry::Special_Rule::ULTIMATE_2KRS01;
        str_to_Special_Rule["ENRAGED_4JSK01"]       = Entry::Special_Rule::ENRAGED_4JSK01;
        str_to_Special_Rule["REQUIEM_5GRN01"]       = Entry::Special_Rule::REQUIEM_5GRN01;
        str_to_Special_Rule["CMOON_6PCI01"]         = Entry::Special_Rule::CMOON_6PCI01;
        str_to_Special_Rule["MIH_6PCI01"]           = Entry::Special_Rule::MIH_6PCI01;
        str_to_Special_Rule["CORPSE_7VTN01"]        = Entry::Special_Rule::CORPSE_7VTN01;
        str_to_Special_Rule["ONLY_SM_7DIO01"]       = Entry::Special_Rule::ONLY_SM_7DIO01;
        str_to_Special_Rule["NO_HAMON"]             = Entry::Special_Rule::NO_HAMON;
        str_to_Special_Rule["MAX_MAGNET_3MRA01"]    = Entry::Special_Rule::MAX_MAGNET_3MRA01;
        str_to_Special_Rule["INF_FLIGHT_3PSP01"]    = Entry::Special_Rule::INF_FLIGHT_3PSP01;
        str_to_Special_Rule["LOVE_DELUXE_4YKK01"]   = Entry::Special_Rule::LOVE_DELUXE_4YKK01;
        str_to_Special_Rule["PERMANENT_AGE"]        = Entry::Special_Rule::PERMANENT_AGE;
        str_to_Special_Rule["NO_HH_STAND_ON"]       = Entry::Special_Rule::NO_HH_STAND_ON;
        str_to_Special_Rule["PL_ASS_ATK_DOWN"]      = Entry::Special_Rule::PL_ASS_ATK_DOWN;
        str_to_Special_Rule["PL_ASS_ATK_UP"]        = Entry::Special_Rule::PL_ASS_ATK_UP;
        str_to_Special_Rule["OP_ASS_ATK_DOWN"]      = Entry::Special_Rule::OP_ASS_ATK_DOWN;
        str_to_Special_Rule["OP_ASS_ATK_UP"]        = Entry::Special_Rule::OP_ASS_ATK_UP;
        str_to_Special_Rule["PL_HAMON_DMG_DOWN"]    = Entry::Special_Rule::PL_HAMON_DMG_DOWN;
        str_to_Special_Rule["PL_HAMON_DMG_UP"]      = Entry::Special_Rule::PL_HAMON_DMG_UP;
        str_to_Special_Rule["OPP_HAMON_DMG_DOWN"]   = Entry::Special_Rule::OPP_HAMON_DMG_DOWN;
        str_to_Special_Rule["OPP_HAMON_DMG_UP"]     = Entry::Special_Rule::OPP_HAMON_DMG_UP;
        str_to_Special_Rule["PL_VAMP_DMG_DOWN"]     = Entry::Special_Rule::PL_VAMP_DMG_DOWN;
        str_to_Special_Rule["PL_VAMP_DMG_UP"]       = Entry::Special_Rule::PL_VAMP_DMG_UP;
        str_to_Special_Rule["PL_VAMP_HEAL_DOWN"]    = Entry::Special_Rule::PL_VAMP_HEAL_DOWN;
        str_to_Special_Rule["PL_VAMP_HEAL_UP"]      = Entry::Special_Rule::PL_VAMP_HEAL_UP;
        str_to_Special_Rule["PL_FIRE_DMG_DOWN"]     = Entry::Special_Rule::PL_FIRE_DMG_DOWN;
        str_to_Special_Rule["PL_FIRE_DMG_UP"]       = Entry::Special_Rule::PL_FIRE_DMG_UP;
        str_to_Special_Rule["NO_SNAIL_PUCCI"]       = Entry::Special_Rule::NO_SNAIL_PUCCI;
        str_to_Special_Rule["OPP_VAMP_DMG_DOWN"]    = Entry::Special_Rule::OPP_VAMP_DMG_DOWN;
        str_to_Special_Rule["OPP_VAMP_DMG_UP"]      = Entry::Special_Rule::OPP_VAMP_DMG_UP;
        str_to_Special_Rule["OPP_VAMP_HEAL_DOWN"]   = Entry::Special_Rule::OPP_VAMP_HEAL_DOWN;
        str_to_Special_Rule["OPP_VAMP_HEAL_UP"]     = Entry::Special_Rule::OPP_VAMP_HEAL_UP;
        str_to_Special_Rule["OPP_FIRE_DMG_DOWN"]    = Entry::Special_Rule::OPP_FIRE_DMG_DOWN;
        str_to_Special_Rule["OPP_FIRE_DMG_UP"]      = Entry::Special_Rule::OPP_FIRE_DMG_UP;
    }
    Entry::Special_Rule get_special_rule_ref(std::string ref) {
        if (!str_to_Special_Rule.contains(ref)) return Entry::Special_Rule::BLANK;
        return str_to_Special_Rule[ref];
    }

    void load_mission_conditions_reflist() {
        str_to_Mission_Condition["BLANK"]                            = Entry::Mission_Condition::BLANK;
        str_to_Mission_Condition["NO_LOSSES"]                        = Entry::Mission_Condition::NO_LOSSES;
        str_to_Mission_Condition["FIRST_ATTACK"]                     = Entry::Mission_Condition::FIRST_ATTACK;
        str_to_Mission_Condition["COUNTERATTACK"]                    = Entry::Mission_Condition::COUNTERATTACK;
        str_to_Mission_Condition["HIT_COMBO_5"]                      = Entry::Mission_Condition::HIT_COMBO_5;
        str_to_Mission_Condition["HIT_COMBO_10"]                     = Entry::Mission_Condition::HIT_COMBO_10;
        str_to_Mission_Condition["HIT_COMBO_20"]                     = Entry::Mission_Condition::HIT_COMBO_20;
        str_to_Mission_Condition["HIT_COMBO_30"]                     = Entry::Mission_Condition::HIT_COMBO_30;
        str_to_Mission_Condition["HIT_COMBO_40"]                     = Entry::Mission_Condition::HIT_COMBO_40;
        str_to_Mission_Condition["HIT_COMBO_50"]                     = Entry::Mission_Condition::HIT_COMBO_50;
        str_to_Mission_Condition["HIT_COMBO_100"]                    = Entry::Mission_Condition::HIT_COMBO_100;
        str_to_Mission_Condition["HIT_COMBO_200"]                    = Entry::Mission_Condition::HIT_COMBO_200;
        str_to_Mission_Condition["HIT_COMBO_300"]                    = Entry::Mission_Condition::HIT_COMBO_300;
        str_to_Mission_Condition["STYLISH_EVADE"]                    = Entry::Mission_Condition::STYLISH_EVADE;
        str_to_Mission_Condition["TAUNT"]                            = Entry::Mission_Condition::TAUNT;
        str_to_Mission_Condition["STAND_ON"]                         = Entry::Mission_Condition::STAND_ON;
        str_to_Mission_Condition["DOWN_OPPONENT"]                    = Entry::Mission_Condition::DOWN_OPPONENT;
        str_to_Mission_Condition["WIN_TIME_UP"]                      = Entry::Mission_Condition::WIN_TIME_UP;
        str_to_Mission_Condition["LAUNCH_RUSH_MODE"]                 = Entry::Mission_Condition::LAUNCH_RUSH_MODE;
        str_to_Mission_Condition["WIN_RUSH_MODE"]                    = Entry::Mission_Condition::WIN_RUSH_MODE;
        str_to_Mission_Condition["USE_HHA"]                          = Entry::Mission_Condition::USE_HHA;
        str_to_Mission_Condition["USE_GHA"]                          = Entry::Mission_Condition::USE_GHA;
        str_to_Mission_Condition["LAND_GHA"]                         = Entry::Mission_Condition::LAND_GHA;
        str_to_Mission_Condition["LAND_HHA"]                         = Entry::Mission_Condition::LAND_HHA;
        str_to_Mission_Condition["WIN_HHA"]                          = Entry::Mission_Condition::WIN_HHA;
        str_to_Mission_Condition["WIN_GHA"]                          = Entry::Mission_Condition::WIN_GHA;
        str_to_Mission_Condition["ATK_OPP_STAND"]                    = Entry::Mission_Condition::ATK_OPP_STAND;
        str_to_Mission_Condition["ATK_STAND_ON"]                     = Entry::Mission_Condition::ATK_STAND_ON;
        str_to_Mission_Condition["POW_1ZPL01"]                       = Entry::Mission_Condition::POW_1ZPL01;
        str_to_Mission_Condition["ZOOM_PUNCH"]                       = Entry::Mission_Condition::ZOOM_PUNCH;
        str_to_Mission_Condition["HAMON_BREATH"]                     = Entry::Mission_Condition::HAMON_BREATH;
        str_to_Mission_Condition["USE_SEAT_JUMP_1ZPL01"]             = Entry::Mission_Condition::USE_SEAT_JUMP_1ZPL01;
        str_to_Mission_Condition["LAND_IMPUDENT_FOOL_1DIO01"]        = Entry::Mission_Condition::LAND_IMPUDENT_FOOL_1DIO01;
        str_to_Mission_Condition["LAND_FROZEN_TRACKS_1DIO01"]        = Entry::Mission_Condition::LAND_FROZEN_TRACKS_1DIO01;
        str_to_Mission_Condition["LAND_LUCK_PLUCK_1JNT01"]           = Entry::Mission_Condition::LAND_LUCK_PLUCK_1JNT01;
        str_to_Mission_Condition["USE_MERE_DOG_1DIO01"]              = Entry::Mission_Condition::USE_MERE_DOG_1DIO01;
        str_to_Mission_Condition["LAND_VAPOR_FREEZE_1DIO01"]         = Entry::Mission_Condition::LAND_VAPOR_FREEZE_1DIO01;
        str_to_Mission_Condition["LAND_HAMON_SKILL"]                 = Entry::Mission_Condition::LAND_HAMON_SKILL;
        str_to_Mission_Condition["LAND_NEXT_LINE_2JSP01"]            = Entry::Mission_Condition::LAND_NEXT_LINE_2JSP01;
        str_to_Mission_Condition["LAND_CLACKER_BOOMERANG_2JSP01"]    = Entry::Mission_Condition::LAND_CLACKER_BOOMERANG_2JSP01;
        str_to_Mission_Condition["LAND_DONT_GET_IT_2JSP01"]          = Entry::Mission_Condition::LAND_DONT_GET_IT_2JSP01;
        str_to_Mission_Condition["LAND_SMASH_YOU_GOOD_2JSP01"]       = Entry::Mission_Condition::LAND_SMASH_YOU_GOOD_2JSP01;
        str_to_Mission_Condition["USE_HAMON_GUARD_2JSP01"]           = Entry::Mission_Condition::USE_HAMON_GUARD_2JSP01;
        str_to_Mission_Condition["USE_HEAT_MODE_2ESD01"]             = Entry::Mission_Condition::USE_HEAT_MODE_2ESD01;
        str_to_Mission_Condition["LAND_HOW_COULD_YOU_2ESD01"]        = Entry::Mission_Condition::LAND_HOW_COULD_YOU_2ESD01;
        str_to_Mission_Condition["LAND_BURNING_KING_2ESD01"]         = Entry::Mission_Condition::LAND_BURNING_KING_2ESD01;
        str_to_Mission_Condition["LAND_BUBBLE_CUTTER_2CSR01"]        = Entry::Mission_Condition::LAND_BUBBLE_CUTTER_2CSR01;
        str_to_Mission_Condition["LAND_GLIDING_CUTTER_2CSR01"]       = Entry::Mission_Condition::LAND_GLIDING_CUTTER_2CSR01;
        str_to_Mission_Condition["LAND_CAT_STANCE_2CSR01"]           = Entry::Mission_Condition::LAND_CAT_STANCE_2CSR01;
        str_to_Mission_Condition["USE_VAPOR_BARRIER_2WMU01"]         = Entry::Mission_Condition::USE_VAPOR_BARRIER_2WMU01;
        str_to_Mission_Condition["ATK_BENT_BACKWARDS_2WMU01"]        = Entry::Mission_Condition::ATK_BENT_BACKWARDS_2WMU01;
        str_to_Mission_Condition["LAND_SNAKE_MUFFLER_2LSA01"]        = Entry::Mission_Condition::LAND_SNAKE_MUFFLER_2LSA01;
        str_to_Mission_Condition["LAND_VICTORY_EVERYTHING_2KRS01"]   = Entry::Mission_Condition::LAND_VICTORY_EVERYTHING_2KRS01;
        str_to_Mission_Condition["USE_BRILLIANT_BLADES_2KRS01"]      = Entry::Mission_Condition::USE_BRILLIANT_BLADES_2KRS01;
        str_to_Mission_Condition["USE_NIGERUNDAYO_2JSP01"]           = Entry::Mission_Condition::USE_NIGERUNDAYO_2JSP01;
        str_to_Mission_Condition["USE_PREDATOR_WINGS_2KRS01"]        = Entry::Mission_Condition::USE_PREDATOR_WINGS_2KRS01;
        str_to_Mission_Condition["LAND_YOU_IDIOT_2KRS01"]            = Entry::Mission_Condition::LAND_YOU_IDIOT_2KRS01;
        str_to_Mission_Condition["LAND_SUPREME_GENES_2KRS01"]        = Entry::Mission_Condition::LAND_SUPREME_GENES_2KRS01;
        str_to_Mission_Condition["LAND_MY_HAMON_2KRS01"]             = Entry::Mission_Condition::LAND_MY_HAMON_2KRS01;
        str_to_Mission_Condition["LAND_INCREDIBLE_POWER_3JTR01"]     = Entry::Mission_Condition::LAND_INCREDIBLE_POWER_3JTR01;
        str_to_Mission_Condition["LAND_RED_BIND_3AVD01"]             = Entry::Mission_Condition::LAND_RED_BIND_3AVD01;
        str_to_Mission_Condition["LAND_THE_HARD_WAY_3AVD01"]         = Entry::Mission_Condition::LAND_THE_HARD_WAY_3AVD01;
        str_to_Mission_Condition["WIN_MY_STAND_JUDGE_3JTR01"]        = Entry::Mission_Condition::WIN_MY_STAND_JUDGE_3JTR01;
        str_to_Mission_Condition["LAND_NO_MATCH_3KKY01"]             = Entry::Mission_Condition::LAND_NO_MATCH_3KKY01;
        str_to_Mission_Condition["LAND_EMERALD_SPLASH_3KKY01"]       = Entry::Mission_Condition::LAND_EMERALD_SPLASH_3KKY01;
        str_to_Mission_Condition["LAND_CROSSFIRE_HURRICANE_3AVD01"]  = Entry::Mission_Condition::LAND_CROSSFIRE_HURRICANE_3AVD01;
        str_to_Mission_Condition["WIN_CROSSFIRE_VARIATION_3AVD01"]   = Entry::Mission_Condition::WIN_CROSSFIRE_VARIATION_3AVD01;
    }
    Entry::Mission_Condition get_mission_condition_ref(std::string ref) {
        if (!str_to_Mission_Condition.contains(ref)) return Entry::Mission_Condition::BLANK;
        return str_to_Mission_Condition[ref];
    }
};

    } // namespace ASBR
} // namespace nucc

#endif // KOJO_NUCC_ASBR_MAINMODEPARAM