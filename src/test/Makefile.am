include $(top_srcdir)/vars.mk

AM_CPPFLAGS = -iquote$(top_srcdir)/src
gtest_cflags = $(PTHREAD_CFLAGS)
gtest_ldadd = -lgtest -lgtest_main -lgmock $(PTHREAD_LIBS)

TEMPLATESEARCH+=${srcdir}/templates

check_PROGRAMS =
BUILT_SOURCES =
CLEANFILES =

TESTS = $(check_PROGRAMS)
