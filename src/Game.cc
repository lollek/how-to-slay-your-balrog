#include <cstring>
#include <cstdlib>

#include <iostream>

#include "Game.hh"
#include "Tables.hh"

using namespace std;

Game::Game(const string &savefile, unsigned seed) :
  savefile(savefile),
  seed(seed),
  graphics(NULL),
  player(NULL)
{
  graphics = new Graphics();
  graphics->drawSplashScreen(splashfile);
  createCharacter();
}

Game::~Game()
{
  delete player;
  delete graphics;
}

int Game::run()
{
  return 0;
}

string Game::to6dStr(int value) const
{
  string return_string = "      ";
  int i = return_string.length();
  do
  {
    return_string[--i] = '0' + value % 10;
    value /= 10;
  } while (value != 0 && i > 0);
  return return_string;
}

void Game::createCharacter()
{
  player = new Player;

  /* Draw the background */
  graphics->clear();
  graphics->print(1, 2, "Name        :");
  graphics->print(1, 3, "Race        :");
  graphics->print(1, 4, "Sex         :");
  graphics->print(1, 5, "Class       :");

  CreateSub_selectRace();
  CreateSub_selectSex();
  CreateSub_generatePlayer();
  CreateSub_selectJob();
  CreateSub_addStartingMoney();
  CreateSub_printFightingStats();

  graphics->clear_from(14);
  CreateSub_printMiscStats();

  graphics->refresh();
  /*
  put_misc2();
  put_misc3();
  get_name();
  */

}

void Game::CreateSub_selectRace()
{
  for (int i = 0; i < Tables::num_races; ++i)
  {
    const int x = 2 + (15 * (i % 5));
    const int y = 21 + (i / 5);
    string line = "a) " + Tables::races[i].name;
    line[0] += i;
    graphics->print(x, y, line);
  }
  graphics->print(2, 20, "Choose a race (? for Help): ");
  graphics->refresh();

  int player_race = graphics->get_key();
  while (player_race < 'a' || ('a' + Tables::num_races -1) < player_race)
  {
    graphics->bell();
    player_race = graphics->get_key();
  }
  player_race -= 'a';
  player->setRace(player_race);

  graphics->print(15, 3, Tables::races[player_race].name);
  graphics->clear_from(20);
}

void Game::CreateSub_selectSex()
{
  graphics->print(2, 21, "m) Male       f) Female");
  graphics->print(2, 20, "Choose a sex (? for Help): ");
  graphics->refresh();

  int player_sex = graphics->get_key();
  while (player_sex != 'f' && player_sex != 'm')
  {
    graphics->bell();
    player_sex = graphics->get_key();
  }
  player_sex = player_sex == 'm';
  player->setSex(player_sex);
  graphics->print(15, 4, player_sex ? "Male" : "Female");
}

void Game::CreateSub_generatePlayer()
{
  for (;;)
  {
    graphics->clear_from(10);
    player->generate();
    CreateSub_printBackground();
    CreateSub_printSocialStats();
    CreateSub_printFightingStats();
    graphics->print(2, 20, "Hit space to reroll or ESC to "
                           "accept characteristics: ");
    graphics->refresh();

    /* 27 == ESC, 32 == space */
    int key = graphics->get_key();
    while (key != 27 && key != 32)
    {
      graphics->bell();
      key = graphics->get_key();
    }
    if (key == 27)
      return;
  }
}

void Game::CreateSub_printBackground() const
{
  graphics->print(27, 14, "Character Background");
  string background_nfo = player->getBackground();

  int y = 15;
  unsigned start = 0;
  unsigned stop = 0;
  while ((stop = start + 59) < background_nfo.length())
  {
    while (background_nfo[stop] != ' ')
      --stop;
    graphics->println(10, y++, background_nfo.substr(start, stop-start));
    start = stop +1;
  }
  graphics->println(10, y++, background_nfo.substr(start));
}

void Game::CreateSub_printSocialStats() const
{
  graphics->println(38, 2, "Age          : " + to6dStr(player->getAge()));
  graphics->println(38, 3, "Height       : " + to6dStr(player->getHeight()));
  graphics->println(38, 4, "Weight       : " + to6dStr(player->getWeight()));
  graphics->println(38, 5, "Social Class : " + to6dStr(player->getSocialClass()));
}

void Game::CreateSub_printFightingStats() const
{
  graphics->print(61, 2, "STR :" + to6dStr(player->getStr()));
  graphics->print(61, 3, "DEX :" + to6dStr(player->getDex()));
  graphics->print(61, 4, "CON :" + to6dStr(player->getCon()));
  graphics->print(61, 5, "WIS :" + to6dStr(player->getWis()));
  graphics->print(61, 6, "INT :" + to6dStr(player->getInt()));
  graphics->print(61, 7, "CHA :" + to6dStr(player->getCha()));

  graphics->print(1, 9, "+ To Hit    : " + to6dStr(player->getPlusToHit()));
  graphics->print(1,10, "+ To Damage : " + to6dStr(player->getPlusToDmg()));
  graphics->print(1,11, "+ To AC     : " + to6dStr(player->getPlusToAC()));
  graphics->print(1,12, "  Total AC  : " + to6dStr(player->getAC()));
}

void Game::CreateSub_printMiscStats() const
{
  graphics->print(29, 9, "Level      : " + to6dStr(player->getLevel()));
  graphics->print(29,10, "Experience : " + player->getXP());
  graphics->print(29,11, "Max Exp    : " + player->getMaxXP());
  string xp_to_advance = player->getLevel() == Tables::max_player_level ?
                          "******" : to6dStr(player->getXPToLevel());
  graphics->print(29,12, "Exp to Adv.: " + xp_to_advance);
  graphics->print(29,13, "Gold       : " + to6dStr(player->getGold()));
  graphics->print(52, 9, "Max Hit Points : " + to6dStr(player->getMaxHP()));
  graphics->print(52,10, "Cur Hit Points : "); /* mhp */
  graphics->print(52,11, "Max Mana       : "); /* mana */
  graphics->print(52,12, "Cur Mana       : "); /* cmana */
}

void Game::CreateSub_selectJob()
{
  int class_array[] = { -1, -1, -1, -1, -1, -1 };
  int k = 0;
  int race = player->getRace();

  /* Print available jobs and add them to class_array */
  for (unsigned i = 0, mask = 0x1; i < Tables::num_jobs; ++i, mask <<= 1)
    if (Tables::races[race].rtclass & mask)
    {
      class_array[k] = i;
      const int x = 2 + (15 * (k % 5));
      const int y = 21 + k / 5;
      string tmp_string = "a) " + Tables::jobs[i].title;
      tmp_string[0] += k++;
      graphics->print(x, y, tmp_string);
    }

  /* Choose job from class_array */
  graphics->println(2, 20, "Choose a class (? for Help): ");
  int player_job = graphics->get_key();
  while (player_job < 'a' || ('a' + k) <= player_job)
  {
    graphics->bell();
    player_job = graphics->get_key();
  }
  player_job = class_array[player_job - 'a'];
  player->setJob(player_job);
  graphics->print(15, 5, Tables::jobs[player_job].title);
}

void Game::CreateSub_addStartingMoney()
{
  int base_gold = 325;
  int negative = (player->getStr() - 10 +
                  player->getDex() - 10 +
                  player->getCon() - 10 +
                  player->getWis() - 10 +
                  player->getInt() - 10 ) * 5;
  int gender_bonus = player->getSex() == 'f' ? 50 : 0;
  int family_gold = player->getSocialClass() * 6;
  int charisma_gold = player->getCha();

  int gold = base_gold + family_gold + charisma_gold + gender_bonus -
             negative + rand() % 25 + 1;
  if (gold < 80)
    gold = 80;

  this->player->modifyGold(gold);
}

