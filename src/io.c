/* source/io.c: terminal I/O code, uses the curses package

   Copyright (c) 1989-92 James E. Wilson, Robert A. Koeneke

   This software may be copied and distributed for educational, research, and
   not for profit purposes provided that this copyright and statement are
   included in all such copies. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#ifndef STDIO_LOADED
#define STDIO_LOADED
#endif

#include "config.h"
#include "constant.h"
#include "types.h"
#include "externs.h"

#include <curses.h>

/* These are included after curses.h to avoid redefintion warnings for
   TRUE, FALSE, and NULL.  */

#include <sys/ioctl.h>
#include <termio.h>
#include <sys/wait.h>

#define use_value
#define use_value2

char *getenv();

#ifndef VMS
#ifdef USG
void exit();
#if defined(__TURBOC__)
void sleep();
#else
#ifndef AMIGA
unsigned sleep();
#endif
#endif
#endif
#endif

#if !defined(MAC) && !defined(MSDOS) && !defined(ATARI_ST) && !defined(VMS)
#ifndef AMIGA
#ifdef USG
static struct termio save_termio;
#else
static struct ltchars save_special_chars;
static struct sgttyb save_ttyb;
static struct tchars save_tchars;
static int save_local_chars;
#endif
#endif
#endif

static int curses_on = FALSE;
static WINDOW *savescr;		/* Spare window for saving the screen. -CJS-*/

/* suspend()							   -CJS-
   Handle the stop and start signals. This ensures that the log
   is up to date, and that the terminal is fully reset and
   restored.  */
int suspend()
{
  struct sgttyb tbuf;
  struct ltchars lcbuf;
  struct tchars cbuf;
  int lbuf;
  long time();

  py.misc.male |= 2;
  (void) ioctl(0, TIOCGETP, (char *)&tbuf);
  (void) ioctl(0, TIOCGETC, (char *)&cbuf);
  (void) ioctl(0, TIOCGLTC, (char *)&lcbuf);
  restore_term();
  (void) kill(0, SIGSTOP);
  curses_on = TRUE;
  (void) ioctl(0, TIOCSETP, (char *)&tbuf);
  (void) ioctl(0, TIOCSETC, (char *)&cbuf);
  (void) ioctl(0, TIOCSLTC, (char *)&lcbuf);
  (void) wrefresh(curscr);
  py.misc.male &= ~2;
  return 0;
}

/* initializes curses routines */
void init_curses()
#ifdef MAC
{
  /* Primary initialization is done in mac.c since game is restartable */
  /* Only need to clear the screen here */
  Rect scrn;

  scrn.left = scrn.top = 0;
  scrn.right = SCRN_COLS;
  scrn.bottom = SCRN_ROWS;
  EraseScreen(&scrn);
  UpdateScreen();
}
#else
{
  int i, y, x;

#ifdef AMIGA
  if (opentimer() == 0)
    {
      (void) printf ("Could not open timer device.\n");
      exit (1);
    }
#endif

#ifndef USG
  (void) ioctl(0, TIOCGLTC, (char *)&save_special_chars);
  (void) ioctl(0, TIOCGETP, (char *)&save_ttyb);
  (void) ioctl(0, TIOCGETC, (char *)&save_tchars);
#if !defined(atarist) && !defined(__GNUC__)
  (void) ioctl(0, TIOCLGET, (char *)&save_local_chars);
#endif
#else
#if !defined(VMS) && !defined(MSDOS) && !defined(ATARI_ST)
#ifndef AMIGA
  (void) ioctl(0, TCGETA, (char *)&save_termio);
#endif
#endif
#endif

  /* PC curses returns ERR */
#if defined(USG) && !defined(PC_CURSES) && !defined(AMIGA)
  if (initscr() == NULL)
#else
  if (initscr() == ERR)
#endif
    {
      (void) printf("Error allocating screen in curses package.\n");
      exit(1);
    }
  if (LINES < 24 || COLS < 80)	 /* Check we have enough screen. -CJS- */
    {
      (void) printf("Screen too small for moria.\n");
      exit (1);
    }
#ifdef SIGTSTP
#if defined(atarist) && defined(__GNUC__)
  (void) signal (SIGTSTP, (__Sigfunc)suspend);
#else
  (void) signal (SIGTSTP, suspend);
#endif
#endif
  if (((savescr = newwin (0, 0, 0, 0)) == NULL)
#ifdef VMS
      || ((tempscr = newwin (0, 0, 0, 0)) == NULL))
#else
    )
#endif
    {
      (void) printf ("Out of memory in starting up curses.\n");
      exit_game();
    }
  (void) clear();
  (void) refresh();
  moriaterm ();

#if 0
  /* This assumes that the terminal is 80 characters wide, which is not
     guaranteed to be true.  */

  /* check tab settings, exit with error if they are not 8 spaces apart */
  (void) move(0, 0);
  for (i = 1; i < 10; i++)
    {
      (void) addch('\t');
      getyx(stdscr, y, x);
      if (y != 0 || x != i*8)
	break;
    }
  if (i != 10)
    {
      msg_print("Tabs must be set 8 spaces apart.");
      exit_game();
    }
#endif
}
#endif

/* Set up the terminal into a suitable state for moria.	 -CJS- */
void moriaterm()
#ifdef MAC
/* Nothing to do on Mac */
{
}
#else
{
#if !defined(MSDOS) && !defined(ATARI_ST) && !defined(VMS)
#ifndef AMIGA
#ifdef USG
  struct termio tbuf;
#else
  struct ltchars lbuf;
  struct tchars buf;
#endif
#endif
#endif

  curses_on = TRUE;
#ifndef BSD4_3
  use_value crmode();
#else
#ifdef VMS
  use_value vms_crmode ();
#else
  use_value cbreak();
#endif
#endif
  use_value noecho();
  /* can not use nonl(), because some curses do not handle it correctly */
#ifdef MSDOS
  msdos_raw();
#else
#ifdef AMIGA
  init_color (0,   0,   0,   0);	/* pen 0 - black */
  init_color (1,1000,1000,1000);	/* pen 1 - white */
  init_color (2,   0, 300, 700);	/* pen 2 - blue */
  init_color (3,1000, 500,   0);	/* pen 3 - orange */
#else
#if !defined(ATARI_ST) && !defined(VMS)
#ifdef USG
  (void) ioctl(0, TCGETA, (char *)&tbuf);
  /* disable all of the normal special control characters */
  tbuf.c_cc[VINTR] = (char)3; /* control-C */
  tbuf.c_cc[VQUIT] = (char)-1;
  tbuf.c_cc[VERASE] = (char)-1;
  tbuf.c_cc[VKILL] = (char)-1;
  tbuf.c_cc[VEOF] = (char)-1;

  /* don't know what these are for */
  tbuf.c_cc[VEOL] = (char)-1;
#ifdef VEOL2
  tbuf.c_cc[VEOL2] = (char)-1;
#endif

  /* stuff needed when !icanon, i.e. cbreak/raw mode */
  tbuf.c_cc[VMIN] = 1;  /* Input should wait for at least 1 char */
  tbuf.c_cc[VTIME] = 0; /* no matter how long that takes. */

  (void) ioctl(0, TCSETA, (char *)&tbuf);
#else
  /* disable all of the special characters except the suspend char, interrupt
     char, and the control flow start/stop characters */
  (void) ioctl(0, TIOCGLTC, (char *)&lbuf);
  lbuf.t_suspc = (char)26; /* control-Z */
  lbuf.t_dsuspc = (char)-1;
  lbuf.t_rprntc = (char)-1;
  lbuf.t_flushc = (char)-1;
  lbuf.t_werasc = (char)-1;
  lbuf.t_lnextc = (char)-1;
  (void) ioctl(0, TIOCSLTC, (char *)&lbuf);

  (void) ioctl (0, TIOCGETC, (char *)&buf);
  buf.t_intrc = (char)3; /* control-C */
  buf.t_quitc = (char)-1;
  buf.t_startc = (char)17; /* control-Q */
  buf.t_stopc = (char)19; /* control-S */
  buf.t_eofc = (char)-1;
  buf.t_brkc = (char)-1;
  (void) ioctl(0, TIOCSETC, (char *)&buf);
#endif
#endif
#endif
#endif

}
#endif


/* Dump IO to buffer					-RAK-	*/
void put_buffer(out_str, row, col)
char *out_str;
int row, col;
#ifdef MAC
{
  /* The screen manager handles writes past the edge ok */
  DSetScreenCursor(col, row);
  DWriteScreenStringAttr(out_str, ATTR_NORMAL);
}
#else
{
  vtype tmp_str;

  /* truncate the string, to make sure that it won't go past right edge of
     screen */
  if (col > 79)
    col = 79;
  (void) strncpy (tmp_str, out_str, 79 - col);
  tmp_str [79 - col] = '\0';

  if (mvaddstr(row, col, tmp_str) == ERR)
    {
      abort();
      /* clear msg_flag to avoid problems with unflushed messages */
      msg_flag = 0;
      (void) sprintf(tmp_str, "error in put_buffer, row = %d col = %d\n",
		     row, col);
      prt(tmp_str, 0, 0);
      bell();
      /* wait so user can see error */
      (void) sleep(2);
    }
}
#endif


/* Dump the IO buffer to terminal			-RAK-	*/
void put_qio()
{
  screen_change = TRUE;	   /* Let inven_command know something has changed. */
#ifdef MAC
  UpdateScreen();
#else
  (void) refresh();
#endif
}

/* Put the terminal in the original mode.			   -CJS- */
void restore_term()
#ifdef MAC
/* Nothing to do on Mac */
{
}
#else
{
#ifdef AMIGA
  closetimer ();
#endif

  if (!curses_on)
    return;
  put_qio();  /* Dump any remaining buffer */
#ifdef MSDOS
  (void) sleep(2);   /* And let it be read. */
#endif
#ifdef VMS
  clear_screen();
  pause_line(15);
#endif
  /* this moves curses to bottom right corner */
  mvcur(stdscr->_cury, stdscr->_curx, LINES-1, 0);
  endwin();  /* exit curses */
  (void) fflush (stdout);
#ifdef MSDOS
  msdos_noraw();
#endif
  /* restore the saved values of the special chars */
#ifdef USG
#if !defined(MSDOS) && !defined(ATARI_ST) && !defined(VMS)
#ifndef AMIGA
  (void) ioctl(0, TCSETA, (char *)&save_termio);
#endif
#endif
#else
  (void) ioctl(0, TIOCSLTC, (char *)&save_special_chars);
  (void) ioctl(0, TIOCSETP, (char *)&save_ttyb);
  (void) ioctl(0, TIOCSETC, (char *)&save_tchars);
#if !defined(atarist) && !defined(__GNUC__)
  (void) ioctl(0, TIOCLSET, (char *)&save_local_chars);
#endif
#endif
  curses_on = FALSE;
}
#endif


void shell_out()
#if defined(atarist) && defined(__GNUC__)
{ char fail_message[80], arg_list[1], *p;
  int  escape_code;

  save_screen();
  clear_screen();
  use_value nocbreak();         /* Must remember to reset terminal modes   */
  use_value echo();             /* or shell i/o will be quite messed up!   */

  p = (char *)getenv("SHELL");
  if (p != (char *)NULL)
    { put_buffer("Escaping to Shell\n",0,0);
      put_qio();
      arg_list[0]=0;
      escape_code = Pexec(0,p,arg_list,0);   /* Launch the shell.          */

      if (escape_code != 0)
         { sprintf(fail_message,"Pexec() error code = %d\n",escape_code);
           put_buffer(fail_message,0,0);
           put_qio();
	   sleep(5);
	 }
    }
  use_value cbreak();        /* Reset the terminal back to CBREAK/NOECHO   */
  use_value noecho();
  clear_screen();            /* Do not want shell data on screen.          */
  restore_screen();
}

#else

#ifdef MAC
{
  alert_error("This command is not implemented on the Macintosh.");
}
#else
#if defined(AMIGA) || defined(ATARIST_TC)
{
  put_buffer("This command is not implemented.\n", 0, 0);
}
#else
#ifdef VMS /* TPP */
{
  int val, istat;
  char *str;

  save_screen();
  /* clear screen and print 'exit' message */
  clear_screen();
  put_buffer("[Entering subprocess, type 'EOJ' to resume your game.]\n",
	     0, 0);
  put_qio();

  use_value vms_nocrmode();
  use_value echo();
  ignore_signals();

  istat = lib$spawn();
  if (!istat)
    lib$signal (istat);

  restore_signals();
  use_value vms_crmode();
  use_value noecho();
  /* restore the cave to the screen */
  restore_screen();
  put_buffer("Welcome back to UMoria.\n", 0, 0);
  save_screen();
  clear_screen();
  put_qio();
  restore_screen();
  (void) wrefresh(curscr);
}
#else
{
#ifdef USG
#if !defined(MSDOS) && !defined(ATARI_ST) && !defined(AMIGA)
  struct termio tbuf;
#endif
#else
  struct sgttyb tbuf;
  struct ltchars lcbuf;
  struct tchars cbuf;
  int lbuf;

  /* To remove compiler complaints: TODO: Investigate. maybe remove */
  (void)tbuf;
  (void)lcbuf;
  (void)cbuf;

#endif
#ifdef MSDOS
  char	*comspec, key;
#else
#ifdef ATARI_ST
  char comstr[80];
  char *str;
  extern char **environ;
#else
  int val;
  char *str;
#endif
#endif

  save_screen();
  /* clear screen and print 'exit' message */
  clear_screen();
#ifndef ATARI_ST
  put_buffer("[Entering shell, type 'exit' to resume your game.]\n",0,0);
#else
  put_buffer("[Escaping to shell]\n",0,0);
#endif
  put_qio();

#ifdef USG
#if !defined(MSDOS) && !defined(ATARI_ST) && !defined(AMIGA)
  (void) ioctl(0, TCGETA, (char *)&tbuf);
#endif
#else
  (void) ioctl(0, TIOCGETP, (char *)&tbuf);
  (void) ioctl(0, TIOCGETC, (char *)&cbuf);
  (void) ioctl(0, TIOCGLTC, (char *)&lcbuf);
  (void) ioctl(0, TIOCLGET, (char *)&lbuf);
#endif
  /* would call nl() here if could use nl()/nonl(), see moriaterm() */
#ifndef BSD4_3
  use_value nocrmode();
#else
#ifdef VMS
  use_value vms_nocrmode ();
#else
  use_value nocbreak();
#endif
#endif
#ifdef MSDOS
  use_value msdos_noraw();
#endif
  use_value echo();
  ignore_signals();
#ifdef MSDOS		/*{*/
  if ((comspec = getenv("COMSPEC")) == CNIL
  ||  spawnl(P_WAIT, comspec, comspec, CNIL) < 0) {
	clear_screen();	/* BOSS key if shell failed */
	put_buffer("M:\\> ", 0, 0);
	do {
	  key = inkey();
	} while (key != '!');
  }

#else		/* MSDOS }{*/
#ifndef ATARI_ST
  val = fork();
  if (val == 0)
    {
#endif
      default_signals();
#ifdef USG
#if !defined(MSDOS) && !defined(ATARI_ST) && !defined(AMIGA)
      (void) ioctl(0, TCSETA, (char *)&save_termio);
#endif
#else
      (void) ioctl(0, TIOCSLTC, (char *)&save_special_chars);
      (void) ioctl(0, TIOCSETP, (char *)&save_ttyb);
      (void) ioctl(0, TIOCSETC, (char *)&save_tchars);
      (void) ioctl(0, TIOCLSET, (char *)&save_local_chars);
#endif
#ifndef MSDOS
      /* close scoreboard descriptor */
      /* it is not open on MSDOS machines */
      (void) fclose(highscore_fp);
#endif
      if ((str = getenv("SHELL")))
#ifndef ATARI_ST
	(void) execl(str, str, (char *) 0);
#else
	system(str);
#endif
      else
#ifndef ATARI_ST
	(void) execl("/bin/sh", "sh", (char *) 0);
#endif
      msg_print("Cannot execute shell.");
#ifndef ATARI_ST
      exit(1);
    }
  if (val == -1)
    {
      msg_print("Fork failed. Try again.");
      return;
    }
#ifdef USG
  (void) wait((int *) 0);
#else
  (void) wait((union wait *) 0);
#endif
#endif /* ATARI_ST */
#endif		 /* MSDOS }*/
  restore_signals();
  /* restore the cave to the screen */
  restore_screen();
#ifndef BSD4_3
  use_value crmode();
#else
#ifdef VMS
  use_value vms_crmode ();
#else
  use_value cbreak();
#endif
#endif
  use_value noecho();
  /* would call nonl() here if could use nl()/nonl(), see moriaterm() */
#ifdef MSDOS
  msdos_raw();
#endif
  /* disable all of the local special characters except the suspend char */
  /* have to disable ^Y for tunneling */
#ifdef USG
#if !defined(MSDOS) && !defined(ATARI_ST)
  (void) ioctl(0, TCSETA, (char *)&tbuf);
#endif
#else
  (void) ioctl(0, TIOCSLTC, (char *)&lcbuf);
  (void) ioctl(0, TIOCSETP, (char *)&tbuf);
  (void) ioctl(0, TIOCSETC, (char *)&cbuf);
  (void) ioctl(0, TIOCLSET, (char *)&lbuf);
#endif
  (void) wrefresh(curscr);
}
#endif
#endif
#endif
#endif

/* Returns a single character input from the terminal.	This silently -CJS-
   consumes ^R to redraw the screen and reset the terminal, so that this
   operation can always be performed at any input prompt.  inkey() never
   returns ^R.	*/
char inkey()
{
  int i;

  put_qio();			/* Dump IO buffer		*/
  command_count = 0;  /* Just to be safe -CJS- */
  while (TRUE)
    {
      i = getch();

      /* some machines may not sign extend. */
      if (i == EOF)
	{
	  eof_flag++;
	  /* avoid infinite loops while trying to call inkey() for a -more-
	     prompt. */
	  msg_flag = FALSE;

	  (void) refresh ();
	  if (!character_generated || character_saved)
	    exit_game();
	  disturb(1, 0);
	  if (eof_flag > 100)
	    {
	      /* just in case, to make sure that the process eventually dies */
	      panic_save = 1;
	      (void) strcpy(died_from, "(end of input: panic saved)");
	      if (!save_char())
		{
		  (void) strcpy(died_from, "panic: unexpected eof");
		  death = TRUE;
		}
	      exit_game();
	    }
	  return ESCAPE;
	}
      if (i != CTRL('R'))
	return (char)i;
      (void) wrefresh (curscr);
      moriaterm();
    }
}


/* Flush the buffer					-RAK-	*/
void flush()
{
  /* the code originally used ioctls, TIOCDRAIN, or TIOCGETP/TIOCSETP, or
     TCGETA/TCSETAF, however this occasionally resulted in loss of output,
     the happened especially often when rlogin from BSD to SYS_V machine,
     using check_input makes the desired effect a bit clearer */
  /* wierd things happen on EOF, don't try to flush input in that case */
  if (!eof_flag)
    while (check_input(0));

  /* used to call put_qio() here to drain output, but it is not necessary */
}


/* Clears given line of text				-RAK-	*/
void erase_line(row, col)
int row;
int col;
{
  if (row == MSG_LINE && msg_flag)
    msg_print(CNIL);
  (void) move(row, col);
  clrtoeol();
}


/* Clears screen */
void clear_screen()
{
  if (msg_flag)
    msg_print(CNIL);
  (void) clear();
}

void clear_from (row)
int row;
{
  (void) move(row, 0);
  clrtobot();
}


/* Outputs a char to a given interpolated y, x position	-RAK-	*/
/* sign bit of a character used to indicate standout mode. -CJS */
void print(ch, row, col)
char ch;
int row;
int col;
{
  vtype tmp_str;

  row -= panel_row_prt;/* Real co-ords convert to screen positions */
  col -= panel_col_prt;
  if (mvaddch (row, col, ch) == ERR)
    {
      abort();
      /* clear msg_flag to avoid problems with unflushed messages */
      msg_flag = 0;
      (void) sprintf(tmp_str, "error in print, row = %d col = %d\n",
		     row, col);
      prt(tmp_str, 0, 0);
      bell ();
      /* wait so user can see error */
      (void) sleep(2);
    }
}


/* Moves the cursor to a given interpolated y, x position	-RAK-	*/
void move_cursor_relative(row, col)
int row;
int col;
{
  vtype tmp_str;

  row -= panel_row_prt;/* Real co-ords convert to screen positions */
  col -= panel_col_prt;
  if (move (row, col) == ERR)
    {
      abort();
      /* clear msg_flag to avoid problems with unflushed messages */
      msg_flag = 0;
      (void) sprintf(tmp_str,
		     "error in move_cursor_relative, row = %d col = %d\n",
		     row, col);
      prt(tmp_str, 0, 0);
      bell();
      /* wait so user can see error */
      (void) sleep(2);
    }
}


/* Print a message so as not to interrupt a counted command. -CJS- */
void count_msg_print(p)
char *p;
{
  int i;

  i = command_count;
  msg_print(p);
  command_count = i;
}


/* Outputs a line to a given y, x position		-RAK-	*/
void prt(str_buff, row, col)
char *str_buff;
int row;
int col;
{
  if (row == MSG_LINE && msg_flag)
    msg_print(CNIL);
  (void) move(row, col);
  clrtoeol();
  put_buffer(str_buff, row, col);
}


/* move cursor to a given y, x position */
void move_cursor(row, col)
int row, col;
{
  (void) move (row, col);
}


/* Outputs message to top line of screen				*/
/* These messages are kept for later reference.	 */
void msg_print(str_buff)
char *str_buff;
{
  register int old_len, new_len;
  int combine_messages = FALSE;
  char in_char;

  if (msg_flag)
    {
      old_len = strlen(old_msg[last_msg]) + 1;

      /* If the new message and the old message are short enough, we want
	 display them together on the same line.  So we don't flush the old
	 message in this case.  */
	 
      if (str_buff)
	new_len = strlen (str_buff);
      else
	new_len = 0;

      if (! str_buff || (new_len + old_len + 2 >= 73))
	{
	  /* ensure that the complete -more- message is visible. */
	  if (old_len > 73)
	    old_len = 73;
	  put_buffer(" -more-", MSG_LINE, old_len);
	  /* let sigint handler know that we are waiting for a space */
	  wait_for_more = 1;
	  do
	    {
	      in_char = inkey();
	    }
	  while ((in_char != ' ') && (in_char != ESCAPE) && (in_char != '\n')
		 && (in_char != '\r'));
	  wait_for_more = 0;
	}
      else
	combine_messages = TRUE;
    }

  if (! combine_messages)
    {
      (void) move(MSG_LINE, 0);
      clrtoeol();
    }

  /* Make the null string a special case.  -CJS- */
  if (str_buff)
    {
      command_count = 0;
      msg_flag = TRUE;

      /* If the new message and the old message are short enough, display
	 them on the same line.  */
      
      if (combine_messages)
	{
	  put_buffer (str_buff, MSG_LINE, old_len + 2);
	  strcat (old_msg[last_msg], "  ");
	  strcat (old_msg[last_msg], str_buff);
	}
      else
	{
	  put_buffer(str_buff, MSG_LINE, 0);
	  last_msg++;
	  if (last_msg >= MAX_SAVE_MSG)
	    last_msg = 0;
	  (void) strncpy(old_msg[last_msg], str_buff, VTYPESIZ);
	  old_msg[last_msg][VTYPESIZ - 1] = '\0';
	}
    }
  else
    msg_flag = FALSE;
}


/* Used to verify a choice - user gets the chance to abort choice.  -CJS- */
int get_check(prompt)
char *prompt;
{
  int res;
  int y, x;

  prt(prompt, 0, 0);
  getyx(stdscr, y, x);

  (void)y; /* To remove compiler complaints */

  if (x > 73)
    (void) move(0, 73);
  (void) addstr(" [y/n]");
  do
    {
      res = inkey();
    }
  while(res == ' ');
  erase_line(0, 0);
  if (res == 'Y' || res == 'y')
    return TRUE;
  else
    return FALSE;
}

/* Prompts (optional) and returns ord value of input char	*/
/* Function returns false if <ESCAPE> is input	*/
int get_com(prompt, command)
char *prompt;
char *command;
{
  int res;

  if (prompt)
    prt(prompt, 0, 0);
  *command = inkey();
  if (*command == ESCAPE)
    res = FALSE;
  else
    res = TRUE;
  erase_line(MSG_LINE, 0);
  return(res);
}

/* Gets a string terminated by <RETURN>		*/
/* Function returns false if <ESCAPE> is input	*/
int get_string(in_str, row, column, slen)
char *in_str;
int row, column, slen;
{
  register int start_col, end_col, i;
  char *p;
  int flag, aborted;

  aborted = FALSE;
  flag	= FALSE;
  (void) move(row, column);
  for (i = slen; i > 0; i--)
    (void) addch(' ');
  (void) move(row, column);
  start_col = column;
  end_col = column + slen - 1;
  if (end_col > 79)
    {
      slen = 80 - column;
      end_col = 79;
    }
  p = in_str;
  do
    {
      i = inkey();
      switch(i)
	{
	case ESCAPE:
	  aborted = TRUE;
	  break;
	case CTRL('J'): case CTRL('M'):
	  flag	= TRUE;
	  break;
	case DELETE: case CTRL('H'):
	  if (column > start_col)
	    {
	      column--;
	      put_buffer(" ", row, column);
	      move_cursor(row, column);
	      *--p = '\0';
	    }
	  break;
	default:
	  if (!isprint(i) || column > end_col)
	    bell();
	  else
	    {
	      use_value2 mvaddch(row, column, (char)i);
	      *p++ = i;
	      column++;
	    }
	  break;
	}
    }
  while ((!flag) && (!aborted));
  if (aborted)
    return(FALSE);
  /* Remove trailing blanks	*/
  while (p > in_str && p[-1] == ' ')
    p--;
  *p = '\0';
  return(TRUE);
}


/* Pauses for user response before returning		-RAK-	*/
void pause_line(prt_line)
int prt_line;
{
  prt("[Press any key to continue.]", prt_line, 23);
  (void) inkey();
  erase_line(prt_line, 0);
}


/* Pauses for user response before returning		-RAK-	*/
/* NOTE: Delay is for players trying to roll up "perfect"	*/
/*	characters.  Make them wait a bit.			*/
void pause_exit(prt_line, delay)
int prt_line;
int delay;
{
  char dummy;

  prt("[Press any key to continue, or Q to exit.]", prt_line, 10);
  dummy = inkey();
  if (dummy == 'Q')
    {
      erase_line(prt_line, 0);
      if (delay > 0)  (void) sleep((unsigned)delay);
      exit_game();
    }
  erase_line(prt_line, 0);
}

void save_screen()
{
  overwrite(stdscr, savescr);
}

void restore_screen()
{
  overwrite(savescr, stdscr);
  touchwin(stdscr);
}

void bell()
{
  put_qio();

  /* The player can turn off beeps if he/she finds them annoying.  */
  if (! sound_beep_flag)
    return;

  (void) write(1, "\007", 1);
}

/* definitions used by screen_map() */
/* index into border character array */
#define TL 0	/* top left */
#define TR 1
#define BL 2
#define BR 3
#define HE 4	/* horizontal edge */
#define VE 5

/* character set to use */
#define CH(x)	(screen_border[0][x])

  /* Display highest priority object in the RATIO by RATIO area */
#define RATIO 3

void screen_map()
{
  register int	i, j;
  static int8u screen_border[2][6] = {
    {'+', '+', '+', '+', '-', '|'},	/* normal chars */
    {201, 187, 200, 188, 205, 186}	/* graphics chars */
  };
  int8u map[MAX_WIDTH / RATIO + 1];
  int8u tmp;
  int priority[256];
  int row, orow, col, myrow, mycol = 0;
  char prntscrnbuf[80];

  for (i = 0; i < 256; i++)
    priority[i] = 0;
  priority['<'] = 5;
  priority['>'] = 5;
  priority['@'] = 10;
  priority['#'] = -5;
  priority['.'] = -10;
  priority['\''] = -3;
  priority[' '] = -15;

  save_screen();
  clear_screen();
  use_value2 mvaddch(0, 0, CH(TL));
  for (i = 0; i < MAX_WIDTH / RATIO; i++)
    (void) addch(CH(HE));
  (void) addch(CH(TR));
  orow = -1;
  map[MAX_WIDTH / RATIO] = '\0';
  for (i = 0; i < MAX_HEIGHT; i++)
    {
      row = i / RATIO;
      if (row != orow)
	{
	  if (orow >= 0)
	    {
	      /* can not use mvprintw() on ibmpc, because PC-Curses is horribly
		 written, and mvprintw() causes the fp emulation library to be
		 linked with PC-Moria, makes the program 10K bigger */
	      (void) sprintf(prntscrnbuf,"%c%s%c",CH(VE), map, CH(VE));
	      use_value2 mvaddstr(orow+1, 0, prntscrnbuf);
	    }
	  for (j = 0; j < MAX_WIDTH / RATIO; j++)
	    map[j] = ' ';
	  orow = row;
	}
      for (j = 0; j < MAX_WIDTH; j++)
	{
	  col = j / RATIO;
	  tmp = loc_symbol(i, j);
	  if (priority[map[col]] < priority[tmp])
	    map[col] = tmp;
	  if (map[col] == '@')
	    {
	      mycol = col + 1; /* account for border */
	      myrow = row + 1;
	    }
	}
    }
  if (orow >= 0)
    {
      (void) sprintf(prntscrnbuf,"%c%s%c",CH(VE), map, CH(VE));
      use_value2 mvaddstr(orow+1, 0, prntscrnbuf);
    }
  use_value2 mvaddch(orow + 2, 0, CH(BL));
  for (i = 0; i < MAX_WIDTH / RATIO; i++)
    (void) addch(CH(HE));
  (void) addch(CH(BR));

  use_value2 mvaddstr(23, 23, "Hit any key to continue");
  if (mycol > 0)
    (void) move(myrow, mycol);
  (void) inkey();
  restore_screen();
}
