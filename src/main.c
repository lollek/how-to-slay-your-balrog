/* UNIX Moria Version 5.x
   source/main.c: initialization, main() function and main loop
   Copyright (c) 1989-92 James E. Wilson, Robert A. Koeneke

   This software may be copied and distributed for educational, research, and
   not for profit purposes provided that this copyright and statement are
   included in all such copies. */


/* Original copyright message follows. */

/* Moria Version 4.8	COPYRIGHT (c) Robert Alan Koeneke		*/
/*									 */
/*	 I lovingly dedicate this game to hackers and adventurers	 */
/*	 everywhere...							 */
/*									 */
/*									 */
/*	 Designer and Programmer : Robert Alan Koeneke			 */
/*				   University of Oklahoma		 */
/*									 */
/*	 Assistant Programmers	 : Jimmey Wayne Todd			 */
/*				   University of Oklahoma		 */
/*									 */
/*				   Gary D. McAdoo			 */
/*				   University of Oklahoma		 */
/*									 */
/*	 UNIX Port		 : James E. Wilson			 */
/*				   UC Berkeley				 */
/*				   wilson@kithrup.com			 */
/*									 */
/*	 MSDOS Port		 : Don Kneller				 */
/*				   1349 - 10th ave			 */
/*				   San Francisco, CA 94122		 */
/*				   kneller@cgl.ucsf.EDU			 */
/*				   ...ucbvax!ucsfcgl!kneller		 */
/*				   kneller@ucsf-cgl.BITNET		 */
/*									 */
/*	 BRUCE Moria		 : Christopher Stuart			 */
/*				   Monash University			 */
/*				   Melbourne, Victoria, AUSTRALIA	 */
/*				   cjs@moncsbruce.oz			 */
/*									 */
/*       Amiga Port              : Corey Gehman                          */
/*                                 Clemson University                    */
/*                                 cg377170@eng.clemson.edu              */
/*									 */
/*	 Version 5.5		 : David Grabiner			 */
/*				   Harvard University			 */
/*				   grabiner@math.harvard.edu		 */
/*                                                                       */
/*	 Moria may be copied and modified freely as long as the above	 */
/*	 credits are retained.	No one who-so-ever may sell or market	 */
/*	 this software in any form without the expressed written consent */
/*	 of the author Robert Alan Koeneke.				 */
/*									 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "config.h"
#include "constant.h"
#include "types.h"
#include "externs.h"

long time();

static void char_inven_init();
static void init_m_level();
static void init_t_level();
static void price_adjust();

/* Initialize, restore, and get the ball rolling.	-RAK-	*/
int main(int argc, char *argv[])
{
  int32u seed;
  int generate;
  int result;
  char *p;
  int new_game = FALSE;
  int force_rogue_like = FALSE;
  int force_keys_to;

  /* default command set defined in config.h file */
  rogue_like_commands = TRUE;

  /* call this routine to grab a file pointer to the highscore file */
  /* and prepare things to relinquish setuid privileges */
  init_scorefile();

  if (0 != setuid(getuid()))
    {
      perror("Can't set permissions correctly!  Setuid call failed.\n");
      exit(0);
    }
  if (0 != setgid(getgid()))
    {
      perror("Can't set permissions correctly!  Setgid call failed.\n");
      exit(0);
    }

  /* use curses */
  init_curses();

  seed = 0; /* let wizard specify rng seed */
  /* check for user interface option */
  for (--argc, ++argv; argc > 0 && argv[0][0] == '-'; --argc, ++argv)
    switch (argv[0][1])
      {
      case 'N':
      case 'n': new_game = TRUE; break;
      case 'O':
      case 'o':
	/* rogue_like_commands may be set in get_char(), so delay this
	   until after read savefile if any */
	force_rogue_like = TRUE;
	force_keys_to = FALSE;
	break;
      case 'R':
      case 'r':
	force_rogue_like = TRUE;
	force_keys_to = TRUE;
	break;
      case 'S': display_scores(TRUE); exit_game();
      case 's': display_scores(FALSE); exit_game();
      case 'W':
      case 'w':
	to_be_wizard = TRUE;

	if (isdigit((int)argv[0][2]))
	  seed = atoi(&argv[0][2]);
	break;
      default: (void) printf("Usage: moria [-norsw] [savefile]\n");
	exit_game();
      }

  /* Check operating hours			*/
  /* If not wizard  No_Control_Y	       */
  read_times();

  /* Some necessary initializations		*/
  /* all made into constants or initialized in variables.c */

  price_adjust();

  /* Grab a random seed from the clock		*/
  init_seeds(seed);

  /* Init monster and treasure levels for allocate */
  init_m_level();
  init_t_level();

  /* Init the store inventories			*/
  store_init();

  /* Auto-restart of saved file */
  if (argv[0] != CNIL)
    (void) strcpy (savefile, argv[0]);
  else if ((p = getenv("MORIA_SAV")) != CNIL)
    (void) strcpy(savefile, p);
  else if ((p = getenv("HOME")) != CNIL)
    (void) sprintf(savefile, "%s/%s", p, MORIA_SAV);
  else
    (void) strcpy(savefile, MORIA_SAV);

/* This restoration of a saved character may get ONLY the monster memory. In
   this case, get_char returns false. It may also resurrect a dead character
   (if you are the wizard). In this case, it returns true, but also sets the
   parameter "generate" to true, as it does not recover any cave details. */

  result = FALSE;
  if ((new_game == FALSE) && !access(savefile, 0) && get_char(&generate))
    result = TRUE;

  /* enter wizard mode before showing the character display, but must wait
     until after get_char in case it was just a resurrection */
  if (to_be_wizard)
    if (!enter_wiz_mode())
      exit_game();

  if (result)
    {
      change_name();

      /* could be restoring a dead character after a signal or HANGUP */
      if (py.misc.chp < 0)
	death = TRUE;
    }
  else
    {	  /* Create character	   */
      create_character();
      birth_date = time ((long *)0);
      char_inven_init();
      py.flags.food = 7500;
      py.flags.food_digested = 2;
      if (class[py.misc.pclass].spell == MAGE)
	{	  /* Magic realm   */
	  clear_screen(); /* makes spell list easier to read */
	  calc_spells(A_INT);
	  calc_mana(A_INT);
	}
      else if (class[py.misc.pclass].spell == PRIEST)
	{	  /* Clerical realm*/
	  calc_spells(A_WIS);
	  clear_screen(); /* force out the 'learn prayer' message */
	  calc_mana(A_WIS);
	}
      /* prevent ^c quit from entering score into scoreboard,
	 and prevent signal from creating panic save until this point,
	 all info needed for save file is now valid */
      character_generated = 1;
      generate = TRUE;
    }

  if (force_rogue_like)
    rogue_like_commands = force_keys_to;

  magic_init();

  /* Begin the game				*/
  clear_screen();
  prt_stat_block();
  if (generate)
    generate_cave();

  /* Loop till dead, or exit			*/
  while(!death)
    {
      dungeon();				  /* Dungeon logic */

      /* check for eof here, see inkey() in io.c */
      /* eof can occur if the process gets a HANGUP signal */
      if (eof_flag)
	{
	  (void) strcpy(died_from, "(end of input: saved)");
	  if (!save_char())
	    {
	      (void) strcpy(died_from, "unexpected eof");
	    }
	  /* should not reach here, by if we do, this guarantees exit */
	  death = TRUE;
	}

      if (!death) generate_cave();	       /* New level	*/
    }

  exit_game();		/* Character gets buried. */
  /* should never reach here, but just in case */
  return (0);
}

/* Init players with some belongings			-RAK-	*/
static void char_inven_init()
{
  register int i, j;
  inven_type inven_init;

  /* this is needed for bash to work right, it can't hurt anyway */
  for (i = 0; i < INVEN_ARRAY_SIZE; i++)
    invcopy(&inventory[i], OBJ_NOTHING);

  for (i = 0; i < 5; i++)
    {
      j = player_init[py.misc.pclass][i];
      invcopy(&inven_init, j);
      /* this makes it known2 and known1 */
      store_bought(&inven_init);
      /* must set this bit to display tohit/todam for stiletto */
      if (inven_init.tval == TV_SWORD)
	inven_init.ident |= ID_SHOW_HITDAM;
      (void) inven_carry(&inven_init);
    }

  /* wierd place for it, but why not? */
  for (i = 0; i < 32; i++)
    spell_order[i] = 99;
}


/* Initializes M_LEVEL array for use with PLACE_MONSTER	-RAK-	*/
static void init_m_level()
{
  register int i, k;

  for (i = 0; i <= MAX_MONS_LEVEL; i++)
    m_level[i] = 0;

  k = MAX_CREATURES - WIN_MON_TOT;
  for (i = 0; i < k; i++)
    m_level[c_list[i].level]++;

  for (i = 1; i <= MAX_MONS_LEVEL; i++)
    m_level[i] += m_level[i-1];
}


/* Initializes T_LEVEL array for use with PLACE_OBJECT	-RAK-	*/
static void init_t_level()
{
  register int i, l;
  int tmp[MAX_OBJ_LEVEL+1];

  for (i = 0; i <= MAX_OBJ_LEVEL; i++)
    t_level[i] = 0;
  for (i = 0; i < MAX_DUNGEON_OBJ; i++)
    t_level[object_list[i].level]++;
  for (i = 1; i <= MAX_OBJ_LEVEL; i++)
    t_level[i] += t_level[i-1];

  /* now produce an array with object indexes sorted by level, by using
     the info in t_level, this is an O(n) sort! */
  /* this is not a stable sort, but that does not matter */
  for (i = 0; i <= MAX_OBJ_LEVEL; i++)
    tmp[i] = 1;
  for (i = 0; i < MAX_DUNGEON_OBJ; i++)
    {
      l = object_list[i].level;
      sorted_objects[t_level[l] - tmp[l]] = i;
      tmp[l]++;
    }
}


/* Adjust prices of objects				-RAK-	*/
static void price_adjust()
{
  register int i;

  /* round half-way cases up */
  for (i = 0; i < MAX_OBJECTS; i++)
    object_list[i].cost = ((object_list[i].cost * COST_ADJ) + 50) / 100;
}
