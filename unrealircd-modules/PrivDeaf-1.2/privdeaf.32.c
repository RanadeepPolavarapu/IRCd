/*
 * usermode +D: makes it so you cannot receive private messages/notices
 * except from opers, U-lines and servers. -- Syzop
 */

#include "config.h"
#include "struct.h"
#include "common.h"
#include "sys.h"
#include "numeric.h"
#include "msg.h"
#include "channel.h"
#include <time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <io.h>
#endif
#include <fcntl.h>
#include "h.h"
#ifdef STRIPBADWORDS
#include "badwords.h"
#endif
#ifdef _WIN32
#include "version.h"
#endif

ModuleHeader MOD_HEADER(privdeaf)
  = {
	"privdeaf",
	"v1.2", 
	"Private Messages Deaf (+D) -- by Syzop",
	"3.2-b8-1",
	NULL 
    };

static long UMODE_PRIVDEAF = 0;
static Umode *UmodePrivdeaf = NULL;

DLLFUNC char *privdeaf_checkmsg(aClient *, aClient *, aClient *, char *, int);

DLLFUNC int MOD_INIT(privdeaf)(ModuleInfo *modinfo)
{
	UmodePrivdeaf = UmodeAdd(modinfo->handle, 'D', UMODE_GLOBAL, umode_allow_all, &UMODE_PRIVDEAF);
	if (!UmodePrivdeaf)
	{
		/* I use config_error() here because it's printed to stderr in case of a load
		 * on cmd line, and to all opers in case of a /rehash.
		 */
		config_error("privdeaf: Could not add usermode 'D': %s", ModuleGetErrorStr(modinfo->handle));
		return MOD_FAILED;
	}
	
	 HookAddPCharEx(modinfo->handle, HOOKTYPE_USERMSG, privdeaf_checkmsg);

	/* Ah well.. we'll just go perm for now. */
	ModuleSetOptions(modinfo->handle, MOD_OPT_PERM);

	return MOD_SUCCESS;
}

DLLFUNC int MOD_LOAD(privdeaf)(int module_load)
{
	return MOD_SUCCESS;
}

DLLFUNC int MOD_UNLOAD(privdeaf)(int module_unload)
{
	return MOD_SUCCESS;
}

DLLFUNC char *privdeaf_checkmsg(aClient *cptr, aClient *sptr, aClient *acptr, char *text, int notice)
{
	if ((acptr->umodes & UMODE_PRIVDEAF) && !IsAnOper(sptr) &&
	    !IsULine(sptr) && !IsServer(sptr) && (sptr != acptr))
	{
		sendnotice(sptr, "Message to '%s' not delivered: User does not accept private messages", acptr->name);
		return NULL;
	} else
		return text;
}
