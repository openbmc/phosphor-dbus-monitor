include $(top_srcdir)/vars.mk

AM_CPPFLAGS = -iquote$(top_srcdir)/src
gtest_cflags = $(PTHREAD_CFLAGS)
gtest_ldadd = -lgtest -lgtest_main -lgmock $(PTHREAD_LIBS)

TEMPLATESEARCH+=${srcdir}/templates

check_PROGRAMS =
BUILT_SOURCES =
CLEANFILES =

TESTS = $(check_PROGRAMS)

check_PROGRAMS += pathgentest
pathgentest_SOURCES = \
	pathgentest.cpp
pathgentest_CXXFLAGS = \
	$(gtest_cflags)
pathgentest_LDFLAGS = \
	$(OESDK_TESTCASE_FLAGS)
pathgentest_LDADD = \
	${gtest_ldadd}

BUILT_SOURCES += pathgentest.hpp
CLEANFILES += pathgentest.hpp

PATH_TEST_GEN_DEPS = \
	templates/pathgentest.mako.hpp \
	yaml/pathgentest

pathgentest.hpp: $(PDMGEN) $(PATH_TEST_GEN_DEPS)
	$(AM_V_GEN) $(PYTHON) $(PDMGEN) \
		-t pathgentest.mako.hpp \
		-p "${TEMPLATESEARCH}" \
		-d yaml/pathgentest \
		-o $(builddir)/$@ \
		generate-cpp

check_PROGRAMS += propertygentest
propertygentest_SOURCES = \
	propertygentest.cpp
propertygentest_CXXFLAGS = \
	$(gtest_cflags)
propertygentest_LDFLAGS = \
	$(OESDK_TESTCASE_FLAGS)
propertygentest_LDADD = \
	${gtest_ldadd}

BUILT_SOURCES += propertygentest.hpp
CLEANFILES += propertygentest.hpp

PROPERTY_TEST_GEN_DEPS = \
	templates/propertygentest.mako.hpp \
	yaml/propertygentest

propertygentest.hpp: $(PROPERTY_TEST_GEN_DEPS)
	$(AM_V_GEN) $(PYTHON) $(PDMGEN) \
		-t propertygentest.mako.hpp \
		-p "${TEMPLATESEARCH}" \
		-d yaml/propertygentest \
		-o $(builddir)/$@ \
		generate-cpp
