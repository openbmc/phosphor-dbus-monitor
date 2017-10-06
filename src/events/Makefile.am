include ${top_srcdir}/vars.mk

AM_DEFAULT_SOURCE_EXT = .cpp
AM_CPPFLAGS = -iquote ${top_srcdir}/src

sbin_PROGRAMS = phosphor-event-logger

nobase_nodist_include_HEADERS = \
	xyz/openbmc_project/Events/Internal/Manager/server.hpp

phosphor_event_logger_SOURCES = \
	main.cpp \
	../propertywatch.cpp \
	event.cpp \
	event_manager.cpp \
	xyz/openbmc_project/Events/Internal/Manager/server.cpp

phosphor_event_logger_LDADD = \
	$(SDBUSPLUS_LIBS) \
	$(PHOSPHOR_DBUS_INTERFACES_LIBS) \
	$(PHOSPHOR_LOGGING_LIBS) \
    $(builddir)/../journal.o \
    $(builddir)/../elog.o

phosphor_event_logger_CXXFLAGS = \
	$(SDBUSPLUS_CFLAGS) \
	$(PHOSPHOR_DBUS_INTERFACES_CFLAGS) \
	$(PHOSPHOR_LOGGING_CFLAGS)

BUILT_SOURCES = generated.hpp \
	xyz/openbmc_project/Events/Internal/Manager/server.hpp

CLEANFILES = generated.hpp \
	xyz/openbmc_project/Events/Internal/Manager/server.hpp

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

xyz/openbmc_project/Events/Internal/Manager/server.cpp: xyz/openbmc_project/Events/Internal/Manager.interface.yaml xyz/openbmc_project/Events/Internal/Manager/server.hpp
	@mkdir -p `dirname $@`
	$(SDBUSPLUSPLUS) -r $(srcdir) interface server-cpp xyz.openbmc_project.Events.Internal.Manager > $@

xyz/openbmc_project/Events/Internal/Manager/server.hpp: xyz/openbmc_project/Events/Internal/Manager.interface.yaml
	@mkdir -p `dirname $@`
	$(SDBUSPLUSPLUS) -r $(srcdir) interface server-header xyz.openbmc_project.Events.Internal.Manager > $@
