/*
 * "No Codes", a very simple (but often requested) module written by Syzop.
 * This module will strip messages with bold/underline/reverse if chanmode
 * +S is set, and block them if +c is set.
 */

#include "unrealircd.h"

#define NOCODES_VERSION "v1.2"

ModuleHeader MOD_HEADER(nocodes)
= {
	"nocodes",	/* Name of module */
	NOCODES_VERSION, /* Version */
	"Strip/block bold/underline/reverse - by Syzop", /* Short description of module */
	"3.2-b8-1",
	NULL 
};

char *nocodes_pre_chanmsg(aClient *sptr, aChannel *chptr, char *text, int notice);

MOD_INIT(nocodes)
{
	HookAddPChar(modinfo->handle, HOOKTYPE_PRE_CHANMSG, 0, nocodes_pre_chanmsg);
	return MOD_SUCCESS;
}

MOD_LOAD(m_dummy)
{
	return MOD_SUCCESS;
}

MOD_UNLOAD(m_dummy)
{
	return MOD_SUCCESS;
}

static int has_controlcodes(char *p)
{
	for (; *p; p++)
		if ((*p == '\002') || (*p == '\037') || (*p == '\026'))
			return 1;
	return 0;
}

char *nocodes_pre_chanmsg(aClient *sptr, aChannel *chptr, char *text, int notice)
{
static char retbuf[4096];

	if (has_channel_mode(chptr, 'S'))
	{
		strlcpy(retbuf, StripControlCodes(text), sizeof(retbuf));
		return retbuf;
	} else
	if (has_channel_mode(chptr, 'c'))
	{
		if (has_controlcodes(text))
		{
			sendto_one(sptr, err_str(ERR_CANNOTSENDTOCHAN),
				me.name, sptr->name, chptr->chname,
				"Control codes (bold/underline/reverse) are not permitted in this channel",
				chptr->chname);
			return NULL;
		} else
			return text;
	} else
		return text;
}
