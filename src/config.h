/* source/config.h: configuration definitions

   Copyright (c) 1989-92 James E. Wilson

   This software may be copied and distributed for educational, research, and
   not for profit purposes provided that this copyright and statement are
   included in all such copies. */

#define CONFIG_H_INCLUDED

/* Person to bother if something goes wrong.  */
/* Recompile files.c and misc2.c if this changes.  */
#define WIZARD	"David Grabiner"

#ifdef M_XENIX
#define SYS_V
#define unix
#endif

#define MORIA_SAV	"moria.save"
#define MORIA_HOU	"/opt/share/moria/files/hours"
#define MORIA_MOR	"/opt/share/moria/files/news"
#define MORIA_TOP	"/opt/share/moria/files/scores"
#define MORIA_HELP	"/opt/share/moria/files/roglcmds.hlp"
#define MORIA_ORIG_HELP	"/opt/share/moria/files/origcmds.hlp"
#define MORIA_WIZ_HELP	"/opt/share/moria/files/rwizcmds.hlp"
#define MORIA_OWIZ_HELP	"/opt/share/moria/files/owizcmds.hlp"
#define MORIA_WELCOME	"/opt/share/moria/files/welcome.hlp"
#define MORIA_VER	"/opt/share/moria/files/version.hlp"

/* Substitute strchr for index on USG versions of UNIX.  */
#if defined(SYS_V) || defined(MSDOS) || defined(MAC) || defined(VMS)
#define index strchr
#endif

/* Define USG for many systems, this is basically to select SYS V style
   system calls (as opposed to BSD style).  */
#if defined(SYS_III) || defined(SYS_V) || defined(MSDOS) || defined(MAC)
#ifndef USG
#define USG
#endif
#endif

#ifdef SECURE
extern int PlayerUID;
#define getuid() PlayerUID
#define geteuid() PlayerUID
#endif
