/*
 * org.tizen.taskmgr
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Flora License, Version 1.1 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://floralicense.org/license/
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */




#include <stdio.h>
#include <unistd.h>
#include <appcore-efl.h>
#include <Elementary.h>
#include <vconf.h>
#include <aul.h>
#include <sysman.h>

#include "taskmanager.h"
#include "_util_log.h"
#include "_util_efl.h"
#include "_logic.h"
#include "_genlist.h"

struct text_part {
	char *part;
	char *msgid;
};

enum {
	IDLELOCK_OFF = 0x0,
	IDLELOCK_ON,
	IDLELOCK_MAX,
};

enum {
	LCD_OFF = 0x0,
	LCD_ON,
	LCD_MAX,
};

static struct text_part main_txt[] = {
};

static void update_ts(Evas_Object *eo, struct text_part *tp, int size)
{
	int i;

	if (eo == NULL || tp == NULL || size < 0)
		return;

	for (i = 0; i < size; i++) {
		if (tp[i].part && tp[i].msgid)
			edje_object_part_text_set(eo,
						  tp[i].part, _(tp[i].msgid));
	}
}

static int _lang_changed(void *data)
{
	struct appdata *ad = data;

	if (ad->ly == NULL)
		return 0;

	update_ts(elm_layout_edje_get(ad->ly),
		  main_txt, sizeof(main_txt) / sizeof(main_txt[0]));

	return 0;
}

int _get_vconf_idlelock(void)
{
	int ret = -1;
	int lock = IDLELOCK_OFF;

	ret = vconf_get_int(VCONFKEY_IDLE_LOCK_STATE, &lock);
	retvm_if(ret < 0, -1, "Failed to get vconf\n");
	_D("idlelock vconf:%d\n", lock);

	return lock == VCONFKEY_IDLE_LOCK ? IDLELOCK_ON : IDLELOCK_OFF;
}

int _get_vconf_lcdstate(void)
{
	int ret = -1;
	int lcd = 0;

	ret = vconf_get_int(VCONFKEY_PM_STATE, &lcd);
	retvm_if(ret < 0, -1, "Failed to get vconf\n");
	_D("lcd vconf:%d\n", lcd);

	return lcd == VCONFKEY_PM_STATE_LCDOFF ? LCD_OFF : LCD_ON;
}

Eina_Bool _exit_cb(void *data)
{
	int lock = IDLELOCK_ON;
	lock = _get_vconf_idlelock();

	_D("lock(%d)\n", lock);
	if(lock == IDLELOCK_OFF){
		_D("normal case\n");
		elm_exit();
	}
	else{
		_D("IDLELOCK is set, taskmnager doesn't exit\n");
		return ECORE_CALLBACK_CANCEL;
	}
	return ECORE_CALLBACK_CANCEL;
}

void _key_grab(struct appdata *ad)
{
	int ret = 0;
	ret = efl_util_grab_key(ad->win, KEY_SELECT, SHARED_GRAB);
	retm_if(ret < 0, "Failed to grab home key\n");
}

int _set_launch_effect(Evas_Object *win)
{
	Ecore_Evas *ee = NULL;
	Ecore_X_Window xwin = 0;
	static Ecore_X_Atom ATOM_WM_WINDOW_ROLE = 0;
	static Ecore_X_Atom ATOM_NET_WM_NAME = 0;
	retvm_if(win == NULL, -1, "[Error] Invalid argument: win is NULL\n");

	ATOM_WM_WINDOW_ROLE = ecore_x_atom_get("WM_WINDOW_ROLE");
	if (!ATOM_WM_WINDOW_ROLE) {
		fprintf(stderr,
			"[App] %s(%d) XInternAtom(WM_WINDOW_ROLE) failed.\n",
			__func__, __LINE__);
	}

	ATOM_NET_WM_NAME = ecore_x_atom_get("_NET_WM_NAME");
	if (!ATOM_NET_WM_NAME) {
		fprintf(stderr,
			"[App] %s(%d) XInternAtom(ATOM_NET_WM_NAME) failed.\n",
			__func__, __LINE__);
	}

	xwin = elm_win_xwindow_get(win);
	ecore_x_window_prop_string_set(xwin, ATOM_WM_WINDOW_ROLE,
								   "TASK_MANAGER");
	ecore_x_window_prop_string_set(xwin, ATOM_NET_WM_NAME, "TASK_MANAGER");

	ee = ecore_evas_object_ecore_evas_get(win);
	ecore_evas_name_class_set(ee, "TASK_MANAGER", "TASK_MANAGER");
	return 0;
}

int _unset_notification_level(Evas_Object *win)
{
	efl_util_netwm_window_type_set(win, EFL_UTIL_WINDOW_TYPE_NORMAL);
	return 0;
}


int _set_notification_level(Evas_Object *win, Efl_Util_Notification_Level level)
{
	efl_util_set_system_notification_level(win, level);
	return 0;
}

void _check_show_state(void)
{
	int lcd = LCD_OFF, idlelock = IDLELOCK_OFF;
	lcd = _get_vconf_lcdstate();
	idlelock = _get_vconf_idlelock();
	if(lcd == LCD_OFF || idlelock == IDLELOCK_ON)
	{
		elm_exit();
	}
}

int app_create(void *data)
{
	Evas_Object *win;
	struct appdata *ad = data;
	int r;

	win = _add_window(PACKAGE);
	retv_if(win == NULL, -1);
	elm_win_indicator_mode_set(win, ELM_WIN_INDICATOR_SHOW);
	ad->win = win;

	_set_launch_effect(win);
//	_set_notification_level(win, UTILX_NOTIFICATION_LEVEL_NORMAL);

	/* init internationalization */
	r = appcore_set_i18n(PACKAGE, LOCALEDIR);
	retvm_if(r < 0, -1, "Failed to set i18n\n");
	_lang_changed(ad);


	elm_theme_extension_add(NULL, EDJ_THEME);

	_app_create(ad);
	_set_itc();
	_set_genlist(ad);

	/* set dead signal listener */
	aul_listen_app_dead_signal(_dead_cb, ad);

	appcore_set_event_callback(APPCORE_EVENT_LANG_CHANGE,
			_lang_changed, ad);

	ecore_idler_add(_create_idler_cb, ad);

	return 0;
}

static int app_terminate(void *data)
{
	return 0;
}

static int app_pause(void *data)
{
	return 0;
}

static int app_resume(void *data)
{
_D("func\n");
	struct appdata *ad = data;

	refresh_app_info(ad);
	if (ad->killall_timer) {
		ecore_timer_del(ad->killall_timer);
		ad->killall_timer = NULL;
	}
	if (ad->exit_timer) {
		ecore_timer_del(ad->exit_timer);
		ad->exit_timer = NULL;
	}

	return 0;
}

static int app_reset(bundle *b, void *data)
{
	struct appdata *ad = data;

	/* appcore measure time example */
	printf("from AUL to %s(): %d msec\n", __func__,
			appcore_measure_time_from("APP_START_TIME"));
	printf("from create to %s(): %d msec\n", __func__,
			appcore_measure_time());

	if (ad->win)
		elm_win_activate(ad->win);
	return 0;
}

int main(int argc, char *argv[])
{
	sysconf_set_mempolicy(OOM_IGNORE);

	struct appdata ad;
	struct appcore_ops ops = {
		.create = app_create,
		.terminate = app_terminate,
		.pause = app_pause,
		.resume = app_resume,
		.reset = app_reset,
	};

	/* appcore measure time example */
	_D("from AUL to %s(): %d msec\n", __func__,
	       appcore_measure_time_from("APP_START_TIME"));

	memset(&ad, 0x0, sizeof(struct appdata));
	ops.data = &ad;

	return appcore_efl_main(PACKAGE, &argc, &argv, &ops);
}
