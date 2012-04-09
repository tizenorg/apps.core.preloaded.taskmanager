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



#ifndef __TASKMANAGER_LOGIC_H__
#define __TASKMANAGER_LOGIC_H__

#include "taskmanager.h"

int _dead_cb(int pid, void *data);
Eina_Bool _create_idler_cb(void *data);
int _app_create(struct appdata *ad);
void _ok_response_cb(void *data, Evas_Object *obj, void *event_info);
void _cancel_response_cb(void *data, Evas_Object *obj, void *event_info);

#endif
/* __TASKMANAGER_LOGIC_H__ */
