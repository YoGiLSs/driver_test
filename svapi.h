/*
 *  Copyright (C) 2012 by Desay SV automotive
 */
#ifndef _SVAPI_H_
#define _SVAPI_H_
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <error.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LOGE(fmt, arg...) printf(fmt, ##arg)

#ifdef DEBUG
#define LOGD(fmt, arg...) printf(fmt, ##arg)
#else
#define LOGD
#endif

/*
 * Function Naming Rules: svapi_[module]_[operation]_[content]
 * Return values (< 0 = Error)
 */
#define SVAPI_E_OK		0
#define SVAPI_E_ERROR		(-1)

/*****************************************************************************
 * Version Info
 ****************************************************************************/
#define SV_API_VERSION	"1.3.0"

/*
 * Function svapi_api_get_version
 * Check the compatibility of libsvapi.so and svapi.h
 * NEED: len >= 16 bytes
 * if "pbuf==SV_API_VERSION", same version
 */
int svapi_api_get_version(char *buf, int len);

/*****************************************************************************
 * Software Upgrade Interface
 ****************************************************************************/
#define UPGRADE_OS		(1U << 0)
#define UPGRADE_SVP		(1U << 1)
#define UPGRADE_APP		(1U << 2)
#define UPGRADE_LOGO		(1U << 3)
#define UPGRADE_RADIO		(1U << 4)

/* Media Type */
#define EMMC_BLOCK	1
#define USB_SDCARD	2
/* Callback to show progress bar */
typedef void (* upgrade_status_callback)(unsigned int percent, char *info);

/*
 * Function svapi_upgrade_get_flag
 * return  0 Success
 * return -1 Failure
 */
int svapi_upgrade_get_flag(unsigned int *pflag);

/*
 * Function svapi_upgrade_set_flag
 * int flag = (UPGRADE_SVP | UPGRADE_APP...) and then call svapi_upgrade_set_flag(flag);
 * return  0 Success
 * return -1 Failure
 */
int svapi_upgrade_set_flag(unsigned int flag);

/*
 * Function svapi_upgrade_clear_flag
 * int flag = (UPGRADE_SVP | UPGRADE_APP...) and then call svapi_upgrade_clear_flag(flag);
 * return  0 Success
 * return -1 Failure
 */
int svapi_upgrade_clear_flag(unsigned int flag);

/*
 * Function svapi_upgrade_package_is_vaild
 * int packages = (UPGRADE_SVP | UPGRADE_APP...)
 * unsigned int media = EMMC_BLOCK or USB_SDCARD
 * return  0 valid
 * return -1 Failure
 */
int svapi_upgrade_package_is_valid(unsigned int packages, unsigned int media);

/*
 * Function svapi_upgrade_copy_file : backup system.bin svp.bin app-pkg.bin to EMMC
 * int packages = (UPGRADE_SVP | UPGRADE_APP...)
 * unsigned int media = USB_SDCARD
 * return  0 Success
 * return -1 Failure
 */
int svapi_upgrade_copy_to_emmc(unsigned int packages, unsigned int media, upgrade_status_callback status_cb);

/*****************************************************************************
 * Software Upgrade Interface for Recovery Mode only
 ****************************************************************************/
#define UPGRADE_U1		(1U << 16)
#define UPGRADE_U2		(1U << 17)
#define UPGRADE_UIMAGE		(1U << 18)
#define UPGRADE_TARGETFS	(1U << 19)
/*
 * Function svapi_upgrade_package_get_path
 * int package = UPGRADE_SVP ,call svapi_upgrade_package_to_file(package, dir, filename, status_cb);
 * return  0 Success
 * return -1 Failure
 */
int svapi_upgrade_package_to_file(unsigned int package, const char *dir, const char *name, upgrade_status_callback status_cb);

/*****************************************************************************
 * Sensor Interface
 ****************************************************************************/
/*
 * Function svapi_gyro_get_value
 * 12 bit ADC sample value
 */
int svapi_gyro_get_value(unsigned int *pvalue);

/*
 * Function svapi_acc_get_value
 * 12 bit ADC sample value
 */
int svapi_acc_get_value(unsigned int *pvalue);

/*
 * Function svapi_dir_get_value
 * Get rearview pin state
 *  *pvalue 0: Reverse
 *  *pvalue 1: Drive
 */
int svapi_reverse_get_value(unsigned int *pvalue);

/*
 * Function svapi_velo_get_value
 * Get velocity value from 32 bit MCU, Message len 14Byte
 * NEED: len >= 14 bytes
 */
int svapi_velo_get_value(char *buff, int len);

/*****************************************************************************
 * backlight Interface
 ****************************************************************************/
/*
 * Function svapi_backlight_on_off
 * value   0:off  1:on
 * return  0 Success
 * return -1 Failure
 */
int svapi_backlight_on_off(int value);

/*****************************************************************************
 * brightness Interface
 ****************************************************************************/
/*
 * Function svapi_brightness_setting
 * value   1-10
 * return  0 Success
 * return -1 Failure
 */
int svapi_brightness_setting(int value);

/*****************************************************************************
 * buletooth control Interface
 ****************************************************************************/
/*
 * Function svapi_buletooth_on_off
 * value   0:off  1:on
 * return  0 Success
 * return -1 Failure
 */
int svapi_bluetooth_power_on_off(int value);

/*****************************************************************************
 * wifi control Interface
 ****************************************************************************/
/*
 * Function svapi_wifi_on_off
 * value   0:off  1:on
 * return  0 Success
 * return -1 Failure
 */
int svapi_wifi_power_on_off(int value);

/*****************************************************************************
 * Disable or Enable touch panel
 ****************************************************************************/
/*
 * Function svapi_touch_on_off
 * value   0:off  1:on
 * return  0 Success
 * return -1 Failure
 */
int svapi_touch_on_off(int value);

/*****************************************************************************
 * Disable or Enable rvc_power
 ****************************************************************************/
/*
 * Function svapi_rvc_power_on_off
 * value   0:off  1:on
 * return  0 Success
 * return -1 Failure
 */
int svapi_rvc_power_on_off(int value);

/*****************************************************************************
 * rvc power status
 ****************************************************************************/
/*
 * Function svapi_get_rvc_power_status
 * status
 *   0: power down
 *   1: power on
 * return  0 Success
 * return -1 Failure
 */
int svapi_get_rvc_power_status(int *status);

/*****************************************************************************
 * Card id Interface
 ****************************************************************************/
/*
 * Function svapi_get_cid
 * pvalue  buffer for cid, need 32bit
 * type	   sdcardX(sdcard0, sdcard1, ...)	hub_sdcard0
 * return  0 Success
 * return -1 Failure
 */
int svapi_get_cid(char *pvalue, const char *type);

/*****************************************************************************
 * GPS Antenna detect
 ****************************************************************************/
/*
 * Function svapi_gps_antenna_status
 * status
 *   0 :short connect
 *   1 :normal connect
 *   2 :no connect
 *   3 :invaild
 * return  0 Success
 * return -1 Failure
 */
int svapi_gps_antenna_status(int *status);

/*****************************************************************************
 * USB Hub Detect Interface
 ****************************************************************************/
/*
 * Function svapi_usb_hub_detect
 * device_num 0:musb-hdrc.0  1:musb-hdrc.1
 * return  0 usb hub is connected
 * return -1 usb hub is not connected
 */
int svapi_usb_hub_detect(int device_num);

/*****************************************************************************
 * Display Detect Interface
 ****************************************************************************/
/*
 * Function svapi_display_detect
 * status
 *   0: detect no error
 *   1: detect error
 * index
 *   0: missing_message_detect
 *   1: voltage_error_detect
 *   2: interal_error_detect
 *   3: HDCP Recognition
 * return  0 Success
 * return -1 Failure:Display not response or incorrect parameter
 */
int svapi_display_detect(int *status,int index);

/*****************************************************************************
 * Display version info Interface
 ****************************************************************************/
/*
 * Function svapi_display_version_info
 * buf
 *	buf size must be bigger than 18 byte to record version
 * index
 *   0: display software version
 *   1: touchpanel config version
 * return  0 Success
 * return -1 Failure:Display not response or incorrect parameter
 */
int svapi_display_version_info(char *buf,int index);

/*****************************************************************************
 * Disable or Enable watchdog
 ****************************************************************************/
/*
 * Function svapi_watchdog_on_off
 * value   0:off  1:on
 * return  0 Success
 * return -1 Failure
 */
int svapi_watchdog_on_off(int value);

/*****************************************************************************
 * Camera Signal Detect Interface
 ****************************************************************************/
/*
 * Function svapi_video_signal_detect
 * num 0:rvc video signal
 * return  0 video signal is detected
 * return -1 video signal is not detected
 */
int svapi_video_signal_detect(int num);

/*****************************************************************************
 * RVC Caution TEXT Language setting
 ****************************************************************************/
/*
 * Function svapi_rvc_caution_on_off
 * on_off:  0:off  1:on
 * return  0 Success
 * return -1 Failure
 */
int svapi_rvc_caution_on_off(int on_off);

/*
 * Function svapi_rvc_caution_set
 * language:  0:US  1:CN  2:JP  0xFF:NONE
 * return  0 Success
 * return -1 Failure
 */
int svapi_rvc_caution_set(int language);

/*
 * Function svapi_rvc_caution_get
 * language:  0:US  1:CN  2:JP  0xFF:NONE
 * return  0 Success
 * return -1 Failure
 */
int svapi_rvc_caution_get(int *language);

/*****************************************************************************
 * Get disk label
 ****************************************************************************/
/*
 * Function svapi_get_label
 * mount_path: /media/sdcardXpX
 * return  0 Success
 * return -1 Failure
 */
int svapi_get_label(const char *mount_path, char *label_buf, int label_buf_size);

/*****************************************************************************
 * Display GAMMA
 ****************************************************************************/
/*
 * Function svapi_gamma_set
 * gamma_curve_number: select which gamma curve num,value range(0~14), default is 7
 * return  0 gamma curve set success
 * return -1  gamma curve set failed
 */
int svapi_gamma_set(int gamma_curve_number);

/*
 * Function svapi_gamma_get
 * gamma_curve_number: value range(0~14)
 * return  0 gamma curve get success
 * return -1  gamma curve get failed
 */
int svapi_gamma_get(int *gamma_curve_number);

typedef struct {
	int brightness; /*0 <= brightness <= 255*/
	int contrast; /*0 <= contrast <= 255*/
	int saturation; /*0 <= saturation <= 255*/
	int hue; /*-128 <= hue <= 128*/
	int channelNum; /* 0:rvc ;1:dvd(dvd not support)*/
} CONF_VIDEO;

/*
 * Function svapi_video_config_set
 * return  0 Success
 * return -1 Failure
 */
int svapi_video_config_set(CONF_VIDEO *conf_enc);

/*
 * Function svapi_video_config_get
 * return  0 Success
 * return -1 Failure
 */
int svapi_video_config_get(CONF_VIDEO *conf_enc);

/*****************************************************************************
 * BT and WIFI MAC address
 ****************************************************************************/
/*
 * Function svapi_bt_get_mac
 * mac: 6 Bytes
 *        example: AA:BB:CC:DD:EE:FF
 *                 mac[0]:mac[1]:mac[2]:mac[3]:mac[4]:mac[5]
 * len == 6
 * return  0 Success
 * return -1 Failure
 */
int svapi_bt_get_mac(unsigned char *mac, int len);

/*
 * Function svapi_wifi_get_mac
 * mac: 6 Bytes
 *        example: AA:BB:CC:DD:EE:FF
 *                 mac[0]:mac[1]:mac[2]:mac[3]:mac[4]:mac[5]
 * len == 6
 * return  0 Success
 * return -1 Failure
 */
int svapi_wifi_get_mac(unsigned char *mac, int len);

/*
 * Function svapi_bt_factory_burn_mac
 * mac: 6 Bytes
 * return  0 Success
 * return -1 Failure
 */
int svapi_bt_factory_burn_mac(unsigned char *mac, int len);

/*
 * Function svapi_wifi_factory_burn_mac
 * mac: 6 Bytes
 * return  0 Success
 * return -1 Failure
 */
int svapi_wifi_factory_burn_mac(unsigned char *mac, int len);

/*
 * Function svapi_factory_reset
 * return  0 Success
 * return -1 Failure
 */
int svapi_factory_reset(void);

/*
 * Function svapi_tmc_get_device_info
 * id: 6 Bytes
 * return  0 Success
 * return -1 Failure
 */
int svapi_tmc_get_device_info(unsigned char *id, int len);

/*
 * Function svapi_tmc_set_device_info
 * id: 6 Bytes
 * return  0 Success
 * return -1 Failure
 */
int svapi_tmc_set_device_info(unsigned char *id, int len);

/*****************************************************************************
 * tuner control Interface
 ****************************************************************************/
/*
 * Function svapi_tuner_power_on_off
 * value   0:off  1:on
 * return  0 Success
 * return -1 Failure
 */
int svapi_tuner_power_on_off(int value);

/*****************************************************************************
 * usb otg set mode Interface
 ****************************************************************************/
/*
 * Function: svapi_otg_set_mode
 * port: usb1 or usb2
 * mode: host or device
 * return  0 Success
 * return -1 Failure
 */
int svapi_otg_set_mode(const char *port, const char *mode);

/*****************************************************************************
 * layer control Interface
 ****************************************************************************/
typedef enum {
        GRAPHIC_LAYER = 0,
        VIDEO_LAYER,
        RVC_LAYER,
        WARNING_LAYER,
}LAYER_ID;

typedef enum {
        ENABLE_PROP = 0,
        ALPHA_PROP,
        CKEY_PROP,
        MAX_PROP,
}LAYER_PROP_ID;

typedef struct {
        unsigned char layer_id;
        unsigned char prop_id;
        unsigned int prop_value;
}LAYER_PROP_CONF;

/*
 * Function: svapi_layer_set_prop_conf
 * layer_prop_conf layer prop
 * return  0 Success
 * return -1 Failure
 */

int svapi_layer_set_prop_conf(LAYER_PROP_CONF *layer_prop_conf);

/*
 * Function: svapi_layer_get_prop_conf
 * layer_prop_conf layer prop
 * return  0 Success
 * return -1 Failure
 */

int svapi_layer_get_prop_conf(LAYER_PROP_CONF *layer_prop_conf);

/*****************************************************************************
 * pa control Interface
 ****************************************************************************/
/*
 * Function svapi_pa_set_mute
 * value   mute
 * return  0 Success
 * return -1 Failure
 */

int svapi_pa_set_mute(unsigned char value);

#ifdef __cplusplus
}
#endif
#endif /* SVAPI_H */
