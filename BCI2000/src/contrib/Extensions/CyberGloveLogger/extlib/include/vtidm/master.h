/*============================================================================

  $Id: master.h,v 1.2 2002/10/11 22:33:17 dgomez Exp $

  HISTORY:

 ============================================================================*/

#ifndef _VT_MASTER_H_
#define _VT_MASTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_MASTER_TCP_PORT   "12345"
#define DEFAULT_MAX_CLIENTS       "13"

#define  DEFAULT_MODULE_PATH "." DSEP
#define  DEFAULT_CONFIG_PATH ".." DSEP "config"
#define  DEFAULT_SESSION_SERVER "127.0.0.1"

#ifndef _WIN32
#define DEFALULT_MASTER_UNIXSOCK_NAME "/tmp/vtidm/master.sock"
#define  MASTER_LOCK_FILE    "/tmp/vtidm.lck"
#endif

#ifdef __cplusplus
}
#endif

#endif
