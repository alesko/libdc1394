/*
 * 1394-Based Digital Camera Control Library
 * Copyright (C) 2000 SMART Technologies Inc.
 *
 * Written by Gord Peters <GordPeters@smarttech.com>
 * Additions by Chris Urmson <curmson@ri.cmu.edu>
 * Additions by Damien Douxchamps <ddouxchamps@users.sf.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef __DC1394_CAMERA_CONTROL_H__
#define __DC1394_CAMERA_CONTROL_H__

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>

#define uint_t   unsigned int
#define uint16_t unsigned short int
#define uint64_t unsigned long long int
#define uchar_t  unsigned char

#ifndef octlet_t
typedef u_int64_t octlet_t;
#endif
#ifndef quadlet_t
typedef u_int32_t quadlet_t;
#endif
#ifndef nodeid_t
typedef u_int16_t nodeid_t;
#endif


/* Note on the coding of libdc1394 versions:
   - LIBDC1394_VERSION represent the release number of the library,
     e.g. 2.0.0. It consists in 4 digits, 2 for each number in the version.
     Version 2.1 will thus appear as 0201. The last version number is not
     used because a ".1" increment (as opposed to ".0.1") is necessary if
     the API had significant changes justifying the use of this number.
   - LIBDC1394_VERSION_xxx are the version codes used by libtool and defined
     in configure.in. _MAJOR is for API changes (compat break), _REVISION is
     for backward compatible API changes (e.g. new functions) and _AGE is
     when the API does not change (only code changes). At least that's my
     wild guess.
   These numbers can be used to adapt the function calls to the library
   version installed on the machine. They appeared in 2.0 so that you
   may need to check their existance if your code must work with 1.x.
*/
#define LIBDC1394_VERSION            0200
#define LIBDC1394_VERSION_MAJOR        20
#define LIBDC1394_VERSION_REVISION      0
#define LIBDC1394_VERSION_AGE           0

/* Enumeration of iso data speeds */
typedef enum {
  DC1394_ISO_SPEED_100= 0,
  DC1394_ISO_SPEED_200,
  DC1394_ISO_SPEED_400,
  DC1394_ISO_SPEED_800,
  DC1394_ISO_SPEED_1600,
  DC1394_ISO_SPEED_3200
} dc1394speed_t;
#define DC1394_ISO_SPEED_MIN                   DC1394_ISO_SPEED_100
#define DC1394_ISO_SPEED_MAX                   DC1394_ISO_SPEED_3200
#define DC1394_ISO_SPEED_NUM                  (DC1394_ISO_SPEED_MAX - DC1394_ISO_SPEED_MIN + 1)

/* Enumeration of video framerates */
typedef enum {
  DC1394_FRAMERATE_1_875= 32,
  DC1394_FRAMERATE_3_75,
  DC1394_FRAMERATE_7_5,
  DC1394_FRAMERATE_15,
  DC1394_FRAMERATE_30,
  DC1394_FRAMERATE_60,
  DC1394_FRAMERATE_120,
  DC1394_FRAMERATE_240
} dc1394framerate_t;
#define DC1394_FRAMERATE_MIN               DC1394_FRAMERATE_1_875
#define DC1394_FRAMERATE_MAX               DC1394_FRAMERATE_240
#define DC1394_FRAMERATE_NUM              (DC1394_FRAMERATE_MAX - DC1394_FRAMERATE_MIN + 1)

/* Enumeration of video modes */
typedef enum {
  DC1394_VIDEO_MODE_160x120_YUV444= 64,
  DC1394_VIDEO_MODE_320x240_YUV422,
  DC1394_VIDEO_MODE_640x480_YUV411,
  DC1394_VIDEO_MODE_640x480_YUV422,
  DC1394_VIDEO_MODE_640x480_RGB8,
  DC1394_VIDEO_MODE_640x480_MONO8,
  DC1394_VIDEO_MODE_640x480_MONO16,
  DC1394_VIDEO_MODE_800x600_YUV422,
  DC1394_VIDEO_MODE_800x600_RGB8,
  DC1394_VIDEO_MODE_800x600_MONO8,
  DC1394_VIDEO_MODE_1024x768_YUV422,
  DC1394_VIDEO_MODE_1024x768_RGB8,
  DC1394_VIDEO_MODE_1024x768_MONO8,
  DC1394_VIDEO_MODE_800x600_MONO16,
  DC1394_VIDEO_MODE_1024x768_MONO16,
  DC1394_VIDEO_MODE_1280x960_YUV422,
  DC1394_VIDEO_MODE_1280x960_RGB8,
  DC1394_VIDEO_MODE_1280x960_MONO8,
  DC1394_VIDEO_MODE_1600x1200_YUV422,
  DC1394_VIDEO_MODE_1600x1200_RGB8,
  DC1394_VIDEO_MODE_1600x1200_MONO8,
  DC1394_VIDEO_MODE_1280x960_MONO16,
  DC1394_VIDEO_MODE_1600x1200_MONO16,
  DC1394_VIDEO_MODE_EXIF,
  DC1394_VIDEO_MODE_FORMAT7_0,
  DC1394_VIDEO_MODE_FORMAT7_1,
  DC1394_VIDEO_MODE_FORMAT7_2,
  DC1394_VIDEO_MODE_FORMAT7_3,
  DC1394_VIDEO_MODE_FORMAT7_4,
  DC1394_VIDEO_MODE_FORMAT7_5,
  DC1394_VIDEO_MODE_FORMAT7_6,
  DC1394_VIDEO_MODE_FORMAT7_7
} dc1394video_mode_t;
#define DC1394_VIDEO_MODE_MIN	    DC1394_VIDEO_MODE_160x120_YUV444
#define DC1394_VIDEO_MODE_MAX       DC1394_VIDEO_MODE_FORMAT7_7
#define DC1394_VIDEO_MODE_NUM      (DC1394_VIDEO_MODE_MAX - DC1394_VIDEO_MODE_MIN + 1)

// Special min/max are defined for Format_7
#define DC1394_VIDEO_MODE_FORMAT7_MIN       DC1394_VIDEO_MODE_FORMAT7_0
#define DC1394_VIDEO_MODE_FORMAT7_MAX       DC1394_VIDEO_MODE_FORMAT7_7
#define DC1394_VIDEO_MODE_FORMAT7_NUM      (DC1394_VIDEO_MODE_FORMAT7_MAX - DC1394_VIDEO_MODE_FORMAT7_MIN + 1)

/* Enumeration of colour modings */
typedef enum {
  DC1394_COLOR_CODING_MONO8= 320,
  DC1394_COLOR_CODING_YUV411,
  DC1394_COLOR_CODING_YUV422,
  DC1394_COLOR_CODING_YUV444,
  DC1394_COLOR_CODING_RGB8,
  DC1394_COLOR_CODING_MONO16,
  DC1394_COLOR_CODING_RGB16,
  DC1394_COLOR_CODING_MONO16S,
  DC1394_COLOR_CODING_RGB16S,
  DC1394_COLOR_CODING_RAW8,
  DC1394_COLOR_CODING_RAW16
} dc1394color_coding_t;
#define DC1394_COLOR_CODING_MIN     DC1394_COLOR_CODING_MONO8
#define DC1394_COLOR_CODING_MAX     DC1394_COLOR_CODING_RAW16
#define DC1394_COLOR_CODING_NUM    (DC1394_COLOR_CODING_MAX - DC1394_COLOR_CODING_MIN + 1)

/* Enumeration of trigger modes */
typedef enum {
  DC1394_TRIGGER_MODE_0= 352,
  DC1394_TRIGGER_MODE_1,
  DC1394_TRIGGER_MODE_2,
  DC1394_TRIGGER_MODE_3,
  DC1394_TRIGGER_MODE_4,
  DC1394_TRIGGER_MODE_5,
  DC1394_TRIGGER_MODE_14,
  DC1394_TRIGGER_MODE_15
} dc1394trigger_mode_t;
#define DC1394_TRIGGER_MODE_MIN     DC1394_TRIGGER_MODE_0
#define DC1394_TRIGGER_MODE_MAX     DC1394_TRIGGER_MODE_15
#define DC1394_TRIGGER_MODE_NUM    (DC1394_TRIGGER_MODE_MAX - DC1394_TRIGGER_MODE_MIN + 1)

/* Enumeration of camera features */
typedef enum {
  DC1394_FEATURE_BRIGHTNESS= 416,
  DC1394_FEATURE_EXPOSURE,
  DC1394_FEATURE_SHARPNESS,
  DC1394_FEATURE_WHITE_BALANCE,
  DC1394_FEATURE_HUE,
  DC1394_FEATURE_SATURATION,
  DC1394_FEATURE_GAMMA,
  DC1394_FEATURE_SHUTTER,
  DC1394_FEATURE_GAIN,
  DC1394_FEATURE_IRIS,
  DC1394_FEATURE_FOCUS,
  DC1394_FEATURE_TEMPERATURE,
  DC1394_FEATURE_TRIGGER,
  DC1394_FEATURE_TRIGGER_DELAY,
  DC1394_FEATURE_WHITE_SHADING,
  DC1394_FEATURE_FRAME_RATE,
  /* 16 reserved features */
  DC1394_FEATURE_ZOOM,
  DC1394_FEATURE_PAN,
  DC1394_FEATURE_TILT,
  DC1394_FEATURE_OPTICAL_FILTER,
  /* 12 reserved features */
  DC1394_FEATURE_CAPTURE_SIZE,
  DC1394_FEATURE_CAPTURE_QUALITY
  /* 14 reserved features */
} dc1394feature_t;
#define DC1394_FEATURE_MIN           DC1394_FEATURE_BRIGHTNESS
#define DC1394_FEATURE_MAX           DC1394_FEATURE_CAPTURE_QUALITY
#define DC1394_FEATURE_NUM          (DC1394_FEATURE_MAX - DC1394_FEATURE_MIN + 1)

/* Operation modes */
typedef enum {
  DC1394_OPERATION_MODE_LEGACY = 480,
  DC1394_OPERATION_MODE_1394B
} dc1394operation_mode_t;

/* Format 7 sensor layouts*/
typedef enum {
  DC1394_COLOR_FILTER_RGGB = 512,
  DC1394_COLOR_FILTER_GBRG,
  DC1394_COLOR_FILTER_GRBG,
  DC1394_COLOR_FILTER_BGGR
} dc1394color_filter_t;
#define DC1394_COLOR_FILTER_MIN        DC1394_COLOR_FILTER_RGGB
#define DC1394_COLOR_FILTER_MAX        DC1394_COLOR_FILTER_BGGR
#define DC1394_COLOR_FILTER_NUM       (DC1394_COLOR_FILTER_MAX - DC1394_COLOR_FILTER_MIN + 1)

/* IIDC versions*/
typedef enum {
  DC1394_IIDC_VERSION_1_04 = 544,
  DC1394_IIDC_VERSION_1_20,
  DC1394_IIDC_VERSION_PTGREY,
  DC1394_IIDC_VERSION_1_30,
  DC1394_IIDC_VERSION_1_31,
  DC1394_IIDC_VERSION_1_32,
  DC1394_IIDC_VERSION_1_33,
  DC1394_IIDC_VERSION_1_34,
  DC1394_IIDC_VERSION_1_35,
  DC1394_IIDC_VERSION_1_36,
  DC1394_IIDC_VERSION_1_37,
  DC1394_IIDC_VERSION_1_38,
  DC1394_IIDC_VERSION_1_39
} dc1394iidc_version_t;
#define DC1394_IIDC_VERSION_MIN        DC1394_IIDC_VERSION_1_04
#define DC1394_IIDC_VERSION_MAX        DC1394_IIDC_VERSION_1_39
#define DC1394_IIDC_VERSION_NUM       (DC1394_IIDC_VERSION_MAX - DC1394_IIDC_VERSION_MIN + 1)

/* Enumeration of trigger sources */
typedef enum {
  DC1394_TRIGGER_SOURCE_0= 576,
  DC1394_TRIGGER_SOURCE_1,
  DC1394_TRIGGER_SOURCE_2,
  DC1394_TRIGGER_SOURCE_3,
} dc1394trigger_source_t;
#define DC1394_TRIGGER_SOURCE_MIN     DC1394_TRIGGER_SOURCE_0
#define DC1394_TRIGGER_SOURCE_MAX     DC1394_TRIGGER_SOURCE_3
#define DC1394_TRIGGER_SOURCE_NUM    (DC1394_TRIGGER_SOURCE_MAX - DC1394_TRIGGER_SOURCE_MIN + 1)

/* Maximum number of characters in vendor and model strings */
#define MAX_CHARS                      256

/* Return values for visible functions*/
typedef enum {
  DC1394_SUCCESS=0,   /* Success is zero */
  DC1394_FAILURE,     /* Errors are positive numbers */
  DC1394_NO_FRAME=-2, /* Warnings or info are negative numbers*/
  DC1394_NO_CAMERA=3,
  DC1394_NOT_A_CAMERA,
  DC1394_FUNCTION_NOT_SUPPORTED,
  DC1394_CAMERA_NOT_INITITIALIZED,
  DC1394_INVALID_FEATURE,
  DC1394_INVALID_FORMAT,
  DC1394_INVALID_MODE,
  DC1394_INVALID_FRAMERATE,
  DC1394_INVALID_TRIGGER_MODE,
  DC1394_INVALID_TRIGGER_SOURCE,
  DC1394_INVALID_ISO_SPEED,
  DC1394_INVALID_IIDC_VERSION,
  DC1394_INVALID_COLOR_CODING,
  DC1394_INVALID_COLOR_FILTER,
  DC1394_REQ_VALUE_OUTSIDE_RANGE,
  DC1394_INVALID_ERROR_CODE,
  DC1394_MEMORY_ALLOCATION_FAILURE,
  DC1394_TAGGED_REGISTER_NOT_FOUND,
  DC1394_FORMAT7_ERROR_FLAG_1,
  DC1394_FORMAT7_ERROR_FLAG_2,
  DC1394_INVALID_BAYER_METHOD,
  DC1394_HANDLE_CREATION_FAILURE,
  DC1394_GENERIC_INVALID_ARGUMENT,
  DC1394_INVALID_VIDEO1394_DEVICE,
  DC1394_NO_ISO_CHANNEL,
  DC1394_NO_BANDWIDTH
} dc1394error_t;
#define DC1394_ERROR_NUM DC1394_NO_BANDWIDTH+1

/* Parameter flags for dc1394_setup_format7_capture() */
#define DC1394_QUERY_FROM_CAMERA -1
#define DC1394_USE_MAX_AVAIL     -2
#define DC1394_USE_RECOMMENDED   -3

/* The video1394 policy: blocking (wait for a frame forever)
   or polling (returns if no frames in buffer) */
typedef enum { 
  DC1394_VIDEO1394_WAIT=0,
  DC1394_VIDEO1394_POLL
} dc1394video_policy_t;

/* Yet another boolean data type */
typedef enum {
  DC1394_FALSE= 0,
  DC1394_TRUE
} dc1394bool_t;

/* Yet another boolean data type */
typedef enum {
  DC1394_OFF= 0,
  DC1394_ON
} dc1394switch_t;

typedef enum {
  DC1394_TRIGGER_ACTIVE_LOW= 0,
  DC1394_TRIGGER_ACTIVE_HIGH
} dc1394trigger_polarity_t;

typedef enum {
  DC1394_FEATURE_MODE_MANUAL= 0,
  DC1394_FEATURE_MODE_AUTO,
  DC1394_FEATURE_MODE_ONE_PUSH_AUTO
} dc1394feature_mode_t;


/* Camera capture structure. Do not access directly from your application.
   Use dc1394_video_get_buffer and dc1394_video_get_fill_time instead. */

typedef struct
{
  uint_t                  num;
  dc1394color_coding_t    codings[DC1394_COLOR_CODING_NUM];
} dc1394color_codings_t;

typedef struct
{
  uint_t                  num;
  dc1394video_mode_t      modes[DC1394_VIDEO_MODE_NUM];
} dc1394video_modes_t;

typedef struct
{
  uint_t                  num;
  dc1394framerate_t       framerates[DC1394_FRAMERATE_NUM];
} dc1394framerates_t;

typedef struct
{
  uint_t                  num;
  dc1394trigger_mode_t    modes[DC1394_TRIGGER_MODE_NUM];
} dc1394trigger_modes_t;

typedef struct
{
  uint_t                    num;
  dc1394trigger_source_t    sources[DC1394_TRIGGER_SOURCE_NUM];
} dc1394trigger_sources_t;

/* Camera structure */
typedef struct __dc1394_camera
{
  // system/firmware information
  int                port;
  nodeid_t           node;
  uint64_t           euid_64;
  quadlet_t          ud_reg_tag_12;
  quadlet_t          ud_reg_tag_13;
  octlet_t           command_registers_base;
  octlet_t           unit_directory;
  octlet_t           unit_dependent_directory;
  octlet_t           advanced_features_csr;
  octlet_t           absolute_control_csr;
  octlet_t           format7_csr[DC1394_VIDEO_MODE_FORMAT7_NUM];
  dc1394iidc_version_t iidc_version;
  char               vendor[MAX_CHARS + 1];
  char               model[MAX_CHARS + 1];
  quadlet_t          vendor_id;
  quadlet_t          model_id;
  dc1394bool_t       bmode_capable;
  dc1394bool_t       one_shot_capable;
  dc1394bool_t       multi_shot_capable;
  dc1394bool_t       adv_features_capable;
  dc1394bool_t       can_switch_on_off;

  // some current values
  dc1394video_mode_t video_mode;
  dc1394framerate_t  framerate;
  dc1394switch_t     is_iso_on;
  int                iso_channel; // this variable contains the iso channel requests or the current iso channel
  int                iso_channel_is_set; // >0 if the iso_channel above has been allocated within libraw1394
  uint_t             iso_bandwidth;
  dc1394speed_t      iso_speed;
  uint_t             mem_channel_number;
  uint_t             save_channel;
  uint_t             load_channel;

  int                capture_is_set; // 0 for not set, 1 for RAW1394 and 2 for DMA

  // for broadcast:
  dc1394bool_t       broadcast;
  nodeid_t           node_id_backup;

  // bus information
  

} dc1394camera_t;

typedef struct __dc1394feature_info_t_struct 
{
  dc1394feature_t    id;
  dc1394bool_t       available;
  dc1394bool_t       one_push;
  dc1394bool_t       absolute_capable;
  dc1394bool_t       readout_capable;
  dc1394bool_t       on_off_capable;
  dc1394bool_t       auto_capable;
  dc1394bool_t       manual_capable;
  dc1394bool_t       polarity_capable;
  dc1394switch_t     one_push_active;
  dc1394switch_t     is_on;
  dc1394bool_t       auto_active;
  dc1394trigger_modes_t    trigger_modes;
  dc1394trigger_mode_t     trigger_mode;
  dc1394trigger_polarity_t trigger_polarity;
  dc1394trigger_sources_t  trigger_sources;
  dc1394trigger_source_t   trigger_source;
  uint_t             min;
  uint_t             max;
  uint_t             value;
  uint_t             BU_value;
  uint_t             RV_value;
  uint_t             B_value;
  uint_t             R_value;
  uint_t             G_value;
  uint_t             target_value;
  
  dc1394switch_t     abs_control;
  float              abs_value;
  float              abs_max;
  float              abs_min;
  
} dc1394feature_info_t;

typedef struct __dc1394featureset_t
{
  dc1394feature_info_t    feature[DC1394_FEATURE_NUM];
} dc1394featureset_t;

typedef struct __dc1394format7mode_t
{
  dc1394bool_t present;

  unsigned int size_x;
  unsigned int size_y;
  unsigned int max_size_x;
  unsigned int max_size_y;

  unsigned int pos_x;
  unsigned int pos_y;

  unsigned int unit_size_x;
  unsigned int unit_size_y;
  unsigned int unit_pos_x;
  unsigned int unit_pos_y;

  dc1394color_codings_t color_codings;
  dc1394color_coding_t color_coding;

  unsigned int pixnum;

  unsigned int bpp; // bpp is byte_per_packet, not bit per pixel.
  unsigned int min_bpp;
  unsigned int max_bpp;

  unsigned long long int total_bytes;

  dc1394color_filter_t color_filter;

} dc1394format7mode_t;

typedef struct __dc1394format7modeset_t
{
  dc1394format7mode_t mode[DC1394_VIDEO_MODE_FORMAT7_NUM];
} dc1394format7modeset_t;

/* Feature descriptions */
extern const char *dc1394_feature_desc[DC1394_FEATURE_NUM];
extern const char *dc1394_error_strings[DC1394_ERROR_NUM];

/* Error checking function. displays an error string on stderr and exit current function
   if error is positive. Neg errors are messages and are thus ignored */

#define DC1394_WRN(err, err_string...)                       \
    {                                                        \
    if ((err<0)||(err>DC1394_ERROR_NUM))                     \
      err=DC1394_INVALID_ERROR_CODE;                         \
                                                             \
    if (err>DC1394_SUCCESS) {                                \
      fprintf(stderr,"Libdc1394 warning (%s:%s:%d): %s : ",  \
	      __FILE__, __FUNCTION__, __LINE__,              \
	      dc1394_error_strings[err]);                    \
      fprintf(stderr, err_string);                           \
      fprintf(stderr,"\n");                                  \
    }                                                        \
    }

#define DC1394_ERR(err, err_string...)                       \
    {                                                        \
    if ((err<0)||(err>DC1394_ERROR_NUM))                     \
      err=DC1394_INVALID_ERROR_CODE;                         \
                                                             \
    if (err>DC1394_SUCCESS) {                                \
      fprintf(stderr,"Libdc1394 error (%s:%s:%d): %s : ",    \
	      __FILE__, __FUNCTION__, __LINE__,              \
	      dc1394_error_strings[err]);                    \
      fprintf(stderr, err_string);                           \
      fprintf(stderr,"\n");                                  \
      return;                                                \
    }                                                        \
    }

#define DC1394_ERR_RTN(err, err_string...)                   \
    {                                                        \
    if ((err<0)||(err>DC1394_ERROR_NUM))                     \
      err=DC1394_INVALID_ERROR_CODE;                         \
                                                             \
    if (err>DC1394_SUCCESS) {                                \
      fprintf(stderr,"Libdc1394 error (%s:%s:%d): %s : ",    \
	      __FILE__, __FUNCTION__, __LINE__,              \
	      dc1394_error_strings[err]);                    \
      fprintf(stderr, err_string);                           \
      fprintf(stderr,"\n");                                  \
      return err;                                            \
    }                                                        \
    }

#define DC1394_ERR_CLN(err, cleanup, err_string...)                  \
    {                                                                \
    if ((err<0)||(err>DC1394_ERROR_NUM))                             \
      err=DC1394_INVALID_ERROR_CODE;                                 \
                                                                     \
    if (err>DC1394_SUCCESS) {                                        \
      fprintf(stderr,"Libdc1394 error (%s:%s:%d): %s : ",            \
	      __FILE__, __FUNCTION__, __LINE__,                      \
	      dc1394_error_strings[err]);                            \
      fprintf(stderr, err_string);                                   \
      fprintf(stderr,"\n");                                          \
      cleanup;                                                       \
      return;                                                \
    }                                                                \
    }

#define DC1394_ERR_CLN_RTN(err, cleanup, err_string...)              \
    {                                                                \
    if ((err<0)||(err>DC1394_ERROR_NUM))                             \
      err=DC1394_INVALID_ERROR_CODE;                                 \
                                                                     \
    if (err>DC1394_SUCCESS) {                                        \
      fprintf(stderr,"Libdc1394 error (%s:%s:%d): %s : ",            \
	      __FILE__, __FUNCTION__, __LINE__,                      \
	      dc1394_error_strings[err]);                            \
      fprintf(stderr, err_string);                                   \
      fprintf(stderr,"\n");                                          \
      cleanup;                                                       \
      return err;                                                    \
    }                                                                \
    }

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************
     General system functions
 ***************************************************************************/

/* create / free camera structure */
//dc1394camera_t* dc1394_new_camera(uint_t port, nodeid_t node);
void dc1394_free_camera(dc1394camera_t *camera);

/* locate and initialise the cameras */
dc1394error_t dc1394_find_cameras(dc1394camera_t ***cameras_ptr, uint_t* numCameras);

/* print the camera information */
//dc1394error_t dc1394_get_camera_info(dc1394camera_t *camera);
dc1394error_t dc1394_print_camera_info(dc1394camera_t *camera);

/* Sets and resets the broadcast flag of a camera. If the broadcast flag is set,
   all devices on the bus will execute the command. Useful to sync ISO start
   commands or setting a bunch of cameras at the same time. Broadcast only works
   with identical devices (brand/model). If the devices are not identical your
   mileage may vary. Some cameras may not answer broadcast commands at all. Also,
   this only works with cameras on the SAME bus (IOW, the same port).*/
dc1394error_t dc1394_camera_set_broadcast(dc1394camera_t *camera, dc1394bool_t pwr);

/***************************************************************************
     Other functionalities
 ***************************************************************************/

/* reset camera to factory default settings */
dc1394error_t dc1394_reset_camera(dc1394camera_t *camera);

/* turn camera on or off */
dc1394error_t dc1394_set_camera_power(dc1394camera_t *camera, dc1394switch_t pwr);

/* functions to read and write camera setups in memory channels */
dc1394error_t dc1394_memory_get_load_ch(dc1394camera_t *camera, uint_t *channel);
dc1394error_t dc1394_memory_get_save_ch(dc1394camera_t *camera, uint_t *channel);
dc1394error_t dc1394_memory_is_save_in_operation(dc1394camera_t *camera, dc1394bool_t *value);
dc1394error_t dc1394_memory_save(dc1394camera_t *camera, uint_t channel);
dc1394error_t dc1394_memory_load(dc1394camera_t *camera, uint_t channel);

/***************************************************************************
     Trigger
 ***************************************************************************/

/* external trigger feature functions */
dc1394error_t dc1394_external_trigger_set_polarity(dc1394camera_t *camera, dc1394trigger_polarity_t polarity);
dc1394error_t dc1394_external_trigger_get_polarity(dc1394camera_t *camera, dc1394trigger_polarity_t *polarity);
dc1394error_t dc1394_external_trigger_has_polarity(dc1394camera_t *camera, dc1394bool_t *polarity_capable);
dc1394error_t dc1394_external_trigger_set_power(dc1394camera_t *camera, dc1394switch_t pwr);
dc1394error_t dc1394_external_trigger_get_power(dc1394camera_t *camera, dc1394switch_t *pwr);
dc1394error_t dc1394_external_trigger_set_mode(dc1394camera_t *camera, dc1394trigger_mode_t mode);
dc1394error_t dc1394_external_trigger_get_mode(dc1394camera_t *camera, dc1394trigger_mode_t *mode);
dc1394error_t dc1394_external_trigger_set_source(dc1394camera_t *camera, dc1394trigger_source_t source);
dc1394error_t dc1394_external_trigger_get_source(dc1394camera_t *camera, dc1394trigger_source_t *source);
dc1394error_t dc1394_external_trigger_get_supported_sources(dc1394camera_t *camera, dc1394trigger_sources_t *sources);

/* Turn one software trigger on or off and get state */
dc1394error_t dc1394_software_trigger_set_power(dc1394camera_t *camera, dc1394switch_t pwr);
dc1394error_t dc1394_software_trigger_get_power(dc1394camera_t *camera, dc1394switch_t *pwr);

/***************************************************************************
     Features
 ***************************************************************************/

/* Collects the available features for the camera described by node and stores them in features. */
dc1394error_t dc1394_get_camera_feature_set(dc1394camera_t *camera, dc1394featureset_t *features);

/* Stores the bounds and options associated with the feature described by feature->feature_id */
dc1394error_t dc1394_get_camera_feature(dc1394camera_t *camera, dc1394feature_info_t *feature);

/* Displays the bounds and options of the given feature or of the entire feature set */
dc1394error_t dc1394_print_feature(dc1394feature_info_t *feature);
dc1394error_t dc1394_print_feature_set(dc1394featureset_t *features);

/* get/set the values of multiple-value features on the camera */
dc1394error_t dc1394_feature_whitebalance_get_value(dc1394camera_t *camera, uint_t *u_b_value, uint_t *v_r_value);
dc1394error_t dc1394_feature_whitebalance_set_value(dc1394camera_t *camera, uint_t u_b_value, uint_t v_r_value);
dc1394error_t dc1394_feature_temperature_get_value(dc1394camera_t *camera, uint_t *target_temperature, uint_t *temperature);
dc1394error_t dc1394_feature_temperature_set_value(dc1394camera_t *camera, uint_t target_temperature);
dc1394error_t dc1394_feature_whiteshading_get_value(dc1394camera_t *camera, uint_t *r_value, uint_t *g_value, uint_t *b_value);
dc1394error_t dc1394_feature_whiteshading_set_value(dc1394camera_t *camera, uint_t r_value, uint_t g_value, uint_t b_value);

/* get/set the values of single-value features on the camera */
dc1394error_t dc1394_feature_get_value(dc1394camera_t *camera, dc1394feature_t feature, uint_t *value);
dc1394error_t dc1394_feature_set_value(dc1394camera_t *camera, dc1394feature_t feature, uint_t value);

/* query/set specific feature characteristics */
dc1394error_t dc1394_feature_is_present(dc1394camera_t *camera, dc1394feature_t feature, dc1394bool_t *value);
dc1394error_t dc1394_feature_is_readable(dc1394camera_t *camera, dc1394feature_t feature, dc1394bool_t *value);
dc1394error_t dc1394_feature_get_boundaries(dc1394camera_t *camera, dc1394feature_t feature, uint_t *min, uint_t *max);
dc1394error_t dc1394_feature_is_switchable(dc1394camera_t *camera, dc1394feature_t feature, dc1394bool_t *value);
dc1394error_t dc1394_feature_get_power(dc1394camera_t *camera, dc1394feature_t feature, dc1394switch_t *pwr);
dc1394error_t dc1394_feature_set_power(dc1394camera_t *camera, dc1394feature_t feature, dc1394switch_t pwr);
dc1394error_t dc1394_feature_has_auto_mode(dc1394camera_t *camera,  dc1394feature_t feature, dc1394bool_t *value);
dc1394error_t dc1394_feature_has_manual_mode(dc1394camera_t *camera, dc1394feature_t feature, dc1394bool_t *value);
dc1394error_t dc1394_feature_has_one_push_auto(dc1394camera_t *camera, dc1394feature_t feature, dc1394bool_t *value);
dc1394error_t dc1394_feature_get_mode(dc1394camera_t *camera, dc1394feature_t feature, dc1394feature_mode_t *mode);
dc1394error_t dc1394_feature_set_mode(dc1394camera_t *camera, dc1394feature_t feature, dc1394feature_mode_t mode);

/* absolute settings */
dc1394error_t dc1394_feature_has_absolute_control(dc1394camera_t *camera, dc1394feature_t feature, dc1394bool_t *value);
dc1394error_t dc1394_feature_get_absolute_boundaries(dc1394camera_t *camera, dc1394feature_t feature, float *min, float *max);
dc1394error_t dc1394_feature_get_absolute_value(dc1394camera_t *camera, dc1394feature_t feature, float *value);
dc1394error_t dc1394_feature_set_absolute_value(dc1394camera_t *camera, dc1394feature_t feature, float value);
dc1394error_t dc1394_feature_get_absolute_control(dc1394camera_t *camera, dc1394feature_t feature, dc1394switch_t *pwr);
dc1394error_t dc1394_feature_set_absolute_control(dc1394camera_t *camera, dc1394feature_t feature, dc1394switch_t pwr);

/***************************************************************************
     Video functions: formats, framerates,...
 ***************************************************************************/

/* functions for querying camera attributes */
dc1394error_t dc1394_video_get_supported_modes(dc1394camera_t *camera, dc1394video_modes_t *video_modes);
dc1394error_t dc1394_video_get_supported_framerates(dc1394camera_t *camera, dc1394video_mode_t video_mode, dc1394framerates_t *framerates);

/* get/set the framerate, mode, format, iso speed for the video */
dc1394error_t dc1394_video_get_framerate(dc1394camera_t *camera, dc1394framerate_t *framerate);
dc1394error_t dc1394_video_set_framerate(dc1394camera_t *camera, dc1394framerate_t framerate);
dc1394error_t dc1394_video_get_mode(dc1394camera_t *camera, dc1394video_mode_t *video_mode);
dc1394error_t dc1394_video_set_mode(dc1394camera_t *camera, dc1394video_mode_t video_mode);
dc1394error_t dc1394_video_get_operation_mode(dc1394camera_t *camera, dc1394operation_mode_t *mode);
dc1394error_t dc1394_video_set_operation_mode(dc1394camera_t *camera, dc1394operation_mode_t mode);
dc1394error_t dc1394_video_get_iso_speed(dc1394camera_t *camera, dc1394speed_t *speed);
dc1394error_t dc1394_video_set_iso_speed(dc1394camera_t *camera, dc1394speed_t speed);
dc1394error_t dc1394_video_get_data_depth(dc1394camera_t *camera, uint_t *depth);
 
/* start/stop isochronous data transmission */
dc1394error_t dc1394_video_set_transmission(dc1394camera_t *camera, dc1394switch_t pwr);
dc1394error_t dc1394_video_get_transmission(dc1394camera_t *camera, dc1394switch_t *pwr);
/* The following function is not necessary in general. You should only use it if you
   want a specific ISO channel. Usage: Call it before setting up capture and transmission */
dc1394error_t dc1394_video_specify_iso_channel(dc1394camera_t *camera, int iso_channel);

/* turn one shot mode on or off */
dc1394error_t dc1394_video_set_one_shot(dc1394camera_t *camera, dc1394switch_t pwr);
dc1394error_t dc1394_video_get_one_shot(dc1394camera_t *camera, dc1394bool_t *is_on);

/* turn multishot mode on or off */
dc1394error_t dc1394_video_set_multi_shot(dc1394camera_t *camera, uint_t numFrames, dc1394switch_t pwr);
dc1394error_t dc1394_video_get_multi_shot(dc1394camera_t *camera, dc1394bool_t *is_on, uint_t *numFrames);

/* Utility function */
dc1394error_t dc1394_video_get_bandwidth_usage(dc1394camera_t *camera, uint_t *bandwidth);

/***************************************************************************
     Capture Functions
 ***************************************************************************/

/* setup the capture (DMA or RAW1394)*/
dc1394error_t dc1394_capture_setup_dma(dc1394camera_t *camera, uint_t num_dma_buffers, uint_t drop_frames);
dc1394error_t dc1394_capture_setup(dc1394camera_t *camera);

/* capture video frames (DMA or RAW1394)*/
dc1394error_t dc1394_capture_dma(dc1394camera_t **camera, uint_t num, dc1394video_policy_t policy);
dc1394error_t dc1394_capture(dc1394camera_t **camera, uint_t num);

/* releases memory, channels, bandwidth,... */
dc1394error_t dc1394_capture_stop(dc1394camera_t *camera);

/* DMA specific functions*/

/* Set the DMA device filename manually. In most cases this is not necessary because the capture
   functions probe common filenames such as /dev/video1394/x or /dev/video1394. */
dc1394error_t dc1394_capture_set_dma_device_filename(dc1394camera_t* camera, char *filename);
/* returns the buffer previously handed to the user by dc1394_dma_*_capture to the DMA ring buffer */
dc1394error_t dc1394_capture_dma_done_with_buffer(dc1394camera_t *camera);
/* Functions for accessing the buffer content: */
uchar_t*        dc1394_capture_get_dma_buffer(dc1394camera_t *camera);
struct timeval* dc1394_capture_get_dma_filltime(dc1394camera_t *camera);
uint_t          dc1394_capture_get_width(dc1394camera_t *camera);
uint_t          dc1394_capture_get_height(dc1394camera_t *camera);
uint_t          dc1394_capture_get_height(dc1394camera_t *camera);
uint_t          dc1394_capture_get_bytes_per_frame(dc1394camera_t *camera);


/***************************************************************************
     Format_7 (scalable image format)
 ***************************************************************************/

/* image size */
dc1394error_t dc1394_format7_get_max_image_size(dc1394camera_t *camera, dc1394video_mode_t video_mode, uint_t *h_size,uint_t *v_size);
dc1394error_t dc1394_format7_get_unit_size(dc1394camera_t *camera, dc1394video_mode_t video_mode, uint_t *h_unit, uint_t *v_unit);
dc1394error_t dc1394_format7_get_image_size(dc1394camera_t *camera, dc1394video_mode_t video_mode, uint_t *width, uint_t *height);
dc1394error_t dc1394_format7_set_image_size(dc1394camera_t *camera, dc1394video_mode_t video_mode, uint_t width, uint_t height);

/* image position */
dc1394error_t dc1394_format7_get_image_position(dc1394camera_t *camera, dc1394video_mode_t video_mode, uint_t *left, uint_t *top);
dc1394error_t dc1394_format7_set_image_position(dc1394camera_t *camera, dc1394video_mode_t video_mode, uint_t left, uint_t top);
dc1394error_t dc1394_format7_get_unit_position(dc1394camera_t *camera, dc1394video_mode_t video_mode, uint_t *h_unit_pos, uint_t *v_unit_pos);

/* color coding */
dc1394error_t dc1394_format7_get_color_coding(dc1394camera_t *camera, dc1394video_mode_t video_mode, dc1394color_coding_t *color_coding);
dc1394error_t dc1394_format7_get_color_codings(dc1394camera_t *camera, dc1394video_mode_t video_mode, dc1394color_codings_t *codings);
dc1394error_t dc1394_format7_set_color_coding(dc1394camera_t *camera, dc1394video_mode_t video_mode, dc1394color_coding_t color_coding);
dc1394error_t dc1394_format7_set_color_filter(dc1394camera_t *camera, dc1394video_mode_t video_mode, dc1394color_filter_t color_filter);
dc1394error_t dc1394_format7_get_color_filter(dc1394camera_t *camera, dc1394video_mode_t video_mode, dc1394color_filter_t *color_filter);

/* packet */
dc1394error_t dc1394_format7_get_packet_para(dc1394camera_t *camera, dc1394video_mode_t video_mode, uint_t *min_bytes, uint_t *max_bytes);
dc1394error_t dc1394_format7_get_byte_per_packet(dc1394camera_t *camera, dc1394video_mode_t video_mode, uint_t *packet_bytes);
dc1394error_t dc1394_format7_set_byte_per_packet(dc1394camera_t *camera, dc1394video_mode_t video_mode, uint_t packet_bytes);
dc1394error_t dc1394_format7_get_recommended_byte_per_packet(dc1394camera_t *camera, dc1394video_mode_t video_mode, uint_t *bpp);
dc1394error_t dc1394_format7_get_packet_per_frame(dc1394camera_t *camera, dc1394video_mode_t video_mode, uint_t *ppf);

/* other */
dc1394error_t dc1394_format7_get_data_depth(dc1394camera_t *camera, dc1394video_mode_t video_mode, uint_t *data_depth);
dc1394error_t dc1394_format7_get_frame_interval(dc1394camera_t *camera, dc1394video_mode_t video_mode, float *interval);
dc1394error_t dc1394_format7_get_pixel_number(dc1394camera_t *camera, dc1394video_mode_t video_mode, uint_t *pixnum);
dc1394error_t dc1394_format7_get_total_bytes(dc1394camera_t *camera, dc1394video_mode_t video_mode, uint64_t *total_bytes);

/* These functions get the properties of (one or all) format7 mode(s) */
dc1394error_t dc1394_format7_get_modeset(dc1394camera_t *camera, dc1394format7modeset_t *info);
dc1394error_t dc1394_format7_get_mode_info(dc1394camera_t *camera, dc1394video_mode_t video_mode, dc1394format7mode_t *f7_mode);

/* Joint function that fully sets a certain ROI taking all parameters into account
   Note that this function does not SWITCH to the video mode passed as argument, it mearly sets it */
dc1394error_t dc1394_format7_set_roi(dc1394camera_t *camera, dc1394video_mode_t video_mode, dc1394color_coding_t color_coding,
				     int bytes_per_packet, int left, int top, int width, int height);


/* This will have to be fixed or removed: it's ugly...*/
dc1394error_t dc1394_cleanup_iso_channels_and_bandwidth(dc1394camera_t *camera);

#ifdef __cplusplus
}
#endif

#endif /* __DC1394_CAMERA_CONTROL_H__ */
