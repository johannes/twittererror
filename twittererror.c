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
  | Author: Johannes Schlüter <johannes@php.net>
  +----------------------------------------------------------------------+
*/

/* $Id:$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/url.h"
#include "php_twittererror.h"

void (*original_error_cb)(int type, const char *error_filename, const uint error_lineno, const char *format, va_list args);

ZEND_DECLARE_MODULE_GLOBALS(twittererror)

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("twittererror.mail", "", PHP_INI_ALL, OnUpdateString, mail, zend_twittererror_globals, twittererror_globals)
    STD_PHP_INI_ENTRY("twittererror.pwd",  "", PHP_INI_ALL, OnUpdateString, pwd,  zend_twittererror_globals, twittererror_globals)
PHP_INI_END()
/* }}} */

/* {{{ php_twittererror_init_globals
 */
static void php_twittererror_init_globals(zend_twittererror_globals *twittererror_globals)
{
	twittererror_globals->mail = NULL;
	twittererror_globals->pwd  = NULL;
	twittererror_globals->in_error = 0;
}
/* }}} */

#define TWITTER_HOST "twitter.com"
#define TWITTER_URI  "/statuses/update.xml"

/* {{{ twitter_error_cb
 */
static void twitter_error_cb(int type, const char *error_filename, const uint error_lineno, const char *format, va_list args) {
	char *msg;
	int msg_len;

	if (TWITTERERROR_G(in_error)) {
		return;
	}
	TWITTERERROR_G(in_error) = 1;

	msg_len = vspprintf(&msg, 0, format, args);	
	original_error_cb(type, error_filename, error_lineno, msg, NULL);

	if (TWITTERERROR_G(mail) && *TWITTERERROR_G(mail) && TWITTERERROR_G(pwd) && *TWITTERERROR_G(pwd)) {
		char *user, *pwd;
		int  user_len, pwd_len;
		char *twitterurl = NULL;
		char *msg_enc = NULL, *content = NULL;
		int msg_enc_len, content_len;

		zval zmethod, zcontent;

		php_stream *stream = NULL;
		php_stream_context *context = NULL;

		user    = php_url_encode(TWITTERERROR_G(mail), strlen(TWITTERERROR_G(mail)), &user_len);
		pwd     = php_url_encode(TWITTERERROR_G(pwd),  strlen(TWITTERERROR_G(pwd)),  &pwd_len);
		msg_enc = php_url_encode(msg, msg_len, &msg_enc_len);

		(void)spprintf(&twitterurl, 0, "http://%s:%s@%s%s", user, pwd, TWITTER_HOST, TWITTER_URI);
		efree(user);
		efree(pwd);

		content_len = spprintf(&content, 0, "status=%s", msg_enc);
		efree(msg_enc);

		context = php_stream_context_alloc();

		ZVAL_STRINGL(&zmethod, "POST", sizeof("POST")-1, 1);
		php_stream_context_set_option(context, "http", "method", &zmethod);

		ZVAL_STRINGL(&zcontent, content, content_len, 0);
		php_stream_context_set_option(context, "http", "content", &zcontent);
		
		stream = php_stream_open_wrapper_ex(twitterurl, "r", 0, NULL, context);

		if (stream) {
			php_stream_close(stream);
		}
		zval_dtor(&zmethod);
		TWITTERERROR_G(in_error) = 0;
		efree(twitterurl);
	}

	efree(msg);
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(twittererror)
{
	REGISTER_INI_ENTRIES();

	original_error_cb = zend_error_cb;
	zend_error_cb = twitter_error_cb; 

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(twittererror)
{
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(twittererror)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "twittererror support", "enabled");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */


/* {{{ twittererror_module_entry
 *  */
zend_module_entry twittererror_module_entry = {
	STANDARD_MODULE_HEADER,
	"twittererror",
	NULL,
	PHP_MINIT(twittererror),
	PHP_MSHUTDOWN(twittererror),
	NULL,
	NULL,
	PHP_MINFO(twittererror),
	"0.1",
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_TWITTERERROR
ZEND_GET_MODULE(twittererror)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
