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

check_PROGRAMS += propertywatchgentest
propertywatchgentest_SOURCES = \
	propertywatchgentest.cpp
propertywatchgentest_CXXFLAGS = \
	$(gtest_cflags)
propertywatchgentest_LDFLAGS = \
	$(OESDK_TESTCASE_FLAGS)
propertywatchgentest_LDADD = \
	${gtest_ldadd}

BUILT_SOURCES += propertywatchgentest.hpp
CLEANFILES += propertywatchgentest.hpp

PROPERTY_WATCH_TEST_GEN_DEPS = \
	templates/propertywatchgentest.mako.hpp \
	yaml/propertywatchgentest

propertywatchgentest.hpp: $(PROPERTY_WATCH_TEST_GEN_DEPS)
	$(AM_V_GEN) $(PYTHON) $(PDMGEN) \
		-t propertywatchgentest.mako.hpp \
		-p "${TEMPLATESEARCH}" \
		-d yaml/propertywatchgentest \
		-o $(builddir)/$@ \
		generate-cpp

check_PROGRAMS += propertywatchtest
propertywatchtest_SOURCES = \
	propertywatchtest.cpp
propertywatchtest_CXXFLAGS = \
	$(gtest_cflags) \
	${SDBUSPLUS_CFLAGS}
propertywatchtest_LDFLAGS = \
	$(OESDK_TESTCASE_FLAGS)
propertywatchtest_LDADD = \
	${gtest_ldadd} \
	${SDBUSPLUS_LIBS} \
	$(builddir)/../propertywatch.o

check_PROGRAMS += callbackgentest
callbackgentest_SOURCES = \
	callbackgentest.cpp
callbackgentest_CXXFLAGS = \
	$(gtest_cflags)
callbackgentest_LDFLAGS = \
	$(OESDK_TESTCASE_FLAGS)
callbackgentest_LDADD = \
	${gtest_ldadd}

BUILT_SOURCES += callbackgentest.hpp
CLEANFILES += callbackgentest.hpp

CALLBACK_TEST_GEN_DEPS = \
	templates/callbackgentest.mako.hpp \
	yaml/callbackgentest

callbackgentest.hpp: $(CALLBACK_TEST_GEN_DEPS)
	$(AM_V_GEN) $(PYTHON) $(PDMGEN) \
		-t callbackgentest.mako.hpp \
		-p "${TEMPLATESEARCH}" \
		-d yaml/callbackgentest \
		-o $(builddir)/$@ \
		generate-cpp
