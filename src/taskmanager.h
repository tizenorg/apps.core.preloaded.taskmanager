/*
 * TASKMANAGER
 *
 * Copyright (c) 2000 - 2012 Samsung Electronics Co., Ltd All Rights Reserved.
 *
 * Contact: Jeesun Kim <iamjs.kim@samsung.com>
 *
 * PROPRIETARY/CONFIDENTIAL
 *
 * This software is the confidential and proprietary information of
 * SAMSUNG ELECTRONICS (Confidential Information). You shall not
 * disclose such Confidential Information and shall use it only in
 * accordance with the terms of the license agreement you entered into
 * with SAMSUNG ELECTRONICS.  SAMSUNG make no representations or warranties
 * about the suitability of the software, either express or implied,
 * including but not limited to the implied warranties of merchantability,
 * fitness for a particular purpose, or non-infringement. SAMSUNG shall
 * not be liable for any damages suffered by licensee as a result of
 * using, modifying or distributing this software or its derivatives.
 */



#ifndef __TASKMANAGER_H__
#define __TASKMANAGER_H__

#include <Elementary.h>
#include <Ecore_X.h>
#include <utilX.h>
#include <bundle.h>

#if !defined(PACKAGE)
#define PACKAGE "taskmanager"
#endif

#ifndef PREFIX
#define PREFIX "/opt/apps/org.tizen."PACKAGE
#endif

#if !defined(RESDIR)
#	define RESDIR PREFIX"/res"
#endif

#if !defined(LOCALEDIR)
#define LOCALEDIR RESDIR"/locale"
#endif

#if !defined(EDJDIR)
#define EDJDIR RESDIR"/edje"PACKAGE
#endif

#if !defined(IMAGEDIR)
#	define IMAGEDIR RESDIR"/images/"PACKAGE
#endif

#define EDJ_NAME EDJDIR"/taskmgr.edj"
#define EDJ_THEME EDJDIR"/theme_taskmanager.edj"
#define GRP_TM "task_manager"

#define D_(str) dgettext("sys_string", str)
#define _BUF_MAX	256
#define _EDJ(x)	elm_layout_edje_get(x)

#define POPUP_TIMER 1.0
#define POPUP_TERMINATE_TIMER 1.5

struct appdata {
	Evas *evas;
	Evas_Object *win, *ly, *nv, *gl;

	Evas_Coord root_w, root_h;

	Eina_List *applist[2];
	/* runapp : 0, history: 1 */

	Ecore_Timer *popup_timer;
	Evas_Object *popup_ask;
	Evas_Object *popup_progressbar;

	Ecore_Timer *update_timer;

	double mem_total;

	int mode;
	int ending;
	int endcnt;
};

struct _task_info {
	char *app_name;
	char *pkg_name;
	char *icn_path;
	pid_t pid;
	struct appdata *ad;
	double mem, mem_total;
	double cpu;
	Elm_Object_Item *it;
	int category;
	bundle *b;
	unsigned int oldutime, oldstime;
	struct timeval oldtimev;
};

	/* MODE_KILL_INUSE = MODE_END_INUSE * 2
	 * MODE_KILL_ALL_INUSE = MODE_END_ALL_INUSE * 2
	 */
enum task_mode {
	MODE_NONE = 0,
	MODE_END_INUSE,
	MODE_KILL_INUSE,
	MODE_END_ALL_INUSE,
	MODE_DEL_HISTORY,
	MODE_DEL_ALL_HISTORY,
	MODE_KILL_ALL_INUSE,
};

enum task_status {
	TS_INUSE = 0,
	TS_HISTORY,
	TS_MAX,
};

Evas_Object *load_edj(Evas_Object *parent, const char *file, const char *group);
int _unset_notification_level(Evas_Object *win);
int _set_notification_level(Evas_Object *win, Utilx_Notification_Level level);
Eina_Bool _exit_cb(void *data);

#endif
/* __TASKMANAGER_H___ */
