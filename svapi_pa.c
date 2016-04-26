/*
 *  Copyright (C) 2015 by Desay SV automotive
 *  Created by Ye Yongjie 2015-11-10
 */
#include "svapi.h"

static char *pa_mute = "/sys/bus/i2c/drivers/nxp_pa/0-006e/mute";

static int svapi_pa_mute_set_prop(char *dev, unsigned int value)
{
        int ret = -1;
        int count = -1;
	int fd = -1;
        char buf[16] = {0};

         fd = open(dev, O_WRONLY);
         if (fd < 0) {
         	LOGE("%s\n", dev);
                perror("Open");
                return SVAPI_E_ERROR;
         }

        count = sprintf(buf, "%d", value);
        ret = write(fd, buf, count);
        close(fd);
        if (ret == count && count != -1)
                return SVAPI_E_OK;
        return SVAPI_E_ERROR;
}

int svapi_pa_set_mute(unsigned char value)
{		
	return svapi_pa_mute_set_prop(pa_mute, value);
}



