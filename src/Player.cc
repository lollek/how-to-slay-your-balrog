#include <cstdlib>

#include "Player.hh"

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
