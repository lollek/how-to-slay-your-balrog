#ifndef __MORIA_PLAYER_HH__
#define __MORIA_PLAYER_HH__

#include <string>

class Player
{
  public:
    Player();

    Player(const Player&) = delete;
    void operator=(const Player&) = delete;

  private:
    /* Old type: char[] */
    std::string name; /* Name of character */

    /* Old type: int8u */
    bool gender;      /* Gender of character */

    /* Old type: int32 */
    int gold;         /* Gold of character, old: au */
    int max_exp;      /* Max experience */
    int exp;          /* Current experience */

    /* Old type: int16u */
    int exp_frac;      /* Current xp fraction * 2^16 */
    int age;           /* Age of character */
    int height;        /* Height of character, old: ht */
    int weight;        /* Weight of character, old: wt */
    int level;         /* Character level, old: lev */
    int max_dlevel;    /* max dungeon level explored, old: max_dlv */

    /* Old type: int16 */
    int search_chance; /* chance in search, old: srh */
    int search_freq;   /* Frenq of search, old: fos */
    int base_to_hit;   /* Base to hit, old: bth */
    int bow_to_hit;    /* Base to hit w/ bow, old: bthb */
    int mana;          /* Mana points */
    int max_hp;        /* Max hitpoints, old: mhp */
    int plus_to_hit;   /* Plusses to hit, old: ptohit */
    int plus_to_dmg;   /* Plusses to dmg, old: ptodam */
    int ac;            /* Total AC, old: pac */
    int plus_to_ac;    /* Magic AC, old: ptoac */
    int visual_tohit;  /* Display ToHit, old: dis_th */
    int visual_todmg;  /* Display ToDam, old: dis_td */
    int visual_ac;     /* Display AC, old: dis_ac */
    int visual_tac;    /* DIsplay ToTAC, old: dis_tac */
    int disarm;        /* % to disarm */
    int save;          /* Saving throw */
    int social_class;  /* Social class, old: sc */
    int stealth;       /* Stealth factor, old: stl */

    /* Old type: int8u */
    int job;           /* Class, old: pclass */
    int race;          /* Race, old: prace */
    int hitdie;        /* Hit die */
    int xp_factor;     /* Experience factor, old: expfact */

    /* Old type: int16 */
    int current_mana;  /* Current mana points, old: cmana */
    int current_hp;    /* Current hit points, old: chp */

    /* Old type: int16u */
    int current_mfrac; /* Current mana fraction, old: cmana_frac */
    int current_hfrac; /* Current hp fraction, old: chp_frac */

    /* Old type char[4][60] */
    std::string history[4]; /* History record */
};

#endif //__MORIA_PLAYER_HH__
