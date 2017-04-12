AM_DEFAULT_SOURCE_EXT = .cpp
AM_CPPFLAGS = -iquote ${top_srcdir}

sbin_PROGRAMS = phosphor-dbus-monitor

phosphor_dbus_monitor_SOURCES = \
	main.cpp \
	monitor.cpp
phosphor_dbus_monitor_LDADD = \
	$(SDBUSPLUS_LIBS) \
	$(PHOSPHOR_LOGGING_LIBS)
phosphor_dbus_monitor_CXXFLAGS = \
	$(SDBUSPLUS_CFLAGS) \
	$(PHOSPHOR_LOGGING_CFLAGS)
