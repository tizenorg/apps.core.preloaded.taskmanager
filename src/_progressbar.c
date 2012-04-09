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



#include "taskmanager.h"
#include "_util_log.h"
#include "_util_efl.h"

void _del_progressbar(void *data)
{
	struct appdata *ad = (struct appdata *)data;
	retm_if(ad == NULL, "Invalid argument: appdata is NULL\n");

	if (ad->popup_progressbar) {
		evas_object_hide(ad->popup_progressbar);
		evas_object_del(ad->popup_progressbar);
		ad->popup_progressbar = NULL;
	}
}

void _show_progressbar(void *data)
{
	struct appdata *ad = (struct appdata *)data;
	retm_if(ad == NULL, "Invalid argument: appdata is NULL\n");

	if (ad->popup_progressbar)
		_del_progressbar(ad);

	ad->popup_progressbar = _add_progressbar(ad->win, "list_process",
			ad->root_w, ad->root_h);

	evas_object_show(ad->popup_progressbar);
}
