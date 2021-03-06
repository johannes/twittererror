/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2008 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header,v 1.16.2.1.2.1.2.1 2008/02/07 19:39:50 iliaa Exp $ */

#ifndef PHP_TWITTERERROR_H
#define PHP_TWITTERERROR_H

extern zend_module_entry twittererror_module_entry;
#define phpext_twittererror_ptr &twittererror_module_entry

#ifdef PHP_WIN32
#	define PHP_TWITTERERROR_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_TWITTERERROR_API __attribute__ ((visibility("default")))
#else
#	define PHP_TWITTERERROR_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_BEGIN_MODULE_GLOBALS(twittererror)
	char *mail;
	char *pwd;
	int in_error;
ZEND_END_MODULE_GLOBALS(twittererror)

#ifdef ZTS
#define TWITTERERROR_G(v) TSRMG(twittererror_globals_id, zend_twittererror_globals *, v)
#else
#define TWITTERERROR_G(v) (twittererror_globals.v)
#endif

#endif	/* PHP_TWITTERERROR_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
