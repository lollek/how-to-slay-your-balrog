#ifndef __MORIA_TABLES_HH__
#define __MORIA_TABLES_HH__

#include <string>

namespace Tables
{
  const int num_races = 7;
  enum race_t { HUMAN, HALF_ELF, ELF, HALFLING,
                GNOME, HALF_ORC, HALF_TROLL };
  struct race_data_t
  {
    std::string name;  /* Type of race (old: char *trace) */
    int str_adj;       /* adjustments  (old: int16) */
    int int_adj;
    int wis_adj;
    int dex_adj;
    int con_adj;
    int chr_adj;
    int b_age;       /* Base age of character    (old: in8u) */
    int m_age;       /* Maximum age of character (old: in8u) */
    int m_b_ht;      /* base height for males    (old: in8u)  */
    int m_m_ht;      /* mod height for males     (old: in8u) */
    int m_b_wt;      /* base weight for males    (old: in8u) */
    int m_m_wt;      /* mod weight for males     (old: in8u) */
    int f_b_ht;      /* base height females      (old: in8u) */
    int f_m_ht;      /* mod height for females   (old: in8u) */
    int f_b_wt;      /* base weight for female   (old: in8u) */
    int f_m_wt;      /* mod weight for females   (old: in8u) */
    int b_dis;       /* base chance to disarm      (old: int16) */
    int srh;         /* base chance for search     (old: int16) */
    int stl;         /* Stealth of character       (old: int16) */
    int fos;         /* frequency of auto search   (old: int16) */
    int bth;         /* adj base chance to hit     (old: int16) */
    int bthb;        /* adj base to hit with bows  (old: int16) */
    int bsav;        /* Race base for saving throw (old: int16) */
    int bhitdie;     /* Base hit points for race     (old: int8u) */
    int infra;       /* See infra-red                (old: int8u) */
    int b_exp;       /* Base experience factor       (old: int8u) */
    int rtclass;     /* Bit field for class types    (old: int8u) */
  } races[] = {
   {"Human",  0,  0,  0,  0,  0,  0,
      14,  6, 72,  6,180, 25, 66,  4,150, 20,
      0,  0,  0,  0,  0,  0,  0, 10,  0, 100, 0x3F,
    },
   {"Half-Elf", -1,  1,  0,  1, -1,  1,
      24, 16, 66,  6,130, 15, 62,  6,100, 10,
      2,  6,  1, -1, -1,  5,  3,  9,  2, 110, 0x3F,
    },
   {"Elf", -1,  2,  1,  1, -2,  1,
      75, 75, 60,  4,100,  6, 54,  4, 80,  6,
      5,  8,  1, -2, -5, 15,  6,  8,  3, 120, 0x1F,
    },
   {"Halfling", -2,  2,  1,  3,  1,  1,
      21, 12, 36,  3, 60,  3, 33,  3, 50,  3,
      15, 12,  4, -5,-10, 20, 18,  6,  4, 110, 0x0B,
    },
   {"Gnome", -1,  2,  0,  2,  1, -2,
      50, 40, 42,  3, 90,  6, 39,  3, 75,  3,
      10,  6,  3, -3, -8, 12, 12,  7,  4, 125, 0x0F,
    },
   {"Dwarf",  2, -3,  1, -2,  2, -3,
      35, 15, 48,  3,150, 10, 46,  3,120, 10,
      2,  7,  -1,  0, 15,  0,  9,  9,  5, 120, 0x05,
    },
   {"Half-Orc",  2, -1,  0,  0,  1, -4,
      11,  4, 66,  1,150,  5, 62,  1,120,  5,
      -3,  0, -1,  3, 12, -5, -3, 10,  3, 110, 0x0D,
    },
   {"Half-Troll",4, -4, -2, -4,  3, -6,
      20, 10, 96, 10,255, 50, 84,  8,225, 40,
      -5, -1, -2,  5, 20,-10, -8, 12,  3, 120, 0x05,
    }
  };


  struct background_type
  {
    /* Old typ char * */
    std::string info;       /* History information */

    /* Old type int8u */
    int roll;               /* Die roll needed for history */
    int chart;              /* Table number */
    int next;               /* Pointer to next table */
    int bonux;              /* Bonus to the Social Class +50 */
  } background[] = {
    {"You are the illegitimate and unacknowledged child ",   10, 1, 2, 25},
    {"You are the illegitimate but acknowledged child ",   20, 1, 2, 35},
    {"You are one of several children ",   95, 1, 2, 45},
    {"You are the first child ",  100, 1, 2, 50},
    {"of a Serf. ",   40, 2, 3, 65},
    {"of a Yeoman. ",   65, 2, 3, 80},
    {"of a Townsman. ",   80, 2, 3, 90},
    {"of a Guildsman. ",   90, 2, 3,105},
    {"of a Landed Knight. ",   96, 2, 3,120},
    {"of a Titled Noble. ",   99, 2, 3,130},
    {"of a Royal Blood Line. ",  100, 2, 3,140},
    {"You are the black sheep of the family. ",   20, 3,50, 20},
    {"You are a credit to the family. ",   80, 3,50, 55},
    {"You are a well liked child. ",  100, 3,50, 60},
    {"Your mother was a Green-Elf. ",   40, 4, 1, 50},
    {"Your father was a Green-Elf. ",   75, 4, 1, 55},
    {"Your mother was a Grey-Elf. ",   90, 4, 1, 55},
    {"Your father was a Grey-Elf. ",   95, 4, 1, 60},
    {"Your mother was a High-Elf. ",   98, 4, 1, 65},
    {"Your father was a High-Elf. ",  100, 4, 1, 70},
    {"You are one of several children ",   60, 7, 8, 50},
    {"You are the only child ",  100, 7, 8, 55},
    {"of a Green-Elf ",   75, 8, 9, 50},
    {"of a Grey-Elf ",   95, 8, 9, 55},
    {"of a High-Elf ",  100, 8, 9, 60},
    {"Ranger. ",   40, 9,54, 80},
    {"Archer. ",        70, 9,54, 90},
    {"Warrior. ",        87, 9,54,110},
    {"Mage. ",        95, 9,54,125},
    {"Prince. ",        99, 9,54,140},
    {"King. ",       100, 9,54,145},
    {"You are one of several children of a Halfling ",   85,10,11, 45},
    {"You are the only child of a Halfling ",   100,10,11, 55},
    {"Bum. ",        20,11, 3, 55},
    {"Tavern Owner. ",       30,11, 3, 80},
    {"Miller. ",        40,11, 3, 90},
    {"Home Owner. ",       50,11, 3,100},
    {"Burglar. ",        80,11, 3,110},
    {"Warrior. ",        95,11, 3,115},
    {"Mage. ",        99,11, 3,125},
    {"Clan Elder. ",      100,11, 3,140},
    {"You are one of several children of a Gnome ",    85,13,14, 45},
    {"You are the only child of a Gnome ",    100,13,14, 55},
    {"Beggar. ",        20,14, 3, 55},
    {"Braggart. ",        50,14, 3, 70},
    {"Prankster. ",       75,14, 3, 85},
    {"Warrior. ",        95,14, 3,100},
    {"Mage. ",       100,14, 3,125},
    {"You are one of two children of a Dwarven ",    25,16,17, 40},
    {"You are the only child of a Dwarven ",   100,16,17, 50},
    {"Thief. ",        10,17,18, 60},
    {"Prison Guard. ",       25,17,18, 75},
    {"Miner. ",        75,17,18, 90},
    {"Warrior. ",        90,17,18,110},
    {"Priest. ",        99,17,18,130},
    {"King. ",       100,17,18,150},
    {"You are the black sheep of the family. ",    15,18,57, 10},
    {"You are a credit to the family. ",     85,18,57, 50},
    {"You are a well liked child. ",    100,18,57, 55},
    {"Your mother was an Orc, but it is unacknowledged. ",   25,19,20, 25},
    {"Your father was an Orc, but it is unacknowledged. ",  100,19,20, 25},
    {"You are the adopted child ",     100,20, 2, 50},
    {"Your mother was a Cave-Troll ",     30,22,23, 20},
    {"Your father was a Cave-Troll ",     60,22,23, 25},
    {"Your mother was a Hill-Troll ",     75,22,23, 30},
    {"Your father was a Hill-Troll ",     90,22,23, 35},
    {"Your mother was a Water-Troll ",     95,22,23, 40},
    {"Your father was a Water-Troll ",    100,22,23, 45},
    {"Cook. ",         5,23,62, 60},
    {"Warrior. ",        95,23,62, 55},
    {"Shaman. ",        99,23,62, 65},
    {"Clan Chief. ",      100,23,62, 80},
    {"You have dark brown eyes, ",      20,50,51, 50},
    {"You have brown eyes, ",      60,50,51, 50},
    {"You have hazel eyes, ",      70,50,51, 50},
    {"You have green eyes, ",      80,50,51, 50},
    {"You have blue eyes, ",      90,50,51, 50},
    {"You have blue-gray eyes, ",     100,50,51, 50},
    {"straight ",        70,51,52, 50},
    {"wavy ",        90,51,52, 50},
    {"curly ",       100,51,52, 50},
    {"black hair, ",       30,52,53, 50},
    {"brown hair, ",       70,52,53, 50},
    {"auburn hair, ",       80,52,53, 50},
    {"red hair, ",        90,52,53, 50},
    {"blond hair, ",      100,52,53, 50},
    {"and a very dark complexion.",      10,53, 0, 50},
    {"and a dark complexion.",      30,53, 0, 50},
    {"and an average complexion.",      80,53, 0, 50},
    {"and a fair complexion.",      90,53, 0, 50},
    {"and a very fair complexion.",     100,53, 0, 50},
    {"You have light grey eyes, ",      85,54,55, 50},
    {"You have light blue eyes, ",      95,54,55, 50},
    {"You have light green eyes, ",     100,54,55, 50},
    {"straight ",        75,55,56, 50},
    {"wavy ",       100,55,56, 50},
    {"black hair, and a fair complexion.",     75,56, 0, 50},
    {"brown hair, and a fair complexion.",     85,56, 0, 50},
    {"blond hair, and a fair complexion.",     95,56, 0, 50},
    {"silver hair, and a fair complexion.",    100,56, 0, 50},
    {"You have dark brown eyes, ",      99,57,58, 50},
    {"You have glowing red eyes, ",     100,57,58, 60},
    {"straight ",        90,58,59, 50},
    {"wavy ",       100,58,59, 50},
    {"black hair, ",       75,59,60, 50},
    {"brown hair, ",      100,59,60, 50},
    {"a one foot beard, ",       25,60,61, 50},
    {"a two foot beard, ",       60,60,61, 51},
    {"a three foot beard, ",      90,60,61, 53},
    {"a four foot beard, ",      100,60,61, 55},
    {"and a dark complexion.",     100,61, 0, 50},
    {"You have slime green eyes, ",      60,62,63, 50},
    {"You have puke yellow eyes, ",      85,62,63, 50},
    {"You have blue-bloodshot eyes, ",     99,62,63, 50},
    {"You have glowing red eyes, ",     100,62,63, 55},
    {"dirty ",        33,63,64, 50},
    {"mangy ",        66,63,64, 50},
    {"oily ",       100,63,64, 50},
    {"sea-weed green hair, ",      33,64,65, 50},
    {"bright red hair, ",       66,64,65, 50},
    {"dark purple hair, ",      100,64,65, 50},
    {"and green ",        25,65,66, 50},
    {"and blue ",        50,65,66, 50},
    {"and white ",        75,65,66, 50},
    {"and black ",       100,65,66, 50},
    {"ulcerous skin.",       33,66, 0, 50},
    {"scabby skin.",       66,66, 0, 50},
    {"leprous skin.",      100,66, 0, 50}
  };
}

#endif //__MORIA_TABLES_HH__