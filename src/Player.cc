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
    max_stat[i] = 5 + dice[3*i] + dice[3*i +1] + dice[3*i +2];

  modifyStr(Tables::races[race].str_adj);
  modifyDex(Tables::races[race].dex_adj);
  modifyCon(Tables::races[race].con_adj);
  modifyWis(Tables::races[race].wis_adj);
  modifyInt(Tables::races[race].int_adj);
  modifyCha(Tables::races[race].chr_adj);

  /* Copy max_stat to cur_stat */
  for (int i = 0; i < 6; ++i)
    cur_stat[i] = max_stat[i];
    /* TODO: set_use_stat[i] here */

  /* Add background to player */
  history = "";
  social_class = rand() % 3 + 1;
  int race_histmod = getRace()*3 +1;
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
        history += Tables::background[current].info;
        social_class += Tables::background[current].bonux -50;
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
  if (social_class > 100)
    social_class = 100;
  else if (social_class < 1)
    social_class = 1;

  /* Set age, width and height */
  age = Tables::races[race].b_age + rand() % Tables::races[race].m_age;
  if (gender)
  {
    Tables::race_data_t race_d = Tables::races[race];
    height = race_d.m_b_ht + rand() % (race_d.m_m_ht - race_d.m_b_ht);
    weight = race_d.m_b_wt + rand() % (race_d.m_m_wt - race_d.m_b_wt);
  }
  else
  {
    Tables::race_data_t race_d = Tables::races[race];
    height = race_d.f_b_ht + rand() % (race_d.f_m_ht - race_d.f_b_ht);
    weight = race_d.f_b_wt + rand() % (race_d.f_m_wt - race_d.f_b_wt);
  }
}

int Player::getRace() const { return race; }
bool Player::getSex() const { return gender; }
int Player::getAge() const { return age; }
int Player::getWeight() const { return weight; }
int Player::getHeight() const { return height; }
int Player::getSocialClass() const { return social_class; }
int Player::getLevel() const { return level; }
int Player::getXP() const { return exp; }
int Player::getMaxXP() const { return max_exp; }
int Player::getXPToLevel() const { return Tables::xp_tables[getLevel() -1]; }

int Player::getStr() const { return max_stat[0]; }
int Player::getDex() const { return max_stat[1]; }
int Player::getCon() const { return max_stat[2]; }
int Player::getWis() const { return max_stat[3]; }
int Player::getInt() const { return max_stat[4]; }
int Player::getCha() const { return max_stat[5]; }

string Player::getAgeString() const { return formatInt(getAge()); }
string Player::getWeightString() const { return formatInt(getWeight()); }
string Player::getHeightString() const { return formatInt(getHeight()); }
string Player::getSocialClassString() const { return formatInt(getSocialClass()); }
string Player::getLevelString() const { return formatInt(level); }
string Player::getXPString() const { return formatInt(getXP()); }
string Player::getMaxXPString() const { return formatInt(getMaxXP()); }
string Player::getXPToLevelString() const { return formatInt(getXPToLevel()) ; }
string Player::getPlusToHitString() const { return formatInt(getPlusToHit()); }
string Player::getPlusToDmgString() const { return formatInt(getPlusToDmg()); }
string Player::getPlusToACString() const { return formatInt(getPlusToAC()); }
string Player::getACString() const { return formatInt(getAC()); }

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

string Player::getStrString() const { return getStatString(0); }
string Player::getDexString() const { return getStatString(1); }
string Player::getConString() const { return getStatString(2); }
string Player::getWisString() const { return getStatString(3); }
string Player::getIntString() const { return getStatString(4); }
string Player::getChaString() const { return getStatString(5); }
string Player::getStatString(int stat) const
{
  string return_string;
  int value = max_stat[stat];
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
  const int stat = getDex();
  int bonus = Tables::races[race].b_dis +
              Tables::jobs[job].mdis;

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
  return Tables::races[race].srh + Tables::jobs[job].msrh;
}

int Player::getSearchFreq() const
{
  return Tables::races[race].fos + Tables::jobs[job].mfos;
}

int Player::getStealth() const
{
  return Tables::races[race].stl + Tables::jobs[job].mstl;
}

int Player::getBaseToHit() const
{
  return Tables::races[race].bth + Tables::jobs[job].mbth;
}

int Player::getBowToHit() const 
{
  return Tables::races[race].bthb + Tables::jobs[job].mbthb;
}

int Player::getSave() const
{
  return Tables::races[race].bsav + Tables::jobs[job].msav;
}

int Player::getHitDie() const
{ 
  return Tables::races[race].bhitdie + Tables::jobs[job].adj_hd;
}

int Player::getInfra() const
{
  return Tables::races[race].infra;
}

int Player::getXPFactor() const
{
  return Tables::races[race].b_exp + Tables::jobs[job].m_exp;
}

int Player::getPlusToHit() const
{
  int total;

  int stat = getDex();
  if      (stat <   4) total = -3;
  else if (stat <   6) total = -2;
  else if (stat <   8) total = -1;
  else if (stat <  16) total =  0;
  else if (stat <  17) total =  1;
  else if (stat <  18) total =  2;
  else if (stat <  69) total =  3;
  else if (stat < 118) total =  4;
  else                 total =  5;

  stat = getStr();
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
  int stat = getStr();
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
  int stat = getDex();
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
  int con = getCon();
  if      (con <    7) return(con - 7);
  else if (con <   17) return(0);
  else if (con ==  17) return(1);
  else if (con <   94) return(2);
  else if (con <  117) return(3);
  else                 return(4);
}

int Player::getAC() const
{
  return ac + getPlusToAC();
}

int Player::getMaxHP() const
{
  int hd = getHitDie();
  int ptohp = getPlusToHP();
  int maxhp = getHitDie() + getPlusToHP();
  maxhp += (level -1) * (hd/2 + ptohp);
  return maxhp;
}

string Player::getBackground() const
{
  return history;
}

void Player::modifyGold(int gold)
{
  gold += gold;
}

void Player::modifyStr(int mod) { modifyStat(&max_stat[0], mod); }
void Player::modifyDex(int mod) { modifyStat(&max_stat[1], mod); }
void Player::modifyCon(int mod) { modifyStat(&max_stat[2], mod); }
void Player::modifyWis(int mod) { modifyStat(&max_stat[3], mod); }
void Player::modifyInt(int mod) { modifyStat(&max_stat[4], mod); }
void Player::modifyCha(int mod) { modifyStat(&max_stat[5], mod); }
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
  race = race;
}

void Player::setSex(int sex)
{
  gender = sex;
}

void Player::setJob(int job)
{
  job = job;

  modifyStr(Tables::jobs[job].madj_str);
  modifyDex(Tables::jobs[job].madj_dex);
  modifyCon(Tables::jobs[job].madj_con);
  modifyWis(Tables::jobs[job].madj_wis);
  modifyInt(Tables::jobs[job].madj_int);
  modifyCha(Tables::jobs[job].madj_chr);
  /* TODO: set_use_stat here */

  current_hp = getMaxHP();
}


