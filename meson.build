project(
    'phosphor-dbus-monitor', 'cpp',
    version : '1.0.0',
    meson_version: '>=0.58.0',
    default_options: [
        'warning_level=3',
        'werror=true',
        'cpp_std=c++20',
        'buildtype=debugoptimized',
    ]
)

conf_data = configuration_data()
conf_data.set_quoted('OBJ_EVENT', '/xyz/openbmc_project/events')
conf_data.set_quoted('BUSNAME_EVENT', 'xyz.openbmc_project.Events')
conf_data.set_quoted('EVENTS_PERSIST_PATH', '/var/lib/phosphor-dbus-monitor/events')

conf_data.set('CLASS_VERSION', 1)
conf_data.set('MAX_EVENTS', 20)

sdbusplus_dep = dependency('sdbusplus')
sdeventplus_dep = dependency('sdeventplus')
phosphor_dbus_interfaces_dep = dependency('phosphor-dbus-interfaces')
phosphor_logging_dep = dependency('phosphor-logging')
phosphor_snmp_dep = dependency('phosphor-snmp')

prog_python = find_program('python3', required: true)
realpath_prog = find_program('realpath')

configure_file(output: 'config.h',
    configuration: conf_data
)

subdir('src')
subdir('mslverify')
