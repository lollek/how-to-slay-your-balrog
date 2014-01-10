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
}

#endif //__MORIA_TABLES_HH__
