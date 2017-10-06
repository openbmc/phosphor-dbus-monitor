include ${top_srcdir}/vars.mk

AM_DEFAULT_SOURCE_EXT = .cpp
AM_CPPFLAGS = -iquote ${top_srcdir}/src

sbin_PROGRAMS = phosphor-event-manager

phosphor_event_manager_SOURCES = \
	main.cpp \
	../propertywatch.cpp \
	event.cpp \
	event_manager.cpp

phosphor_event_manager_LDADD = \
	$(SDBUSPLUS_LIBS) \
	$(PHOSPHOR_DBUS_INTERFACES_LIBS) \
	$(PHOSPHOR_LOGGING_LIBS) \
	$(builddir)/../journal.o \
	$(builddir)/../elog.o

phosphor_event_manager_CXXFLAGS = \
	$(SDBUSPLUS_CFLAGS) \
	$(PHOSPHOR_DBUS_INTERFACES_CFLAGS) \
	$(PHOSPHOR_LOGGING_CFLAGS)

BUILT_SOURCES = generated.hpp

CLEANFILES = generated.hpp

TEMPLATES = \
	../templates/callbackgroup.mako.cpp \
	../templates/conditional.mako.cpp \
	../templates/count.mako.cpp \
	../templates/generated.mako.hpp \
	../templates/journal.mako.cpp \
	../templates/elog.mako.cpp \
	../templates/errors.mako.hpp \
	../templates/method.mako.cpp \
	../templates/event.mako.cpp

generated.hpp: $(PDMGEN) $(EVENTS_YAML_PATH) $(TEMPLATES)
	$(AM_V_GEN)$(PYTHON) ${PDMGEN} \
		-t generated.mako.hpp \
		-p "${TEMPLATESEARCH}" \
		-d $(EVENTS_YAML_PATH) \
		-o ${builddir}/$@ \
		generate-cpp
