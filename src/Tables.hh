#ifndef __MORIA_TABLES_HH__
#define __MORIA_TABLES_HH__

#include <string>

namespace Tables
{
  const int max_player_level = 40;
  const int num_jobs = 6;
  const int num_races = 7;

  enum job_t { WARRIOR, MAGE, PRIEST, ROGUE, RANGER, PALADIN, NONE };
  enum race_t { HUMAN, HALF_ELF, ELF, HALFLING, GNOME, HALF_ORC, HALF_TROLL };

  extern struct job_data_t
  {
    std::string title;     /* type of class, old char*  */

    /* Old type int8u */
    int adj_hd;             /* Adjust hit points  */
    int mdis;               /* mod disarming traps  */
    int msrh;               /* modifier to searching */
    int mstl;               /* modifier to stealth  */
    int mfos;               /* modifier to freq-of-search */
    int mbth;               /* modifier to base to hit */
    int mbthb;              /* modifier to base to hit - bows*/
    int msav;               /* Class modifier to save */

    /* Old type int16 */
    int madj_str;           /* Class modifier for strength */
    int madj_int;           /* Class modifier for intelligence*/
    int madj_wis;           /* Class modifier for wisdom */
    int madj_dex;           /* Class modifier for dexterity */
    int madj_con;           /* Class modifier for constitution*/
    int madj_chr;           /* Class modifier for charisma */

    /* Old type int8u */
    job_t spell;            /* class use mage spells */
    int m_exp;              /* Class experience factor */
    int first_spell_lev;    /* First level where class can use spells. */
  } jobs[];


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
