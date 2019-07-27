/* pmarkdown extension for PHP */

#ifndef PHP_PMARKDOWN_H
# define PHP_PMARKDOWN_H

extern zend_module_entry pmarkdown_module_entry;
# define phpext_pmarkdown_ptr &pmarkdown_module_entry

# define PHP_PMARKDOWN_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_PMARKDOWN)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_PMARKDOWN_H */

