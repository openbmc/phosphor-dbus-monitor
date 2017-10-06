# Initialization
AC_PREREQ([2.69])
AC_INIT([phosphor-dbus-monitor], [1.0], [https://github.com/openbmc/phosphor-dbus-monitor/issues])
AC_LANG([C++])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror foreign dist-xz])
AM_SILENT_RULES([yes])

# Checks for programs.
AC_PROG_CXX
AM_PROG_AR
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_AWK
AM_PATH_PYTHON([2.7],
    [AC_SUBST([PYTHON], [echo "$PYTHON"])],
    [AC_MSG_ERROR([Could not find python-2.7 installed...python-2.7 is required])])

# Checks for libraries.
PKG_CHECK_MODULES([SDBUSPLUS], [sdbusplus],, AC_MSG_ERROR(["Requires sdbusplus package."]))
PKG_CHECK_MODULES([PHOSPHOR_DBUS_INTERFACES], [phosphor-dbus-interfaces],, AC_MSG_ERROR(["Requires phosphor-dbus-interfaces package."]))
PKG_CHECK_MODULES([PHOSPHOR_LOGGING], [phosphor-logging],, AC_MSG_ERROR(["Requires phosphor-logging."]))

# Check for sdbus++
AC_PATH_PROG([SDBUSPLUSPLUS], [sdbus++])
AS_IF([test "x$SDBUSPLUSPLUS" == "x"],
    AC_MSG_ERROR(["Requires sdbus++"]))

# Checks for typedefs, structures, and compiler characteristics.
AX_CXX_COMPILE_STDCXX_14([noext])
AX_APPEND_COMPILE_FLAGS([-fpic -Wall -Werror], [CXXFLAGS])

# Checks for library functions.
LT_INIT

# Check/set gtest specific functions.
AX_PTHREAD([GTEST_CPPFLAGS="-DGTEST_HAS_PTHREAD=1"],[GTEST_CPPFLAGS="-DGTEST_HAS_PTHREAD=0"])
AC_SUBST(GTEST_CPPFLAGS)

AC_ARG_ENABLE([oe-sdk],
    AS_HELP_STRING([--enable-oe-sdk], [Link testcases absolutely against OE SDK so they can be ran within it.])
)
AC_ARG_VAR(OECORE_TARGET_SYSROOT,
    [Path to the OE SDK SYSROOT])
AS_IF([test "x$enable_oe_sdk" == "xyes"],
    AS_IF([test "x$OECORE_TARGET_SYSROOT" == "x"],
          AC_MSG_ERROR([OECORE_TARGET_SYSROOT must be set with --enable-oe-sdk])
    )
    AC_MSG_NOTICE([Enabling OE-SDK at $OECORE_TARGET_SYSROOT])
    [
        testcase_flags="-Wl,-rpath,\${OECORE_TARGET_SYSROOT}/lib"
        testcase_flags="${testcase_flags} -Wl,-rpath,\${OECORE_TARGET_SYSROOT}/usr/lib"
        testcase_flags="${testcase_flags} -Wl,-dynamic-linker,`find \${OECORE_TARGET_SYSROOT}/lib/ld-*.so | sort -r -n | head -n1`"
    ]
    AC_SUBST([OESDK_TESTCASE_FLAGS], [$testcase_flags])
)

AC_ARG_VAR(OBJ_EVENT, [The event logger DBus object path])
AS_IF([test "x$OBJ_EVENT" == "x"], [OBJ_EVENT="/xyz/openbmc_project/events"])
AC_DEFINE_UNQUOTED([OBJ_EVENT], ["$OBJ_EVENT"], [The event logger DBus object path])

AC_ARG_VAR(BUSNAME_EVENT, [The Dbus busname to own])
AS_IF([test "x$BUSNAME_EVENT" == "x"], [BUSNAME_EVENT="xyz.openbmc_project.Event"])
AC_DEFINE_UNQUOTED([BUSNAME_EVENT], ["$BUSNAME_EVENT"], [The DBus busname to own])

AC_ARG_VAR(YAML_PATH, [The path to the yaml config files.])
AS_IF([test "x$YAML_PATH" == "x"], [YAML_PATH="\${top_srcdir}/src/example"])

AC_ARG_VAR(EVENTS_YAML_PATH, [The path to the event yaml config files.])
AS_IF([test "x$EVENTS_YAML_PATH" == "x"], [EVENTS_YAML_PATH="\${top_srcdir}/src/events/yaml"])

AC_CONFIG_FILES([Makefile src/Makefile src/test/Makefile src/events/Makefile  mslverify/Makefile])
AC_OUTPUT
