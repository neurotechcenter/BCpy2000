/*------------------------------------------------------*/
/*   client-types.h                                     */
/* VTI driver enums used as VTI message argcode         */
/* This is a machine-generated file.                    */
/* Modify the original to add new features.             */
/*------------------------------------------------------*/

/* FILE:drivers/glove/glove-message.h */
/* To configure CyberGlove */
#define CG_CONFIG_FILTER 0
#define CG_CONFIG_MASK   1
#define CG_CONFIG_UNIT   2
#define CG_CONFIG_KEYS   3


/* FILE:drivers/glove/glove-message.h */
enum {CG_REPORT_DEFAULT          = 0,  
      CG_REPORT_RAW              = 1, 
      CG_REPORT_VH_JOINT_ANGLE   = 2, 
      CG_REPORT_JOINT_ANGLE      = 3, 
      CG_REPORT_JOINT_POSITION   = 4, 
      CG_REPORT_LOCAL_XFORM      = 5, 
      CG_REPORT_GLOBAL_XFORM     = 6, 
      CG_REPORT_SWITCH           = 7,
      CG_REPORT_TOUCH            = 8,
      CG_REPORT_KEYS
};


/* FILE:drivers/glove/glove-message.h */
enum {CG_CALIBRATION_DEFAULT, 
      CG_CALIBRATION_GAIN_OFFSET,
      CG_CALIBRATION_HAND_GEOM, 
      CG_CALIBRATION_MEASUREMENT, 
      CG_CALIBRATION_KEYS};


/* FILE:drivers/glove/glove-message.h */
enum {CG_CONTROL_DEFAULT, 
      CG_CONTROL_TOUCH, 
      CG_CONTROL_LIGHT, 
      CG_CONTROL_KEYS};


/* FILE:drivers/glove/glove-message.h */
enum {CG_EVENT_DEFAULT, 
      CG_EVENT_RAW_CHANGE, 
      CG_EVENT_CALIBRATION_CHANGE,
      CG_EVENT_TOUCH_CHANGE,
      CG_EVENT_SWITCH_CHANGE, 
      CG_EVENT_CONDITION,	/* meet certain condition (e.g. joint angle) */
      CG_EVENT_ERROR, 
      CG_EVENT_KEYS};


/* FILE:drivers/glove/glove-types.h */
enum { CG_THUMB, CG_INDEX, CG_MIDDLE, CG_RING, CG_PINKIE, CG_CARPUS, 
       CG_PALM = CG_CARPUS, CG_FINGERS = 5 };
/* these names are obsolete and for access raw data only */
enum { CG_MCP=0, CG_PIP, CG_DIP, CG_ABDUCT, CG_RAW_JOINTS, 
	 CG_CARPAL0 = 0, CG_WRIST_PITCH, CG_WRIST_YAW, CG_CARPALS };
enum { CG_J0 = 0, 
       CG_J1, 
       CG_J2, 
       CG_J3, 
       CG_J4, 
       CG_FINGER_POINTS, 
       CG_JOINTS = CG_J3}; /* J1: MCP, J2: PIP, J3:DIP, J4:tip */

#define CG_ACTUATORS 6
typedef struct GLOVEINFO_t {
    vt_bool      bRightHand;	/* flag: is the glove for right-hand */
    vt_bool      bTouch;	/* flag: support touch option, yes no */
    vt_bool      bSwitch;	/* flag: wrist switch on/off */
} GLOVEINFO;


/* FILE:drivers/grasp/grasp-message.h */
#ifndef GR_TYPES_DEFINED
#define GR_TYPES_DEFINED
enum
{
    GR_INFO_MODE = 1,
};

enum
{
    GR_CONFIG_DEFAULT = 0,
    GR_CONFIG_GLOVE,
    GR_CONFIG_TRACKER
};


/* FILE:drivers/grasp/grasp-message.h */
/* opmode in reply msg arkcode */
enum {
    GR_REPORT_DEFAULT       = 0,
    GR_REPORT_FORCE,			
    GR_REPORT_IMPEDENCE,
    GR_REPORT_MAX_FORCE,		/* maximum force */
    GR_REPORT_ENCODER,
    GR_REPORT_KEYS
};


/* FILE:drivers/grasp/grasp-message.h */
enum {
    GR_CONTROL_DEFAULT      = 0,
    GR_CONTROL_FORCE,
    GR_CONTROL_IMPEDENCE,
    GR_CONTROL_IDLE,
    GR_CONTROL_REWIND,
    GR_CONTROL_EFFECT,
    GR_CONTROL_RESET,
    GR_CONTROL_KEYS
};


/* FILE:drivers/grasp/grasp-message.h */
/* calibrate strings */
enum {
    GR_CALIBRATION_DEFAULT      = 0,
    GR_CALIBRATION_E2G_MAP      = 1, /* encoder to glove map */
    GR_CALIBRATION_KEYS
};

/* event strings */
enum {
    GR_EVENT_DEFAULT      = 0,
    GR_EVENT_KEYS
};


#endif /* GR_TYPES_DEFINED */


/* FILE:drivers/graspForce/grasp-message.h */
#ifndef GR_TYPES_DEFINED
#define GR_TYPES_DEFINED
enum
{
    GR_INFO_MODE = 1,
};

enum
{
    GR_CONFIG_DEFAULT = 0,
    GR_CONFIG_GLOVE,
    GR_CONFIG_TRACKER
};


/* FILE:drivers/graspForce/grasp-message.h */
/* opmode in reply msg arkcode */
enum {
    GR_REPORT_DEFAULT       = 0,
    GR_REPORT_FORCE,			
    GR_REPORT_IMPEDENCE,
    GR_REPORT_MAX_FORCE,		/* maximum force */
    GR_REPORT_ENCODER,
    GR_REPORT_KEYS
};


/* FILE:drivers/graspForce/grasp-message.h */
enum {
    GR_CONTROL_DEFAULT      = 0,
    GR_CONTROL_FORCE,
    GR_CONTROL_IMPEDENCE,
    GR_CONTROL_IDLE,
    GR_CONTROL_REWIND,
    GR_CONTROL_EFFECT,
    GR_CONTROL_RESET,
    GR_CONTROL_KEYS
};


/* FILE:drivers/graspForce/grasp-message.h */
/* calibrate strings */
enum {
    GR_CALIBRATION_DEFAULT      = 0,
    GR_CALIBRATION_E2G_MAP      = 1, /* encoder to glove map */
    GR_CALIBRATION_KEYS
};

/* event strings */
enum {
    GR_EVENT_DEFAULT      = 0,
    GR_EVENT_KEYS
};


#endif /* GR_TYPES_DEFINED */


/* FILE:drivers/stylus3/stylus3-message.h */
enum {ST_REPORT_DEFAULT, 
      ST_REPORT_RAW,		/* before calibration */
      ST_REPORT_POS_ANGLE,	/* pos + euler angle (zyx) */
      ST_REPORT_POS_QUATERNION,	/* pos + quaternion */
      ST_REPORT_MATRIX,		/* 4x4 matrix */
      ST_REPORT_BUTTON,		/* button */
      ST_REPORT_KEYS };


/* FILE:drivers/stylus3/stylus3-message.h */
enum {ST_CALIBRATION_DEFAULT, 
      ST_CALIBRATION_POSITION,	/* based on desired pos */
      ST_CALIBRATION_ORITENTATION, /* based on desired xform matrix */
      ST_CALIBRATION_XFORM,	/* based on 4x4 desired xform matrix */
      ST_CALIBRATION_MEASUREMENT,
      ST_CALIBRATION_RESTORE,	/* revert to the last saved calibration */
      ST_CALIBRATION_SAVE,	/* save the current calibration */
      ST_CALIBRATION_KEYS
};


/* FILE:drivers/stylus3/stylus3-message.h */
enum {ST_CONTROL_DEFAULT, ST_CONTROL_KEYS};


/* FILE:drivers/stylus3/stylus3-message.h */
enum {ST_EVENT_DEFAULT, 
      ST_EVENT_RAW_CHANGE, 
      ST_EVENT_CALIBRATION_CHANGE,
      ST_EVENT_BUTTON_CHANGE,
      ST_EVENT_CONDITION,
      ST_EVENT_ERROR, 
      ST_EVENT_KEYS};


/* FILE:drivers/stylus3/stylus3-types.h */
#ifndef STYLUS3_TYPES_DEFINED
#define STYLUS3_TYPES_DEFINED

enum
{
    ST_CONFIG_METRIC, 
    ST_CONFIG_KEYS
};


/* FILE:drivers/stylus3/stylus3-types.h */
enum
{
    /* frame based on mounting plate and transmitter cable*/
    ST_AXIS_X,			/* same dir as incoming cable */
    ST_AXIS_Y,			/* satisfy x, z */
    ST_AXIS_Z,			/* mounting plate */
    ST_AXIS_NX,
    ST_AXIS_NY,
    ST_AXIS_NZ,
};

#endif /* STYLUS3_TYPES_DEFINED */


/* FILE:drivers/tracker/tracker-message.h */
enum {TR_REPORT_DEFAULT, 
      TR_REPORT_RAW,		/* before calibration */
      TR_REPORT_POS_ANGLE,	/* pos + euler angle (zyx) */
      TR_REPORT_POS_QUATERNION,	/* pos + quaternion */
      TR_REPORT_MATRIX,		/* 4x4 matrix */
      TR_REPORT_BUTTON, 
      TR_REPORT_KEYS };


/* FILE:drivers/tracker/tracker-message.h */
enum {TR_CALIBRATION_DEFAULT, 
      TR_CALIBRATION_POSITION,	/* based on desired pos */
      TR_CALIBRATION_ORITENTATION, /* based on desired xform matrix */
      TR_CALIBRATION_XFORM,	/* based on 4x4 desired xform matrix */
      TR_CALIBRATION_MEASUREMENT,
      TR_CALIBRATION_RESTORE,	/* revert to the last saved calibration */
      TR_CALIBRATION_SAVE,	/* save the current calibration */
      TR_CALIBRATION_KEYS};


/* FILE:drivers/tracker/tracker-message.h */
enum {TR_CONTROL_DEFAULT, TR_CONTROL_KEYS};


/* FILE:drivers/tracker/tracker-message.h */
enum {TR_EVENT_DEFAULT, 
      TR_EVENT_RAW_CHANGE, 
      TR_EVENT_CALIBRATION_CHANGE,
      TR_EVENT_BUTTON_CHANGE,
      TR_EVENT_CONDITION,
      TR_EVENT_ERROR, 
      TR_EVENT_KEYS};


/* FILE:drivers/tracker/tracker-types.h */
#ifndef TRACKER_TYPES_DEFINED
#define TRACKER_TYPES_DEFINED

enum
{
    TR_CONFIG_HEMI = 0, 
    TR_CONFIG_FILTER, 
    TR_CONFIG_CRTSYNC,
    TR_CONFIG_METRIC, 
    TR_CONFIG_SELECT,		/*  */
    TR_CONFIG_CORD,		/* transmitter cord location,!obsolete! */
    TR_CONFIG_PLATE,		/* mounting plate location, !obsolete! */
    TR_CONFIG_RECEIVER,		/* current receiver for ascii mode only */
    TR_CONFIG_CORDPLATE,	/* right way to do it */
    TR_CONFIG_KEYS
};


/* FILE:drivers/tracker/tracker-types.h */
enum
{
    TR_AXIS_KEEP = -1,		/* keep the old axis */
    TR_AXIS_INVALID = -1,
    /* frame based on mounting plate and transmitter cable*/
    TR_AXIS_X = 0,		/* same dir as incoming cable */
    TR_AXIS_Y,			/* satisfy x, z */
    TR_AXIS_Z,			/* mounting plate */
    TR_AXIS_NX,
    TR_AXIS_NY,
    TR_AXIS_NZ,
    /* based on transmitter dimension and its cable alignment */
    TR_AXIS_FRONT,		/* same direction as incoming  cable */
    TR_AXIS_BACK,		/* reverse dir as ... */
    TR_AXIS_UPPER,		
    TR_AXIS_LOWER,		/* side closest to the cable */
    TR_AXIS_LEFT,		/* facing FRONT, left side */
    TR_AXIS_RIGHT,		/* facing FRONT, right side */
    /* user defined */
    TR_AXIS_USER_DEFINED,
};

#endif /* TRACKER_TYPES_DEFINED */


