include ${top_srcdir}/vars.mk

AM_DEFAULT_SOURCE_EXT = .cpp
AM_CPPFLAGS = -iquote ${top_srcdir}

sbin_PROGRAMS = phosphor-dbus-monitor

phosphor_dbus_monitor_SOURCES = \
	functor.cpp \
	main.cpp \
	monitor.cpp
phosphor_dbus_monitor_LDADD = \
	$(SDBUSPLUS_LIBS) \
	$(PHOSPHOR_LOGGING_LIBS)
phosphor_dbus_monitor_CXXFLAGS = \
	$(SDBUSPLUS_CFLAGS) \
	$(PHOSPHOR_LOGGING_CFLAGS)

BUILT_SOURCES = generated.hpp
CLEANFILES = generated.hpp

TEMPLATES = \
	templates/generated.mako.hpp

generated.hpp: $(PDMGEN) $(YAML_PATH) $(TEMPLATES)
	$(AM_V_GEN)$(PYTHON) ${PDMGEN} \
		-t generated.mako.hpp \
		-p "${TEMPLATESEARCH}" \
		-d ${srcdir}/example \
		-o ${builddir}/$@ \
		generate-cpp
