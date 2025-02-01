project(
    'phosphor-dbus-monitor',
    'cpp',
    version: '1.0.0',
    meson_version: '>=1.1.1',
    default_options: [
        'warning_level=3',
        'werror=true',
        'cpp_std=c++23',
        'buildtype=debugoptimized',
    ],
)

conf_data = configuration_data()
conf_data.set_quoted('OBJ_EVENT', '/xyz/openbmc_project/events')
conf_data.set_quoted('BUSNAME_EVENT', 'xyz.openbmc_project.Events')
conf_data.set_quoted(
    'EVENTS_PERSIST_PATH',
    '/var/lib/phosphor-dbus-monitor/events',
)

conf_data.set('CLASS_VERSION', 1)
conf_data.set('MAX_EVENTS', 20)

cpp = meson.get_compiler('cpp')
# Get Cereal dependency.
cereal_dep = dependency('cereal', required: false)
has_cereal = cpp.has_header_symbol(
    'cereal/cereal.hpp',
    'cereal::specialize',
    dependencies: cereal_dep,
    required: false,
)
if not has_cereal
    cereal_opts = import('cmake').subproject_options()
    cereal_opts.add_cmake_defines(
        {'BUILD_TESTS': 'OFF', 'SKIP_PERFORMANCE_COMPARISON': 'ON'},
    )
    cereal_proj = import('cmake').subproject(
        'cereal',
        options: cereal_opts,
        required: false,
    )
    assert(cereal_proj.found(), 'cereal is required')
    cereal_dep = cereal_proj.dependency('cereal')
endif

sdbusplus_dep = dependency('sdbusplus', required: false)
sdeventplus_dep = dependency('sdeventplus')
phosphor_dbus_interfaces_dep = dependency('phosphor-dbus-interfaces')
phosphor_logging_dep = dependency('phosphor-logging')
phosphor_snmp_dep = dependency('phosphor-snmp')

sdbusplus_python_env = {}
if not sdbusplus_dep.found()
    sdbusplus_proj = subproject('sdbusplus')
    sdbusplus_dep = sdbusplus_proj.get_variable('sdbusplus_dep')
    sdbusplus_python_env = {
        'PYTHONPATH': meson.current_source_dir() / 'subprojects' / 'sdbusplus' / 'tools',
    }
endif

prog_python = find_program('python3', required: true)
realpath_prog = find_program('realpath')

configure_file(output: 'config.h', configuration: conf_data)

subdir('src')
subdir('mslverify')
