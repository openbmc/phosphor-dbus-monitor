sources_mslverify = ['verify.cpp']

deps_mslverify = [
    sdbusplus_dep,
    phosphor_dbus_interfaces_dep,
    phosphor_logging_dep,
]

executable(
    'phosphor-msl-verify',
    sources_mslverify,
    implicit_include_directories: true,
    dependencies: deps_mslverify,
    install: true,
    install_dir: get_option('bindir'),
)
