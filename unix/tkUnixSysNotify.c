/*
 * tkUnixSysNotify.c --
 *
 * 	tkUnixSysNotify.c implements a "sysnotify" Tcl command which
 * 	permits one to post system notifications based on the libnotify API.
 *
 * Copyright © 2020 Kevin Walzer/WordTech Communications LLC.
 *
 * See the file "license.terms" for information on usage and redistribution of
 * this file, and for a DISCLAIMER OF ALL WARRANTIES.
 */

#include "tkInt.h"
#include "tkUnixInt.h"

#ifdef HAVE_LIBNOTIFY

#include <libnotify/notify.h>

/*
 * Forward declarations for procedures defined in this file.
 */

static void SysNotifyDeleteCmd (void *);
static int SysNotifyCmd(void *, Tcl_Interp *, int, Tcl_Obj * const*);
int SysNotify_Init(Tcl_Interp *);

/*
 *----------------------------------------------------------------------
 *
 * SysNotifyDeleteCmd --
 *
 *      Delete notification and clean up.
 *
 * Results:
 *	Window destroyed.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static void SysNotifyDeleteCmd (
    TCL_UNUSED(void *))
{
    notify_uninit();
}

/*
 *----------------------------------------------------------------------
 *
 * SysNotifyCreateCmd --
 *
 *      Create tray command and (unreal) window.
 *
 * Results:
 *	Icon tray and hidden window created.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static int SysNotifyCmd(
    TCL_UNUSED(void *),
    Tcl_Interp *interp,
    int objc,
    Tcl_Obj *const *objv)
{
    const char *title;
    const char *message;
    const char *icon;
    NotifyNotification *notif;

    if (objc < 3) {
	Tcl_WrongNumArgs(interp, 1, objv, "title  message");
	return TCL_ERROR;
    }

    /*
     * Pass strings to notification, and use a sane platform-specific
     * icon in the alert.
     */

    title = Tcl_GetString(objv[1]);
    message = Tcl_GetString(objv[2]);
    icon = "dialog-information";

    /*
     * Call to notify_init should go here to prevent test suite failure.
     */
    notify_init("Wish");
    notif = notify_notification_new(title, message, icon);
    notify_notification_show(notif, NULL);

    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * SysNotify_Init --
 *
 *      Initialize the command.
 *
 * Results:
 *	Command initialized.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
SysNotify_Init(
    Tcl_Interp *interp)
{
    Tcl_CreateObjCommand(interp, "::tk::sysnotify::_sysnotify", SysNotifyCmd, interp,
	    SysNotifyDeleteCmd);
    return TCL_OK;
}

#endif /* HAVE_LIBNOTIFY */
/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * fill-column: 79
 * coding: utf-8
 * End:
 */
