#include <cstdlib>
#include <string>

#include "Tables.hh"

#include "Player.hh"

using namespace std;

Player::Player() :
  name("null"), gender(0), gold(0), max_exp(0), exp(0),
  exp_frac(0), age(0), height(0), weight(0), level(1),
  max_dlevel(0), mana(0), ac(0),
  social_class(0), job(-1), race(0), current_mana(0), current_hp(0),
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

  this->modifyStr(Tables::races[this->race].str_adj);
  this->modifyDex(Tables::races[this->race].dex_adj);
  this->modifyCon(Tables::races[this->race].con_adj);
  this->modifyWis(Tables::races[this->race].wis_adj);
  this->modifyInt(Tables::races[this->race].int_adj);
  this->modifyCha(Tables::races[this->race].chr_adj);

  /* Copy max_stat to cur_stat */
  for (int i = 0; i < 6; ++i)
    this->cur_stat[i] = this->max_stat[i];
    /* TODO: set_use_stat[i] here */

  /* Add background to player */
  this->history = "";
  this->social_class = rand() % 3 + 1;
  int race_histmod = this->getRace()*3 +1;
  int current = 0;
  do
  {
    bool background_flag = false;
    do
    {
      if (Tables::background[current].chart == race_histmod)
      {
        int test_roll = rand() % 100 + 1;
        while (test_roll > Tables::background[current].roll)
          ++current;
        this->history += Tables::background[current].info;
        this->social_class += Tables::background[current].bonux -50;
        int prev = current;
        if (race_histmod > Tables::background[current].next)
          current = 0;
        race_histmod = Tables::background[prev].next;
        background_flag = true;
      }
      else
        ++current;
    } while (!background_flag);
  } while (race_histmod != 0);

  /* Set social class */
  if (this->social_class > 100)
    this->social_class = 100;
  else if (this->social_class < 1)
    this->social_class = 1;

  /* Set age, width and height */
  this->age = Tables::races[race].b_age + rand() % Tables::races[race].m_age;
  if (this->gender)
  {
    Tables::race_data_t race_d = Tables::races[race];
    this->height = race_d.m_b_ht + rand() % (race_d.m_m_ht - race_d.m_b_ht);
    this->weight = race_d.m_b_wt + rand() % (race_d.m_m_wt - race_d.m_b_wt);
  }
  else
  {
    Tables::race_data_t race_d = Tables::races[race];
    this->height = race_d.f_b_ht + rand() % (race_d.f_m_ht - race_d.f_b_ht);
    this->weight = race_d.f_b_wt + rand() % (race_d.f_m_wt - race_d.f_b_wt);
  }
}

int Player::getRace() const { return this->race; }
bool Player::getSex() const { return this->gender; }
int Player::getAge() const { return this->age; }
int Player::getWeight() const { return this->weight; }
int Player::getHeight() const { return this->height; }
int Player::getSocialClass() const { return this->social_class; }

int Player::getStr() const { return this->max_stat[0]; }
int Player::getDex() const { return this->max_stat[1]; }
int Player::getCon() const { return this->max_stat[2]; }
int Player::getWis() const { return this->max_stat[3]; }
int Player::getInt() const { return this->max_stat[4]; }
int Player::getCha() const { return this->max_stat[5]; }

string Player::getAgeString() const
{
  return this->formatInt(this->getAge()); 
}

string Player::getWeightString() const
{
  return this->formatInt(this->getWeight());
}

string Player::getHeightString() const
{
  return this->formatInt(this->getHeight());
}

string Player::getSocialClassString() const
{
  return this->formatInt(this->getSocialClass());
}

string Player::getPlusToHitString() const
{
  return this->formatInt(this->getPlusToHit());
}

string Player::getPlusToDmgString() const
{
  return this->formatInt(this->getPlusToDmg());
}

string Player::getPlusToACString() const
{
  return this->formatInt(this->getPlusToAC());
}

string Player::getACString() const
{
  return this->formatInt(this->getAC());
}

string Player::formatInt(int val) const
{
  string return_string = "      ";
  int i = return_string.length();
  do
  {
    return_string[--i] = '0' + val % 10;
    val /= 10;
  } while (val != 0 && i > 0);
  return return_string;
}

string Player::getStrString() const { return this->getStatString(0); }
string Player::getDexString() const { return this->getStatString(1); }
string Player::getConString() const { return this->getStatString(2); }
string Player::getWisString() const { return this->getStatString(3); }
string Player::getIntString() const { return this->getStatString(4); }
string Player::getChaString() const { return this->getStatString(5); }
string Player::getStatString(int stat) const
{
  string return_string;
  int value = this->max_stat[stat];
  if (value < 18)
  {
    return_string = "      ";
    return_string[5] = '0' + value % 10;
    if (value >= 10)
      return_string[4] = '1';
  }
  else if (value == 118)
    return_string = "18/100";
  else
  {
    return_string = " 18/  ";
    return_string[5] = '0' + (value -18) % 10;
    return_string[4] = '0' + (value -18) / 10;
  }
  return return_string;
}

int Player::getDisarm() const
{
  const int stat = this->getDex();
  int bonus = Tables::races[this->race].b_dis +
              Tables::jobs[this->job].mdis;

  if      (stat <   3) bonus += -8;
  else if (stat ==  4) bonus += -6;
  else if (stat ==  5) bonus += -4;
  else if (stat ==  6) bonus += -2;
  else if (stat ==  7) bonus += -1;
  else if (stat <  13) bonus +=  0;
  else if (stat <  16) bonus +=  1;
  else if (stat <  18) bonus +=  2;
  else if (stat <  59) bonus +=  4;
  else if (stat <  94) bonus +=  5;
  else if (stat < 117) bonus +=  6;
  else                 bonus +=  8;

  return bonus;
}

int Player::getSearchChance() const
{
  return Tables::races[this->race].srh + Tables::jobs[this->job].msrh;
}

int Player::getSearchFreq() const
{
  return Tables::races[this->race].fos + Tables::jobs[this->job].mfos;
}

int Player::getStealth() const
{
  return Tables::races[this->race].stl + Tables::jobs[this->job].mstl;
}

int Player::getBaseToHit() const
{
  return Tables::races[this->race].bth + Tables::jobs[this->job].mbth;
}

int Player::getBowToHit() const 
{
  return Tables::races[this->race].bthb + Tables::jobs[this->job].mbthb;
}

int Player::getSave() const
{
  return Tables::races[this->race].bsav + Tables::jobs[this->job].msav;
}

int Player::getHitDie() const
{ 
  return Tables::races[this->race].bhitdie + Tables::jobs[this->job].adj_hd;
}

int Player::getInfra() const
{
  return Tables::races[this->race].infra;
}

int Player::getXPFactor() const
{
  return Tables::races[this->race].b_exp + Tables::jobs[this->job].m_exp;
}

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

int Player::getPlusToHP() const
{
  int con = this->getCon();
  if      (con <    7) return(con - 7);
  else if (con <   17) return(0);
  else if (con ==  17) return(1);
  else if (con <   94) return(2);
  else if (con <  117) return(3);
  else                 return(4);
}

int Player::getAC() const
{
  return this->ac + this->getPlusToAC();
}

int Player::getMaxHP() const
{
  int hd = this->getHitDie();
  int ptohp = this->getPlusToHP();
  int maxhp = this->getHitDie() + this->getPlusToHP();
  maxhp += (this->level -1) * (hd/2 + ptohp);
  return maxhp;
}

string Player::getBackground() const
{
  return this->history;
}

void Player::modifyGold(int gold)
{
  this->gold += gold;
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

void Player::setJob(int job)
{
  this->job = job;

  this->modifyStr(Tables::jobs[job].madj_str);
  this->modifyDex(Tables::jobs[job].madj_dex);
  this->modifyCon(Tables::jobs[job].madj_con);
  this->modifyWis(Tables::jobs[job].madj_wis);
  this->modifyInt(Tables::jobs[job].madj_int);
  this->modifyCha(Tables::jobs[job].madj_chr);
  /* TODO: set_use_stat here */

  this->current_hp = this->getMaxHP();
}


