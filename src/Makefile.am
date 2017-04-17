AM_DEFAULT_SOURCE_EXT = .cpp
AM_CPPFLAGS = -iquote ${top_srcdir}

sbin_PROGRAMS = phosphor-dbus-monitor

phosphor_dbus_monitor_SOURCES = \
	functor.cpp \
	main.cpp \
	monitor.cpp \
	generated.cpp
phosphor_dbus_monitor_LDADD = \
	$(SDBUSPLUS_LIBS) \
	$(PHOSPHOR_LOGGING_LIBS)
phosphor_dbus_monitor_CXXFLAGS = \
	$(SDBUSPLUS_CFLAGS) \
	$(PHOSPHOR_LOGGING_CFLAGS)

BUILT_SOURCES = generated.cpp
CLEANFILES = generated.cpp

generated.cpp:
	$(AM_V_GEN)$(PYTHON) ${srcdir}/gen-group-defs.py -y ${srcdir}/example/monitoring_defs.yaml -o ${builddir}
