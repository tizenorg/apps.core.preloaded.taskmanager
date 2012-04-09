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



#ifndef __TASKMANAGER_EINA_H__
#define __TASKMANAGER_EINA_H__

#include <aul.h>

#include "taskmanager.h"

void _init_grp_cnt(void);
int _get_grp_cnt(int which);
int runapp_info_get(const aul_app_info *ainfo, void *data);
int taskmanager_get_history_app_info(void *data);
int _free_einalist_all(struct appdata *ad);
int _subt_einalist_item(struct appdata *ad, int pid);

#endif
/* __TASKMANAGER_EINA_H__ */
