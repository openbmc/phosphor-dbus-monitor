include $(top_srcdir)/vars.mk

AM_CPPFLAGS = -iquote$(top_srcdir)/src -iquote$(top_srcdir)/src/events
gtest_cflags = $(PTHREAD_CFLAGS)
gtest_ldadd = -lgtest -lgtest_main -lgmock $(PTHREAD_LIBS)

TEMPLATESEARCH+=${srcdir}/templates

check_PROGRAMS =
noinst_PROGRAMS =
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
		-d ${srcdir}/yaml/pathgentest \
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
		-d ${srcdir}/yaml/propertygentest \
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
		-d ${srcdir}/yaml/propertywatchgentest \
		-o $(builddir)/$@ \
		generate-cpp

# propertywatchtest is intentionally omitted from
# check_PROGRAMS until a bug that manifests with
# with GCC7 can be resolved.

noinst_PROGRAMS += propertywatchtest
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
		-d ${srcdir}/yaml/callbackgentest \
		-o $(builddir)/$@ \
		generate-cpp

check_PROGRAMS += callbackgroupgentest
callbackgroupgentest_SOURCES = \
	callbackgroupgentest.cpp
callbackgroupgentest_CXXFLAGS = \
	$(gtest_cflags)
callbackgroupgentest_LDFLAGS = \
	$(OESDK_TESTCASE_FLAGS)
callbackgroupgentest_LDADD = \
	${gtest_ldadd}

BUILT_SOURCES += callbackgroupgentest.hpp
CLEANFILES += callbackgroupgentest.hpp

CALLBACKGROUP_TEST_GEN_DEPS = \
	templates/callbackgroupgentest.mako.hpp \
	yaml/callbackgroupgentest

callbackgroupgentest.hpp: $(CALLBACKGROUP_TEST_GEN_DEPS)
	$(AM_V_GEN) $(PYTHON) $(PDMGEN) \
		-t callbackgroupgentest.mako.hpp \
		-p "${TEMPLATESEARCH}" \
		-d ${srcdir}/yaml/callbackgroupgentest \
		-o $(builddir)/$@ \
		generate-cpp

check_PROGRAMS += conditiongentest
conditiongentest_SOURCES = \
	conditiongentest.cpp
conditiongentest_CXXFLAGS = \
	$(gtest_cflags)
conditiongentest_LDFLAGS = \
	$(OESDK_TESTCASE_FLAGS)
conditiongentest_LDADD = \
	${gtest_ldadd}

BUILT_SOURCES += conditiongentest.hpp
CLEANFILES += conditiongentest.hpp

CONDITION_TEST_GEN_DEPS = \
	templates/conditiongentest.mako.hpp \
	yaml/conditiongentest

conditiongentest.hpp: $(CONDITION_TEST_GEN_DEPS)
	$(AM_V_GEN) $(PYTHON) $(PDMGEN) \
		-t conditiongentest.mako.hpp \
		-p "${TEMPLATESEARCH}" \
		-d ${srcdir}/yaml/conditiongentest \
		-o $(builddir)/$@ \
		generate-cpp

check_PROGRAMS += callbacktest
callbacktest_SOURCES = \
	callbacktest.cpp
callbacktest_CXXFLAGS = \
	$(gtest_cflags)
	$(SDBUSPLUS_CFLAGS) \
	$(PHOSPHOR_DBUS_INTERFACES_CFLAGS) \
	$(PHOSPHOR_LOGGING_CFLAGS)
callbacktest_LDFLAGS = \
	$(OESDK_TESTCASE_FLAGS)
callbacktest_LDADD = \
	${gtest_ldadd} \
	${SDBUSPLUS_LIBS} \
	$(PHOSPHOR_DBUS_INTERFACES_LIBS) \
	$(PHOSPHOR_LOGGING_LIBS) \
	$(builddir)/../journal.o \
	$(builddir)/../elog.o \
	$(builddir)/../event.o \
	$(builddir)/../event_manager.o
