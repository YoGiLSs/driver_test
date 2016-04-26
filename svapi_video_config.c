/*
 *  Copyright (C) 2015 by Desay SV automotive
 *  Created by Ye Yongjie 2015-11-24
 */
#include "svapi.h"
#include <linux/videodev2.h>

static char *rvc_dev = "/dev/video0";

static int video_prop_set(int v4l2_fd, unsigned int id, int value)
{
	struct v4l2_control ctrl;
	int ret;
	
	ctrl.id = id;
	ret = ioctl(v4l2_fd, VIDIOC_G_CTRL, &ctrl);
	if (ret < 0){
		printf("v4l2 get ctrl id %d error \n", ctrl.id);
		return ret;
	}
	
	ctrl.value = value; //set new brightness
	ret = ioctl(v4l2_fd, VIDIOC_S_CTRL, &ctrl);
	if (ret < 0){
		printf("v4l2 set ctrl id %d value %d error \n", ctrl.id, ctrl.value);
		return ret;
	}

	return 0;
}

static int video_prop_get(int v4l2_fd, unsigned int id, int *value)
{
	struct v4l2_control ctrl;
	int ret;
	
	ctrl.id = id;
	ret = ioctl(v4l2_fd, VIDIOC_G_CTRL, &ctrl);
	if (ret < 0){
		printf("v4l2 get ctrl id %d error \n", ctrl.id);
		return ret;
	}

	*value = ctrl.value;

	return 0;
}

int svapi_video_config_set(CONF_VIDEO *p_user_conf_enc)
{
	int ret;
	CONF_VIDEO conf_enc;
	unsigned char val =0;
	int fd = -1;
	
	printf("OS:svapi:%s\n", __FUNCTION__);
	conf_enc.brightness = p_user_conf_enc->brightness;
	conf_enc.contrast = p_user_conf_enc->contrast;
	conf_enc.saturation = p_user_conf_enc->saturation;
	conf_enc.hue = p_user_conf_enc->hue;
	conf_enc.channelNum = p_user_conf_enc->channelNum;

	if((conf_enc.channelNum < 0) || (conf_enc.channelNum > 1)) {
		printf("OS:svapi:%s,channelNum=%d error,\n", __FUNCTION__,conf_enc.channelNum);
		return SVAPI_E_ERROR;
	}

	if(conf_enc.channelNum == 1){
		printf("OS:svapi:%s,DS03 do not support dvd error,\n", __FUNCTION__);
	}

	if(conf_enc.brightness < 0) {
		conf_enc.brightness = 0;
	}
	if(conf_enc.brightness > 255) {
		conf_enc.brightness = 255;
	}

	if(conf_enc.contrast < 0) {
		conf_enc.contrast = 0;
	}
	if(conf_enc.contrast > 255) {
		conf_enc.contrast = 255;
	}

	if(conf_enc.saturation < 0) {
		conf_enc.saturation = 0;
	}
	if(conf_enc.saturation > 255) {
		conf_enc.saturation = 255;
	}

	if(conf_enc.hue < -128) {
		conf_enc.hue = -128;
	}
	if (conf_enc.hue > 127) {
		conf_enc.hue = 127;
	}

	fd = open(rvc_dev, O_RDWR);
        if (fd < 0) {
                LOGE("%s\n", rvc_dev);
                perror("Open");
                return SVAPI_E_ERROR;
        }

	ret = video_prop_set(fd,V4L2_CID_BRIGHTNESS,conf_enc.brightness);
	if(ret)
		goto error_exit;	
		
	ret = video_prop_set(fd,V4L2_CID_CONTRAST,conf_enc.contrast);
	if(ret)
		goto error_exit;	

	ret = video_prop_set(fd,V4L2_CID_SATURATION,conf_enc.saturation);
	if(ret)
		goto error_exit;	

	ret = video_prop_set(fd,V4L2_CID_HUE,conf_enc.hue);
	if(ret)
		goto error_exit;	

	close(fd);
	printf("OS:svapi:%s done\n", __FUNCTION__);
	return SVAPI_E_OK;

error_exit:
	close(fd);
	return SVAPI_E_ERROR;
}

int svapi_video_config_get(CONF_VIDEO *conf_enc)
{
	int ret;
        int fd = -1;

	if((conf_enc->channelNum < 0) || (conf_enc->channelNum > 1)) {
		printf("OS:svapi:%s,channelNum=%d error,\n", __FUNCTION__,conf_enc->channelNum);
		return SVAPI_E_ERROR;
	}

	if(conf_enc->channelNum == 1){
		printf("OS:svapi:%s,DS03 do not support dvd error,\n", __FUNCTION__);
	}

        fd = open(rvc_dev, O_RDWR);
        if (fd < 0) {
                LOGE("%s\n", rvc_dev);
                perror("Open");
                return SVAPI_E_ERROR;
        }
	
	ret = video_prop_get(fd,V4L2_CID_BRIGHTNESS,&conf_enc->brightness);
	if(ret)
		goto error_exit;	
		
	ret = video_prop_get(fd,V4L2_CID_CONTRAST,&conf_enc->contrast);
	if(ret)
		goto error_exit;	

	ret = video_prop_get(fd,V4L2_CID_SATURATION,&conf_enc->saturation);
	if(ret)
		goto error_exit;	

	ret = video_prop_get(fd,V4L2_CID_HUE,&conf_enc->hue);
	if(ret)
		goto error_exit;	

	close(fd);
	return SVAPI_E_OK;

error_exit:
	close(fd);
	return SVAPI_E_ERROR;
}


