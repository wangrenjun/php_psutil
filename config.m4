PHP_ARG_WITH(php_psutil, for php_psutil support,
[  --with-php_psutil[=DIR]  Set the path to cpslib])

if test -r "$PHP_PHP_PSUTIL/libpslib.so" && test -r "$PHP_PHP_PSUTIL/pslib.h"; then
    PHP_PSUTIL_INCDIR="$PHP_PHP_PSUTIL"
    PHP_PSUTIL_LIBDIR="$PHP_PHP_PSUTIL"
else
    AC_MSG_ERROR([Must set the path to cpslib. Use --with-php_psutil=<DIR> to specify the location of the header and library files])
fi
PHP_ADD_INCLUDE($PHP_PSUTIL_INCDIR)
PHP_ADD_LIBRARY_WITH_PATH(pslib, $PHP_PSUTIL_LIBDIR, PHP_PSUTIL_SHARED_LIBADD)
PHP_SUBST(PHP_PSUTIL_SHARED_LIBADD)
PHP_NEW_EXTENSION(php_psutil, php_psutil.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
