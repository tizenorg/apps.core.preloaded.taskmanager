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



#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

#include "_util_log.h"
#include "_cpu.h"

int _get_sysconf(int *ncpu, long *tick)
{
	*ncpu = sysconf(_SC_NPROCESSORS_ONLN);
	*ncpu = *ncpu < 1 ? 1 : *ncpu;
	*tick = sysconf(_SC_CLK_TCK);
	return 0;
}

int _get_stat_info(pid_t pid, unsigned int *ut, unsigned int *st)
{
	FILE *fp;
	char buf[128] = {0, };
	unsigned long cutime, cstime;
	int i;

	snprintf(buf, sizeof(buf), "/proc/%d/stat", (int)pid);

	*ut = *st = 0;
	cutime = cstime = 0;
	fp = fopen(buf, "r");
	if (fp) {
		retvm_if(fp == NULL, -1, "Failed to open %s\n", buf);
		fscanf(fp, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %lu %lu %ld %ld",
				ut, st, &cutime, &cstime);
		*ut += cutime;
		*st += cstime;
		fclose(fp);

		return 0;
	}
	return -1;
}

double _get_cpu_ratio(void *data, int ncpu, long tick)
{
	struct _task_info *tinfo = (struct _task_info *)data;
	unsigned int utime, stime;
	struct timeval timev;
	double usr, sys;
	int r;
	unsigned long long jiffy;

	retvm_if(tinfo == NULL, -1, "Invalid argument: tinfo is NULL\n");

	utime = 0;
	stime = 0;
	r = _get_stat_info(tinfo->pid, &utime, &stime);
	if (r < 0) {
		_D("failed\n");
		return 0;
	}
	/* retvm_if(r < 0, -1, "Failed to get stat info\n"); */

	gettimeofday(&timev, NULL);
	jiffy = (timev.tv_sec - tinfo->oldtimev.tv_sec) * tick +
		((timev.tv_usec - tinfo->oldtimev.tv_usec) * tick) / 1000000;

	if(utime >= tinfo->oldutime) {
		usr = ((double)(utime - tinfo->oldutime) * 100 / jiffy) / ncpu;
	} else {
		usr = 0.0;
	}
	if(stime >= tinfo->oldstime) {
		sys = ((double)(stime - tinfo->oldstime) * 100 / jiffy) / ncpu;
	} else {
		sys = 0.0;
	}

	/* _D("per:%lf] %lf %lf/ %u %u/ %u %u/ %u\n",
	   usr+sys, usr, sys, utime, stime, tinfo->oldutime, tinfo->oldstime, jiffy);
	 */

	tinfo->oldutime = utime;
	tinfo->oldstime = stime;
	tinfo->oldtimev = timev;

	return usr + sys;
}


