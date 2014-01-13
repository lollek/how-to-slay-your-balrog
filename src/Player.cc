#include <cstdlib>

#include "Tables.hh"

#include "Player.hh"

Player::Player() :
  name("null"), gender(0), gold(0), max_exp(0), exp(0),
  exp_frac(0), age(0), height(0), weight(0), level(1),
  max_dlevel(0), mana(0), max_hp(0), ac(0), visual_tohit(0),
  visual_todmg(0), visual_ac(0), visual_tac(0),
  social_class(0), job(0), race(0), current_mana(0), current_hp(0),
  current_mfrac(0), current_hfrac(0), history(), max_stat(), cur_stat(),
  use_stat(), mod_stat(), flag_status(0), flag_rest(0),
  flag_blind(0), flag_paralysis(0), flag_confused(0), 
  flag_food(0), flag_food_digested(0), flag_protection(0),
  flag_speed(0), flag_fast(0), flag_slow(0), flag_afraid(0),
  flag_poisoned(0), flag_image(0), flag_protevil(0), flag_invuln(0),
  flag_hero(0), flag_shero(0), flag_blessed(0), flag_resist_heat(0),
  flag_resist_cold(0), flag_detect_inv(0), flag_word_recall(0),
  flag_tim_infra(0), flag_see_inv(0), flag_teleport(0),
  flag_free_act(0), flag_slow_digest(0), flag_aggravate(0),
  flag_fire_resist(0), flag_cold_resist(0), flag_acid_resist(0),
  flag_regenerate(0), flag_lght_resist(0), flag_ffall(0), 
  flag_sustain_str(0), flag_sustain_dex(0), flag_sustain_con(0),
  flag_sustain_wis(0), flag_sustain_int(0), flag_sustain_chr(0),
  flag_confuse_monster(0), flag_new_spells(0)
{}

void Player::generate()
{
  /* Roll up stats through a 3, 4 and 5-sided die per stat */
  int dice[18];
  int total;
  do
  {
    total = 0;
    for (int i = 0; i < 18; ++i)
      total += dice[i] = rand() % (3 +i%3) +1;
  } while (total <= 42 || 52 <= total);

  /* Sum them and set the stat 
   * See Player.hh for which max_stat is which */
  for (int i = 0; i < 6; ++i)
    this->max_stat[i] = 5 + dice[3*i] + dice[3*i +1] + dice[3*i +2];

  /* Apply racial modifier.. I think this should be modified a bit */
  switch(this->race)
  {
    case 0: /* Human */ 
      /* this->plus_to_hit = tohit_adj */
      /* this->plus_to_dmg = todam_adj */
      /* this->ac = toac_ adj */
      break;

    case 1: /* Half-Elf */
      this->modifyStr(-1); this->modifyDex(1); this->modifyCon(-1);
      this->modifyInt(1);  this->modifyCha(1);
      /* this->plus_to_hit = tohit_adj */
      /* this->plus_to_dmg = todam_adj */
      /* this->ac = toac_adj */
      break;

    case 2: /* Elf */
      this->modifyStr(-1); this->modifyDex(1); this->modifyCon(-2);
      this->modifyWis(1);  this->modifyInt(2); this->modifyCha(1);
      /* this->plus_to_hit = tohit_adj */
      /* this->plus_to_dmg = todam_adj */
      /* this->ac = toac_adj */
      break;

    case 3: /* Halfling */
      this->modifyStr(-2); this->modifyDex(3); this->modifyCon(1);
      this->modifyWis(1);  this->modifyInt(2); this->modifyCha(1);
      /* this->plus_to_hit = tohit_adj */
      /* this->plus_to_dmg = todam_adj */
      /* this->ac = toac_adj */
      break;

    case 4: /* Gnome */
      this->modifyStr(-1); this->modifyDex(2); this->modifyCon(1);
      this->modifyInt(2);  this->modifyCha(-2);
      /* this->plus_to_hit = tohit_adj */
      /* this->plus_to_dmg = todam_adj */
      /* this->ac = toac_adj */
      break;

    case 5: /* Dwarf */
      this->modifyStr(2); this->modifyDex(-2); this->modifyCon(2);
      this->modifyWis(1); this->modifyInt(-3); this->modifyCha(-3);
      /* this->plus_to_hit = tohit_adj */
      /* this->plus_to_dmg = todam_adj */
      /* this->ac = toac_adj */
      break;

    case 6: /* Half-Orc */
      this->modifyStr(2);  this->modifyCon(1);
      this->modifyInt(-1); this->modifyCha(-4);
      /* this->plus_to_hit = tohit_adj */
      /* this->plus_to_dmg = todam_adj */
      /* this->ac = toac_adj */
      break;

    case 7: /* Half-Troll */
      this->modifyStr(4);  this->modifyDex(-4); this->modifyCon(3);
      this->modifyWis(-2); this->modifyInt(-4); this->modifyCha(-6);
      /* this->plus_to_hit = tohit_adj */
      /* this->plus_to_dmg = todam_adj */
      /* this->ac = toac_adj */
      break;

    default: break;
  }

  /* Copy max_stat to cur_stat */
  for (int i = 0; i < 6; ++i)
    this->cur_stat[i] = this->max_stat[i];
    /* TODO: set_use_stat[i] here */


}

int Player::getRace() const { return this->race; }
bool Player::getSex() const { return this->gender; }
int Player::getStr() const { return this->max_stat[0]; }
int Player::getDex() const { return this->max_stat[1]; }
int Player::getCon() const { return this->max_stat[2]; }
int Player::getWis() const { return this->max_stat[3]; }
int Player::getInt() const { return this->max_stat[4]; }
int Player::getCha() const { return this->max_stat[5]; }
int Player::getDisarm() const
{
  const int stat = this->getDex();
  int bonus;

  if      (stat <   3) bonus = -8;
  else if (stat ==  4) bonus = -6;
  else if (stat ==  5) bonus = -4;
  else if (stat ==  6) bonus = -2;
  else if (stat ==  7) bonus = -1;
  else if (stat <  13) bonus =  0;
  else if (stat <  16) bonus =  1;
  else if (stat <  18) bonus =  2;
  else if (stat <  59) bonus =  4;
  else if (stat <  94) bonus =  5;
  else if (stat < 117) bonus =  6;
  else                 bonus =  8;

  return bonus + Tables::races[this->race].b_dis;
}
int Player::getSearchChance() const { return Tables::races[this->race].srh; }
int Player::getSearchFreq() const { return Tables::races[this->race].fos; }
int Player::getStealth() const { return Tables::races[this->race].stl; }
int Player::getBaseToHit() const { return Tables::races[this->race].bth; }
int Player::getBowToHit() const { return Tables::races[this->race].bthb; }
int Player::getSave() const { return Tables::races[this->race].bsav; }
int Player::getHitDie() const { return Tables::races[this->race].bhitdie; }
int Player::getInfra() const { return Tables::races[this->race].infra; }
int Player::getXPFactor() const { return Tables::races[this->race].b_exp; }
int Player::getPlusToHit() const
{
  int total;

  int stat = this->getDex();
  if      (stat <   4) total = -3;
  else if (stat <   6) total = -2;
  else if (stat <   8) total = -1;
  else if (stat <  16) total =  0;
  else if (stat <  17) total =  1;
  else if (stat <  18) total =  2;
  else if (stat <  69) total =  3;
  else if (stat < 118) total =  4;
  else                 total =  5;

  stat = this->getStr();
  if      (stat <   4) total -= 3;
  else if (stat <   5) total -= 2;
  else if (stat <   7) total -= 1;
  else if (stat <  18) total -= 0;
  else if (stat <  94) total += 1;
  else if (stat < 109) total += 2;
  else if (stat < 117) total += 3;
  else                 total += 4;

  return(total);
}
int Player::getPlusToDmg() const
{
  int stat = this->getStr();
  if      (stat <   4) return(-2);
  else if (stat <   5) return(-1);
  else if (stat <  16) return( 0);
  else if (stat <  17) return( 1);
  else if (stat <  18) return( 2);
  else if (stat <  94) return( 3);
  else if (stat < 109) return( 4);
  else if (stat < 117) return( 5);
  else                 return( 6);
}
int Player::getPlusToAC() const
{
  int stat = this->getDex();
  if      (stat <   4) return(-4);
  else if (stat ==  4) return(-3);
  else if (stat ==  5) return(-2);
  else if (stat ==  6) return(-1);
  else if (stat <  15) return( 0);
  else if (stat <  18) return( 1);
  else if (stat <  59) return( 2);
  else if (stat <  94) return( 3);
  else if (stat < 117) return( 4);
  else                 return( 5);
}

void Player::modifyStr(int mod) { this->modifyStat(&this->max_stat[0], mod); }
void Player::modifyDex(int mod) { this->modifyStat(&this->max_stat[1], mod); }
void Player::modifyCon(int mod) { this->modifyStat(&this->max_stat[2], mod); }
void Player::modifyWis(int mod) { this->modifyStat(&this->max_stat[3], mod); }
void Player::modifyInt(int mod) { this->modifyStat(&this->max_stat[4], mod); }
void Player::modifyCha(int mod) { this->modifyStat(&this->max_stat[5], mod); }
void Player::modifyStat(int *stat, int mod)
{
  /* Positive modifier */
  if (mod > 0)
    for (int i = 0; i < mod; ++i)
    {
      if (*stat < 18)
        ++*stat;
      else if (*stat < 88)
        *stat += rand() % 14 + 6;
      else if (*stat < 108)
        *stat += rand() % 5 + 3;
      else if (*stat < 118)
        ++*stat;
    }

  /* Negative modifier */
  else
    for (int i = 0; i > mod; --i)
    {
      if (*stat > 108)
        --*stat;
      else if (*stat > 88)
        *stat -= rand() % 5 + 3;
      else if (*stat > 18)
      {
        *stat -= rand() % 14 + 6;
        if (*stat < 18)
          *stat = 18;
      }
      else if (*stat > 3)
        --*stat;
    }
}

void Player::setRace(int race)
{
  this->race = race;
}

void Player::setSex(int sex)
{
  this->gender = sex;
}





