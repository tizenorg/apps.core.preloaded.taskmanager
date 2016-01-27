/*
 *  Task Manager
 *
 * Copyright (c) 2000 - 2015 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <Elementary.h>
#include <package_manager.h>
#include <app_manager.h>
#include <context_history.h>

#include "list.h"
#include "log.h"
#include "main.h"
#include "util.h"
#include "conf.h"

#define GO_NEXT 1
#define BREAK 0

typedef struct pkginfo {
	char *appid;
	bool taskmanage;
	char *pkgid;
	char *icon;
	char *name;
	bool nodisplay;
} private_pkginfo_s;

typedef struct {
	char *appid;
	int pid;
} private_pid_s;

static struct {
	Eina_Hash *pkginfo_table;
} private_table_s = {
	.pkginfo_table = NULL,
};

static void _pkglist_unretrieve_item(list_type_default_s *default_info)
{
	if (!default_info) {
		return;
	}

	if (default_info->name) {
		free(default_info->name);
		default_info->name = NULL;
	}
	if (default_info->icon) {
		free(default_info->icon);
		default_info->icon = NULL;
	}
	if (default_info->pkgid) {
		free(default_info->pkgid);
		default_info->pkgid = NULL;
	}
	if (default_info->arg) {
		free(default_info->arg);
		default_info->arg = NULL;
	}
	if (default_info->appid) {
		free(default_info->appid);
		default_info->appid = NULL;
	}
	if (default_info) {
		free(default_info);
		default_info = NULL;
	}
}


static task_mgr_error_e _get_app_launchtime(Eina_List *pkg_list)
{

	int ret = CONTEXT_HISTORY_ERROR_NONE;
	int rec_size = 0;
	int last_launch_time;
	char *context_app_id;
	Eina_List *l;
	list_type_default_s *pkg_info = NULL;

	context_history_h history_handle;
	context_history_list_h history_app_list;
	context_history_record_h history_record;

	ret = context_history_create(&history_handle);
	retv_if(ret != CONTEXT_HISTORY_ERROR_NONE, TASK_MGR_ERROR_FAIL);

	ret = context_history_get_list(history_handle,
			CONTEXT_HISTORY_RECENTLY_USED_APP, NULL, &history_app_list);
	retv_if(ret != CONTEXT_HISTORY_ERROR_NONE, TASK_MGR_ERROR_FAIL);

	ret = context_history_list_get_count(history_app_list, &rec_size);
	retv_if(ret != CONTEXT_HISTORY_ERROR_NONE, TASK_MGR_ERROR_FAIL);

	_D("Num of records: %d", rec_size);

	int i;
	for (i = 0; i < rec_size; ++i) {
		context_history_list_get_current(history_app_list, &history_record);
		context_history_record_get_int(history_record, CONTEXT_HISTORY_LAST_TIME, &last_launch_time);
		context_history_record_get_string(history_record, CONTEXT_HISTORY_APP_ID, &context_app_id);

		EINA_LIST_FOREACH(pkg_list, l, pkg_info) {
			if(!strcmp(pkg_info->appid, context_app_id))
			{
				pkg_info->launch_time = last_launch_time;
			}
		}

		free(context_app_id);

		context_history_record_destroy(history_record);
		context_history_list_move_next(history_app_list);
	}

	context_history_list_destroy(history_app_list);
	context_history_destroy(history_handle);

	return TASK_MGR_ERROR_NONE;
}

static bool _get_pkginfo_cb(app_info_h app_handle, void *user_data)
{
	_D("");

	char *appid = NULL;
	char *pkgid = NULL;
	char *name = NULL;
	char *icon = NULL;
	bool is_running = false;
	bool nodisplay = false;

	Eina_List **pkg_list = (Eina_List **) user_data;
	list_type_default_s *default_pkg_info = NULL;

	if (app_info_get_app_id(app_handle, &appid) != APP_MANAGER_ERROR_NONE
			|| !appid)
		goto ERROR;

	if (app_manager_is_running(appid, &is_running) != APP_MANAGER_ERROR_NONE
			|| !is_running) {

		_D("Application %s is not running", appid);
		free(appid);

		return GO_NEXT;
	}

	if (app_info_get_package(app_handle, &pkgid) != APP_MANAGER_ERROR_NONE
			|| !pkgid)
		goto ERROR;

	if (app_info_get_label(app_handle, &name) != APP_MANAGER_ERROR_NONE
			|| !name)
		goto ERROR;

	if (app_info_is_nodisplay(app_handle, &nodisplay) != APP_MANAGER_ERROR_NONE)
		goto ERROR;

	if (app_info_get_icon(app_handle, &icon) != APP_MANAGER_ERROR_NONE || !icon)
		goto ERROR;

	default_pkg_info = calloc(1, sizeof(list_type_default_s));
	retv_if(!default_pkg_info, BREAK);

	default_pkg_info->appid = appid;
	default_pkg_info->pkgid = pkgid;
	default_pkg_info->name = name;
	default_pkg_info->nodisplay = nodisplay;

	if (0 == access(icon, F_OK)) {
		default_pkg_info->icon = icon;
	} else {
		_D("Fail to access icon path");
		default_pkg_info->icon = strdup(DEFAULT_ICON);
	}

	_D("AppID: %s", default_pkg_info->appid);
	_D("PkgID: %s", default_pkg_info->pkgid);
	_D("Icon: %s", default_pkg_info->icon);
	_D("Label: %s", default_pkg_info->name);

	*pkg_list = eina_list_append(*pkg_list, default_pkg_info);

	return GO_NEXT;

ERROR:
	_E("Cannot get pkg info");
	if (name)
		free(name);
	if (icon)
		free(icon);
	if (pkgid)
		free(pkgid);
	if (appid)
		free(appid);
	if (default_pkg_info)
		free(default_pkg_info);

	return BREAK;
}

static Eina_Bool _remove_pkginfo(const Eina_Hash *hash, const void *key, void *data, void *fdata)
{
	retv_if(!data, EINA_FALSE);

	private_pkginfo_s *pkg_info = data;

	if (pkg_info->name) free(pkg_info->name);
	if (pkg_info->icon) free(pkg_info->icon);
	if (pkg_info->pkgid) free(pkg_info->pkgid);
	if (pkg_info->appid) free(pkg_info->appid);

	return EINA_TRUE;
}

static void _destroy_pkginfo_table(void)
{
	_D("");

	eina_hash_foreach(private_table_s.pkginfo_table, _remove_pkginfo, NULL);
	eina_hash_free(private_table_s.pkginfo_table);
	private_table_s.pkginfo_table = NULL;
}

static int _launch_time_sort_cb(const void *d1, const void *d2)
{
	list_type_default_s *tmp1 = (list_type_default_s *) d1;
	list_type_default_s *tmp2 = (list_type_default_s *) d2;

	if (!tmp1) return -1;
	if (!tmp2) return 1;

	if (!tmp1->appid) return 1;
	else if (!tmp2->appid) return -1;

	return tmp1->launch_time >= tmp2->launch_time ? -1 : 1;
}

static task_mgr_error_e _get_running_apps(Eina_List **pkg_list)
{
	_D("");

	app_info_filter_h handle;
	int ret = 0;

	ret = app_info_filter_create(&handle);
	if (ret != APP_MANAGER_ERROR_NONE)
		return TASK_MGR_ERROR_FAIL;

	ret = app_info_filter_add_bool(handle, PACKAGE_INFO_PROP_APP_TASKMANAGE, 1);
	if (ret != APP_MANAGER_ERROR_NONE)
		return TASK_MGR_ERROR_FAIL;

	ret = app_info_filter_foreach_appinfo(handle, _get_pkginfo_cb, pkg_list);
	if (ret != APP_MANAGER_ERROR_NONE)
		return TASK_MGR_ERROR_FAIL;

	app_info_filter_destroy(handle);

	return TASK_MGR_ERROR_NONE;
}

 Eina_List *list_sort(Eina_List *list, int (*_sort_cb)(const void *d1, const void *d2))
 {
	retv_if(!list, NULL);

	list = eina_list_sort(list, eina_list_count(list), _sort_cb);
	retv_if(!list, NULL);

	return list;
}

extern task_mgr_error_e list_create(Eina_List **pkg_list)
{
	int ret = TASK_MGR_ERROR_NONE;

	ret = _get_running_apps(pkg_list);
	retv_if(ret != TASK_MGR_ERROR_NONE, TASK_MGR_ERROR_FAIL);

	ret = _get_app_launchtime(*pkg_list);
	retv_if(ret != TASK_MGR_ERROR_NONE, TASK_MGR_ERROR_FAIL);

	*pkg_list = list_sort(*pkg_list, _launch_time_sort_cb);
	retv_if(!*pkg_list, TASK_MGR_ERROR_FAIL);

	return TASK_MGR_ERROR_NONE;
}

extern void list_destroy(Eina_List *pkg_list)
{
	_D("");
	list_type_default_s *default_info = NULL;

	_destroy_pkginfo_table();

	if (!pkg_list) {
		_D("pkg_list is null");
		return;
	}

	EINA_LIST_FREE(pkg_list, default_info) {
		if (!default_info) {
			continue;
		}
		_pkglist_unretrieve_item(default_info);
	}

	pkg_list = NULL;
}
