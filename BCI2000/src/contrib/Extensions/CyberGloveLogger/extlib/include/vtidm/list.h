/*============================================================================

  $Id: list.h,v 1.2 2002/10/11 22:33:17 dgomez Exp $

  HISTORY:

============================================================================*/

#ifndef _VT_LIST_H
#define _VT_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "multithread.h"

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-*/
/* types                                                                    */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-*/
#define VT_MAGIC_LIST      0x4c495354

#define LIST_HEAD(t)   \
  long magic; \
  struct t * prev; \
  struct t * next; \
  void (* fpdel)(struct t *); \
  vt_bool once
  
typedef struct _list_item_t {
    LIST_HEAD(_list_item_t);
} list_item_t, *list_item;

typedef struct _vt_list_t {
    long      magic;
    list_item head;
    int       nitems;
    vt_mutex  lock;
} vt_list_t, *vt_list;

typedef void (* VT_LIST_ITEM_DELETE_PROC )(list_item item);

#define VT_ListIsValid(lst)\
 ((vt_list)(lst) != NULL && ((vt_list)(lst))->magic == VT_MAGIC_LIST)

#define VT_ListItemIsValid(item) \
 ((list_item)(item) != NULL && ((list_item)(item))->magic == VT_MAGIC_LIST)

#define VT_ListIsEmpty(lst)\
 (((vt_list)(lst))->nitems < 1 || ((vt_list)(lst))->head == NULL)

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-*/
/* prototypes                                                               */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-*/

VT_DLL_PROC vt_list VT_ListNew();

VT_DLL_PROC void VT_ListDestroy(vt_list lst);

VT_DLL_PROC vt_error VT_ListAddItem(vt_list lst, void * item);

VT_DLL_PROC vt_error VT_ListAddItemBefore(vt_list lst, void * target, void * item);

VT_DLL_PROC vt_error VT_ListAddItemAfter(vt_list lst , void * target, 
				   void * item);
/* fpdel will be called before VT_ListRemoveItem free memory of list item*/
VT_DLL_PROC void * VT_ListItemNew(int sz, VT_LIST_ITEM_DELETE_PROC fpdel);
VT_DLL_PROC void VT_ListItemDestroy(void * item);

VT_DLL_PROC vt_error VT_ListDeleteItem(vt_list lst, void * item);

VT_DLL_PROC int  VT_ListProcess(vt_list lst, 
				vt_bool (* fpsel)(void *),
				vt_error (* fpproc)(void *),
				vt_bool bDelOnError);

VT_DLL_PROC void VT_ListPrint(vt_list lst, char * cmt);

/* the lock functions should have timeout in case something corrupted */
/* call by functions which need to modify the list, prevent others from access
   (read) the list while is being updated.
 */
VT_DLL_PROC vt_error VT_ListLockRead(vt_list lst);
VT_DLL_PROC vt_error VT_ListUnlockRead(vt_list lst);

/* call by functions which need to read the list, prevent others from modify
   the list while is being read*/
#define VT_ListLockWrite   VT_ListLockRead
#define VT_ListUnlockWrite VT_ListUnlockRead

VT_DLL_PROC vt_bool VT_ListItemIsOfList(void * item, vt_list lst) ;

#ifdef __cplusplus
}
#endif

#endif
