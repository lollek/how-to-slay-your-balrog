#ifndef __MORIA_TABLES_HH__
#define __MORIA_TABLES_HH__

#include <string>

namespace Tables
{
  const int num_races = 7;
  enum race_t { HUMAN, HALF_ELF, ELF, HALFLING,
                GNOME, HALF_ORC, HALF_TROLL };
  extern struct race_data_t
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
  } races[];

  extern struct background_type
  {
    /* Old typ char * */
    std::string info;       /* History information */

    /* Old type int8u */
    int roll;               /* Die roll needed for history */
    int chart;              /* Table number */
    int next;               /* Pointer to next table */
    int bonux;              /* Bonus to the Social Class +50 */
  } background[];
}

#endif //__MORIA_TABLES_HH__
