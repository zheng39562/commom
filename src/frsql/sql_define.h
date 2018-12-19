/**********************************************************
 *  \file sql_define.h
 *  \brief
 *  \note	×¢ÒâÊÂÏî£º 
 * 
 * \version 
 * * \author zheng39562@163.com
**********************************************************/
#ifndef _sql_define_H
#define _sql_define_H

#include "frpublic/pub_log.h"
#include "frpublic/pub_timer.h"
#include "frpublic/pub_string.h"

#define SQL_DEBUG_D(msg, arg...) DEBUG_D(msg, ##arg)
#define SQL_DEBUG_I(msg, arg...) DEBUG_I(msg, ##arg)
#define SQL_DEBUG_W(msg, arg...) DEBUG_W(msg, ##arg)
#define SQL_DEBUG_E(msg, arg...) DEBUG_E(msg, ##arg)
#define SQL_DEBUG_C(msg, arg...) DEBUG_C(msg, ##arg)

#endif 

