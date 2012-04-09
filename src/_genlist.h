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



#ifndef __TASKMANAGER_GENLIST_H__
#define __TASKMANAGER_GENLIST_H__

#include <Elementary.h>
#include "taskmanager.h"

Eina_Bool alert_app_info(void *data);
void clear_task_manager_list(void *data);
void refresh_app_info(struct appdata *ad);
void load_task_manager_list(struct appdata *ad);
void _set_itc(void);
void _set_genlist(struct appdata *ad);
void _del_popup_timer(struct appdata *ad);
int response_end_inuse(struct appdata *ad);
int response_end_all_inuse(struct appdata *ad);
int response_del_history(struct appdata *ad);
int response_del_all_history(struct appdata *ad);
int response_kill_inuse(struct appdata *ad);
int response_kill_all_inuse(struct appdata *ad);
void _fini_pthread(void);
Eina_Bool _update_list(void *data);
void _restart_pthread(struct appdata *ad);

#endif
/* __TASKMANAGER_GENLIST_H__ */
