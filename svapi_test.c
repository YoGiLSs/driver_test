/*
 *  Copyright (C) 2012 by Desay SV automotive
 */
#include "svapi.h"

int main(int argc, char *argv[])
{
	int ret = -1;
	int value = -1;
	char api_version[16];
	unsigned int layer_value = 0;
	unsigned char alpha = 0;
	CONF_VIDEO conf_enc;
	LAYER_PROP_CONF layer_prop_conf;

	switch (argc) {
	case 1:
		break;
	case 2:
		break;
	default:
		if (strcmp(argv[1], "tuner") == 0) {
			if (strcmp(argv[2], "on") == 0) {
				printf("TUNER: on, ret=%d\n", ret);
			} else if (strcmp(argv[2], "off") == 0) {
				printf("TUNER: off, ret=%d\n", ret);
			} else if (strcmp(argv[2], "status") == 0) {
				printf("TUNER: power status, ret=%d, status=%d\n", ret, value);
			} else
				printf("ERROR\n");
		}else if (strcmp(argv[1], "usb_otg") == 0) {
			if (strcmp (argv[2], "usb1") == 0) {				
					if(strcmp (argv[3], "device") == 0){
						printf("usb1 set device\n");
					}
					else if(strcmp (argv[3], "host") == 0){
						printf("usb1 set host\n");
					}
					else{
						printf("ERROR\n");
					}
			} else if (strcmp (argv[2], "usb2") == 0) {
					if(strcmp (argv[3], "device") == 0){
						printf("usb2 set device\n");
					}
					else if(strcmp (argv[3], "host") == 0){
						printf("usb2 set host\n");
					}
					else{
						printf("ERROR\n");
					}
			} else {
				printf("ERROR\n");
			}
		}else if(strcmp(argv[1], "pa") == 0){
			if(strcmp(argv[2], "mute") == 0){
				if (strcmp (argv[3], "set") == 0) {
					value = atoi(argv[4]);
				//	ret = svapi_pa_set_mute((unsigned char)value);
					printf("set pa mute ret is %d \n", ret);
				}else{
					printf("ERROR\n");
				}
			}else{
				printf("ERROR\n");
			}
		}else if(strcmp(argv[1], "conf_video") == 0){
			if(argc == 7){
				conf_enc.brightness = atoi(argv[2]);
				conf_enc.contrast = atoi(argv[3]);
				conf_enc.saturation = atoi(argv[4]);
				conf_enc.hue = atoi(argv[5]);
				conf_enc.channelNum = atoi(argv[6]);
			//	svapi_video_config_set(&conf_enc);
			}else if(strcmp(argv[2], "get") == 0){
				conf_enc.channelNum = 0;
			//	svapi_video_config_get(&conf_enc);
				printf("get conf_video brightness:%d contrast:%d saturation:%d hue:%d \n", conf_enc.brightness,conf_enc.contrast,conf_enc.saturation,conf_enc.hue);
			}else{
				printf("error: conf_video requires five parameters: brightness contrast saturation hue channelNum\n");
			}
		}else if(strcmp(argv[1], "conf_layer") == 0){
			if(strcmp(argv[2], "set") == 0){
				layer_prop_conf.layer_id = atoi(argv[3]);
				layer_prop_conf.prop_id = atoi(argv[4]);
				layer_prop_conf.prop_value = atoi(argv[5]);
				ret = svapi_layer_set_prop_conf(&layer_prop_conf);
				printf("set conf_layer ret is %d \n", ret);		
			}else if(strcmp(argv[2], "get") == 0){
				layer_prop_conf.layer_id = atoi(argv[3]);
                                layer_prop_conf.prop_id = atoi(argv[4]);
				ret = svapi_layer_get_prop_conf(&layer_prop_conf);
				printf("get conf_layer ret is %d, value is %d \n", ret, layer_prop_conf.prop_value);
			}else{
				printf("ERROR\n");
			}
		}else{
			printf("Usage:\n\n");
		}
		return 0;
	}
	return 0;
}
