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
  this->graphics = new Graphics();
  graphics->drawSplashScreen(this->splashfile);
  this->createCharacter();
}

Game::~Game()
{
  delete this->graphics;
}

int Game::run()
{
  return 0;
}

int Game::createCharacter()
{
  Player player;

  /* Draw the background */
  int status = 0;
  status += graphics->clear();
  status += graphics->print(1, 2, "Name        :");
  status += graphics->print(1, 3, "Race        :");
  status += graphics->print(1, 4, "Sex         :");
  status += graphics->print(1, 5, "Class       :");

  /* Ask for race */
  for (int i = 0; i < Tables::num_races; ++i)
  {
    const int x = 2 + (15 * (i % 5));
    const int y = 21 + (i / 5);
    string line = "a) " + Tables::races[i].name;
    line[0] += i;
    status += graphics->print(x, y, line);
  }
  status += graphics->print(2, 20, "Choose a race (? for Help): ");
  status += graphics->refresh();

  int player_race = graphics->get_key();
  while (player_race < 'a' || ('a' + Tables::num_races -1) < player_race)
  {
    graphics->bell();
    player_race = graphics->get_key();
  }
  player_race -= 'a';
  player.setRace(player_race);

  status += graphics->print(15, 3, Tables::races[player_race].name);
  status += graphics->clear_from(20);

  /* Ask for sex */
  status += graphics->print(2, 21, "m) Male       f) Female");
  status += graphics->print(2, 20, "Choose a sex (? for Help): ");
  status += graphics->refresh();

  int player_sex = graphics->get_key();
  while (player_sex != 'f' && player_sex != 'm')
  {
    graphics->bell();
    player_sex = graphics->get_key();
  }
  player_sex = player_sex == 'm';
  player.setSex(player_sex);
  status += graphics->print(15, 4, player_sex ? "Male" : "Female");

  /* Generate stats */
  bool stats_generation_is_done = false;
  while (!stats_generation_is_done)
  {
    status += graphics->clear_from(10);
    player.generate();

    /* Print history the pretty way */
    graphics->print(27, 14, "Character Background");
    string background_nfo = player.getBackground();
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

    /* Print the stats */
    graphics->println(38, 2, "Age          : " + player.getAgeString());
    graphics->println(38, 3, "Height       : " + player.getHeightString());
    graphics->println(38, 4, "Weight       : " + player.getWeightString());
    graphics->println(38, 5, "Social Class : " + player.getSocialClassString());

    /* Print stats */
    graphics->print(61, 2, "STR :" + player.getStrString());
    graphics->print(61, 3, "DEX :" + player.getDexString());
    graphics->print(61, 4, "CON :" + player.getConString());
    graphics->print(61, 5, "WIS :" + player.getWisString());
    graphics->print(61, 6, "INT :" + player.getIntString());
    graphics->print(61, 7, "CHA :" + player.getChaString());

    graphics->print(1, 9, "+ To Hit    : " + player.getPlusToHitString());
    graphics->print(1,10, "+ To Damage : " + player.getPlusToDmgString());
    graphics->print(1,11, "+ To AC     : " + player.getPlusToACString());
    graphics->print(1,12, "  Total AC  : " + player.getACString());

    graphics->print(2, 20, "Hit space to reroll "
                            "or ESC to accept characteristics: ");
    status += graphics->refresh();

    /* 27 == ESC, 32 == space */
    int key = graphics->get_key();
    while (key != 27 && key != 32)
    {
      graphics->bell();
      key = graphics->get_key();
    }
    if (key == 27)
      break;
  }

  /* Ask for class, we use a bitwise AND for this */
  int class_array[] = { -1, -1, -1, -1, -1, -1 };
  int k = 0;
  for (unsigned i = 0, mask = 0x1; i < Tables::num_jobs; ++i, mask <<= 1)
    if (Tables::races[player.getRace()].rtclass & mask)
    {
      class_array[k] = i;
      const int x = 2 + (15 * (k % 5));
      const int y = 21 + k / 5;
      string tmp_string = "a) " + Tables::jobs[i].title;
      tmp_string[0] += k++;
      graphics->print(x, y, tmp_string);
    }

  graphics->println(2, 20, "Choose a class (? for Help): ");
  int player_job = graphics->get_key();
  while (player_job < 'a' || ('a' + k) <= player_job)
  {
    graphics->bell();
    player_job = graphics->get_key();
  }
  player_job = class_array[player_job - 'a'];
  player.setJob(player_job);

  graphics->clear_from(20);
  graphics->print(15, 5, Tables::jobs[player_job].title);

  graphics->refresh();
  /*
  get_class();
  get_money();
  put_stats();
  put_misc2();
  put_misc3();
  get_name();
  */

  return status;
}
