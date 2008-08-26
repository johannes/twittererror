PHP_ARG_ENABLE(twittererror, whether to enable twittererror support,
[  --enable-twittererror           Enable twittererror support])

if test "$PHP_TWITTERERROR" != "no"; then
  PHP_NEW_EXTENSION(twittererror, twittererror.c, $ext_shared)
fi
