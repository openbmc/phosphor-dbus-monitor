templates_depends = files(
    'example/example.yaml',
    'example/test.yaml',
    'templates/callbackgroup.cpp.mako',
    'templates/conditional.cpp.mako',
    'templates/count.cpp.mako',
    'templates/elog.cpp.mako',
    'templates/errors.hpp.mako',
    'templates/event.cpp.mako',
    'templates/generated.hpp.mako',
    'templates/journal.cpp.mako',
    'templates/median.cpp.mako',
    'templates/method.cpp.mako',
    'templates/resolve_errors.cpp.mako',
)

yamldir = get_option('YAML_PATH')
if yamldir == ''
    yamldir = meson.project_source_root() + '/src/example'
endif

generated_hpp = custom_target(
    'generated.hpp',
    command: [
        prog_python,
        meson.project_source_root() + '/src/pdmgen.py',
        '-t',
        'generated.hpp.mako',
        '-p',
        meson.project_source_root() + '/src/templates',
        '-d',
        yamldir,
        '-o',
        meson.current_build_dir() + '/generated.hpp',
        '-e',
        meson.current_build_dir() + '/errors.hpp',
        'generate-cpp',
    ],
    depend_files: templates_depends,
    env: sdbusplus_python_env,
    output: 'generated.hpp',
)

sources = [
    generated_hpp,
    'elog.cpp',
    'event_manager.cpp',
    'event_serialize.cpp',
    'journal.cpp',
    'main.cpp',
    'pathwatch.cpp',
    'propertywatch.cpp',
    'resolve_errors.cpp',
    'snmp_trap.cpp',
]

deps = [
    cereal_dep,
    sdbusplus_dep,
    sdeventplus_dep,
    phosphor_dbus_interfaces_dep,
    phosphor_logging_dep,
    phosphor_snmp_dep,
]

executable(
    'phosphor-dbus-monitor',
    sources,
    include_directories: ['..'],
    implicit_include_directories: true,
    dependencies: deps,
    install: true,
    install_dir: get_option('bindir'),
)

build_tests = get_option('tests')
if build_tests.allowed()
    subdir('test')
endif
