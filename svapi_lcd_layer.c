/*
 *  Copyright (C) 2015 by Desay SV automotive
 *  Created by Ye Yongjie 2015-11-10
 */
#include "svapi.h"

static char *layer_dev[4][3] = {
	{
		"/sys/devices/platform/sirfsoc_vdss.0/lcd0-layer0/layer_enable",
		"/sys/devices/platform/sirfsoc_vdss.0/lcd0-layer0/layer_alpha",
		"/sys/devices/platform/sirfsoc_vdss.0/lcd0-layer0/layer_ckey"
	},	
	{
		"/sys/devices/platform/sirfsoc_vdss.0/lcd0-layer1/layer_enable",
		"/sys/devices/platform/sirfsoc_vdss.0/lcd0-layer1/layer_alpha",
                "/sys/devices/platform/sirfsoc_vdss.0/lcd0-layer1/layer_ckey"		
	},
        {
                "/sys/devices/platform/sirfsoc_vdss.0/lcd0-layer2/layer_enable",
                "/sys/devices/platform/sirfsoc_vdss.0/lcd0-layer2/layer_alpha",
                "/sys/devices/platform/sirfsoc_vdss.0/lcd0-layer2/layer_ckey"
        },
        {
                "/sys/devices/platform/sirfsoc_vdss.0/lcd0-layer3/layer_enable",
                "/sys/devices/platform/sirfsoc_vdss.0/lcd0-layer3/layer_alpha",
                "/sys/devices/platform/sirfsoc_vdss.0/lcd0-layer3/layer_ckey"
        }
};

static int svapi_lcd_layer_set_prop(char *dev, unsigned int value)
{
        int ret = -1;
        int count = -1;
	int fd = -1;
        char buf[16] = {0};

         fd = open(dev, O_RDWR);
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

int svapi_layer_set_prop_conf(LAYER_PROP_CONF *layer_prop_conf)
{
	
	if(layer_prop_conf->layer_id > WARNING_LAYER || layer_prop_conf->prop_id >= MAX_PROP){
		printf("svapi layer id or prop id is over max\n");
		return SVAPI_E_ERROR;
	}

	if((layer_prop_conf->prop_id == ENABLE_PROP && layer_prop_conf-> prop_value > 1) ||
		(layer_prop_conf->prop_id == ALPHA_PROP && layer_prop_conf-> prop_value > 255) ){
		printf("svapi layer prop value is invalid \n");
		return SVAPI_E_ERROR;
	}	
	
	return svapi_lcd_layer_set_prop(layer_dev[layer_prop_conf->layer_id][layer_prop_conf->prop_id], layer_prop_conf-> prop_value);
}

int svapi_lcd_layer_get_prop_int(char *dev, unsigned int *value)
{
        int ret = -1;
        char buf[16];
	int fd = -1;

        if (value == NULL) {
                LOGE("pointer to value is NULL\n");
                return SVAPI_E_ERROR;
        }

        fd = open(dev, O_RDWR);
        if (fd < 0) {
                LOGE("%s\n", dev);
                perror("Open");
                return SVAPI_E_ERROR;
        }
        ret = read(fd, buf, 15);
        *value = atoi(buf);
        close(fd);
        if (ret != -1)
                return SVAPI_E_OK;
        return SVAPI_E_ERROR;
}

int svapi_layer_get_prop_conf(LAYER_PROP_CONF *layer_prop_conf)
{
	
        if(layer_prop_conf->layer_id > WARNING_LAYER || layer_prop_conf->prop_id >= MAX_PROP){
                printf("svapi layer id or prop id is over max\n");
                return SVAPI_E_ERROR;
        }
	
	return svapi_lcd_layer_get_prop_int(layer_dev[layer_prop_conf->layer_id][layer_prop_conf->prop_id], &layer_prop_conf-> prop_value);
}


