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



#include <stdio.h>

#include <appcore-common.h>

#include "taskmanager.h"
#include "_util_log.h"
#include "_util_efl.h"

static void ctxpopup_clear(void *data)
{
	struct appdata *ad = data;

	retm_if(data == NULL, "Invalid argument: appdata is NULL\n");

	if (ad->info_timer) {
		ecore_timer_del(ad->info_timer);
		ad->info_timer = NULL;
	}

	if (ad->info_ctxpopup) {
		evas_object_del(ad->info_ctxpopup);
		ad->info_ctxpopup = NULL;
	}

}

static void label_resize_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	Evas_Object *ly = (Evas_Object *)data;
	Evas_Coord w, h;
	Evas_Coord pw, ph;

	retm_if(ly == NULL, "Invalid argument: Evas_Object is NULL\n");

	edje_object_part_geometry_get(_EDJ(ly),
			"padding/t", NULL, NULL, &pw, &ph);
	evas_object_geometry_get(obj, NULL, NULL, &w, &h);
	evas_object_size_hint_min_set(ly, (w + ph * 2), (h + ph * 2));
}

static void _ctxpopup_hide_cb(void *data, Evas_Object *obj, void *event_info)
{
	struct appdata *ad = (struct appdata *)data;
	ctxpopup_clear(ad);
}

int _util_move_ctxpopup(Evas_Object *eo, Evas_Object *target)
{
	double scale;
	Evas_Coord tx, ty, tw, th;
	Evas_Coord cx, cy;

	retvm_if(eo == NULL, -1, "Invalid argument: ctxpopup is NULL\n");

	scale = elm_config_scale_get();

	evas_object_geometry_get(target, &tx, &ty, &tw, &th);

	cx = tx + (int)(tw * 0.5);
	cy = ty + (int)(10.0 * scale);

	evas_object_move(eo, cx, cy);

	return 0;
}

static Eina_Bool info_hide_cb(void *data)
{
	struct appdata *ad = data;

	ctxpopup_clear(ad);

	return ECORE_CALLBACK_CANCEL;
}

void create_info_ctxpopup(void *data, Evas_Object *obj, void *event_info)
{
	struct appdata *ad = (struct appdata *)data;
	Evas_Object *lb, *ly;
	double scale = 0.0;
	char buf[128] = {0, };

	retm_if(data == NULL, "Invalid argument: appdata is NULL\n");

	if (ad->info_ctxpopup) {
		evas_object_del(ad->info_ctxpopup);
		ad->info_ctxpopup = NULL;
	}

	scale = elm_config_scale_get();

	ad->info_ctxpopup = _add_ctxpopup(ad->win);
	evas_object_smart_callback_add(ad->info_ctxpopup, "dismissed",
				       _ctxpopup_hide_cb, ad);

	ly = _add_layout(ad->info_ctxpopup, EDJ_NAME, "info");
	evas_object_resize(ly, (int)(240.0 * scale), (int)(105.0 * scale));
	elm_object_content_set(ad->info_ctxpopup, ly);

	snprintf(buf, sizeof(buf),
			"<font_size=22>%s<font_size>", _("IDS_TASKMGR_INFO_MESSAGE"));
	lb = _add_label(ly, buf);
	evas_object_event_callback_add(lb, EVAS_CALLBACK_RESIZE,
				       label_resize_cb, ly);
	elm_layout_content_set(ly, "swallow", lb);

	_util_move_ctxpopup(ad->info_ctxpopup, ad->info_btn);

	ad->info_timer = ecore_timer_add(3, info_hide_cb, ad);

	evas_object_show(ad->info_ctxpopup);
}


