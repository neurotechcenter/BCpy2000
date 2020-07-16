/*  
 *  cv_protocol_sample.h :  Tobii ClearView Trigger Protocol API
 *
 *  Copyright (C) Tobii Technology 2002-2005, all rights reserved.
 */

#ifndef CV_PROTOCOL_SAMPLE_H_
#define CV_PROTOCOL_SAMPLE_H_


/* Listening port number            */
#define CV_PROTOCOL_PORT_NUMBER             4456

/* Error codes                      */
#define TOB_NO_ERROR                                            0x0
#define TOB_ERR_BASE                                            0x20000000

#define CV_PROTOCOL_ERR_NO_ERR                                  TOB_NO_ERROR
#define CV_PROTOCOL_ERR_BASE                                    (TOB_ERR_BASE | 0x800)  
#define CV_PROTOCOL_ERR_OPERATION_IMPOSSIBLE_IN_CURRENT_STATE   (CV_PROTOCOL_ERR_BASE | 0x1)
#define CV_PROTOCOL_ERR_NOCALIBSET                              (CV_PROTOCOL_ERR_BASE | 0x2)
#define CV_PROTOCOL_ERR_BAD_FORMAT                              (CV_PROTOCOL_ERR_BASE | 0x3)
#define CV_PROTOCOL_ERR_INTERNAL                                (CV_PROTOCOL_ERR_BASE | 0x4)
#define CV_PROTOCOL_ERR_FILE_EXISTS                             (CV_PROTOCOL_ERR_BASE | 0x5)
#define CV_PROTOCOL_ERR_FILE_NAME_FORMAT                        (CV_PROTOCOL_ERR_BASE | 0x6)


/* type field of message header.    */
#define CV_TRANSACTION_TYPE_QUESTION     'Q'    /* Question from client to ClearView                    */
#define CV_TRANSACTION_TYPE_RESPONSE     'R'    /* Response from ClearView                              */



/* command field of message header. */  
typedef enum _ECV_Command {
    CV_CMD_START                        = 101,  /* Start recording                                      */
    CV_CMD_STOP                         = 102,  /* Stop recording                                       */
    CV_CMD_LOG                          = 103,  /* Log data. Requires additional data after header.     */
    CV_CMD_GENERIC_EVENT                = 104,  /* Send a generic event. Will trig a ClearView action   */
                                                /*      dependent of type of ongoing recording.         */
    CV_CMD_START_USING_NAME             = 105   /* Start recording and save it as the file name         */
                                                /*      supplied. Requires additional data after header.*/
} ECV_Command;



/* Message header for both directions: To and from ClearView.   */
/* A complete message consists of the message header and data   */ 
/* appended for the specific commands requiring additional data.*/
typedef struct _SCV_MsgHeader {
    unsigned long type;             /* Direction. See CV_TRANSACTION_TYPE_XXX definitions above.        */
    unsigned long transaction_id;   /* ID set by caller for question type and copied to response.       */ 
    unsigned long transaction_seq;  /* Sequential number incremented by 1 for each response.            */
    unsigned long command;          /* ECV_Command: Kind of message. Copied to response.                */
    unsigned long errorcode;        /* Error code, anything but 0 is error. Definitions in toberror.h.  */
    unsigned long datalen;          /* Length in bytes, not including header, of data followed.         */ 
} SCV_MsgHeader;


#endif /* CV_PROTOCOL_SAMPLE_H_ */
