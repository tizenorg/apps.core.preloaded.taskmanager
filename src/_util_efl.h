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



#ifndef __TASKMANAGER_UTIL_EFL_H__
#define __TASKMANAGER_UTIL_EFL_H__

#include <Elementary.h>

Evas_Object *_add_window(const char *name);
Evas_Object *_add_bg(Evas_Object *parent, char *style);
Evas_Object *_add_genlist(Evas_Object *parent);
Evas_Object *_add_icon(Evas_Object *parent, const char *png);
Evas_Object *_add_layout(Evas_Object *parent, const char *file, const char *group);
Evas_Object *_add_ctxpopup(Evas_Object *parent);
Evas_Object *_add_label(Evas_Object *parent, const char *msg);
Evas_Object *_add_naviframe(Evas_Object *parent);
Evas_Object *_add_layout_main(Evas_Object *parent,
		Eina_Bool content, Eina_Bool transparent);
Evas_Object *_add_progressbar(Evas_Object *parent, const char *style,
		Evas_Coord w, Evas_Coord h);
Evas_Object *_add_popup_ask(Evas_Object *parent, char *text, void *data);
void util_show_popup_with_message(Evas_Object *parent, double in, const char *msg);

#endif
/* __TASKMANAGER_UTIL_EFL_H__ */
