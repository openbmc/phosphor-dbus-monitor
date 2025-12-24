gtest_dep = dependency('gtest', main: true, disabler: true, required: false)
gmock_dep = dependency('gmock', disabler: true, required: false)
if not gtest_dep.found() or not gmock_dep.found()
    gtest_proj = import('cmake').subproject('googletest', required: false)
    if gtest_proj.found()
        gtest_dep = declare_dependency(
            dependencies: [
                dependency('threads'),
                gtest_proj.dependency('gtest'),
                gtest_proj.dependency('gtest_main'),
            ],
        )
        gmock_dep = gtest_proj.dependency('gmock')
    else
        assert(
            not get_option('tests').enabled(),
            'Googletest is required if tests are enabled',
        )
    endif
endif

pathgentest_hpp = custom_target(
    'pathgentest.hpp',
    command: [
        prog_python,
        meson.project_source_root() + '/src/pdmgen.py',
        '-t',
        'pathgentest.hpp.mako',
        '-p',
        meson.project_source_root() + '/src/test/templates',
        '-d',
        meson.project_source_root() + '/src/test/yaml/pathgentest',
        '-o',
        meson.current_build_dir() + '/pathgentest.hpp',
        '-e',
        meson.current_build_dir() + '/pathgentest-errors.hpp',
        'generate-cpp',
    ],
    input: [
        'templates/pathgentest.hpp.mako',
        'yaml/pathgentest/groupone.yaml',
        'yaml/pathgentest/grouptwo.yaml',
    ],
    env: sdbusplus_python_env,
    output: 'pathgentest.hpp',
)

pathgentest_sources = [pathgentest_hpp, 'pathgentest.cpp']

test(
    'pathgentest.cpp',
    executable(
        'pathgentest',
        pathgentest_sources,
        build_by_default: false,
        include_directories: ['..'],
        dependencies: [gtest_dep, gmock_dep, phosphor_dbus_interfaces_dep],
    ),
)


propertygentest_hpp = custom_target(
    'propertygentest.hpp',
    command: [
        prog_python,
        meson.project_source_root() + '/src/pdmgen.py',
        '-t',
        'propertygentest.hpp.mako',
        '-p',
        meson.project_source_root() + '/src/test/templates',
        '-d',
        meson.project_source_root() + '/src/test/yaml/propertygentest',
        '-o',
        meson.current_build_dir() + '/propertygentest.hpp',
        '-e',
        meson.current_build_dir() + '/propertygentest-errors.hpp',
        'generate-cpp',
    ],
    input: [
        'templates/propertygentest.hpp.mako',
        'yaml/propertygentest/groupone.yaml',
        'yaml/propertygentest/grouptwo.yaml',
    ],
    env: sdbusplus_python_env,
    output: 'propertygentest.hpp',
)

propertygentest_sources = [propertygentest_hpp, 'propertygentest.cpp']

test(
    'propertygentest.cpp',
    executable(
        'propertygentest',
        propertygentest_sources,
        build_by_default: false,
        include_directories: ['..'],
        dependencies: [gtest_dep, gmock_dep, phosphor_dbus_interfaces_dep],
    ),
)


propertywatchgentest_hpp = custom_target(
    'propertywatchgentest.hpp',
    command: [
        prog_python,
        meson.project_source_root() + '/src/pdmgen.py',
        '-t',
        'propertywatchgentest.hpp.mako',
        '-p',
        meson.project_source_root() + '/src/test/templates',
        '-d',
        meson.project_source_root() + '/src/test/yaml/propertywatchgentest',
        '-o',
        meson.current_build_dir() + '/propertywatchgentest.hpp',
        '-e',
        meson.current_build_dir() + '/propertywatchgentest-errors.hpp',
        'generate-cpp',
    ],
    input: [
        'templates/propertywatchgentest.hpp.mako',
        'yaml/propertywatchgentest/watchone.yaml',
        'yaml/propertywatchgentest/watchtwo.yaml',
    ],
    env: sdbusplus_python_env,
    output: 'propertywatchgentest.hpp',
)

propertywatchgentest_sources = [
    propertywatchgentest_hpp,
    'propertywatchgentest.cpp',
]

test(
    'propertywatchgentest.cpp',
    executable(
        'propertywatchgentest',
        propertywatchgentest_sources,
        build_by_default: false,
        include_directories: ['..'],
        dependencies: [gtest_dep, gmock_dep, phosphor_dbus_interfaces_dep],
    ),
)


callbackgentest_hpp = custom_target(
    'callbackgentest.hpp',
    command: [
        prog_python,
        meson.project_source_root() + '/src/pdmgen.py',
        '-t',
        'callbackgentest.hpp.mako',
        '-p',
        meson.project_source_root() + '/src/test/templates',
        '-d',
        meson.project_source_root() + '/src/test/yaml/callbackgentest',
        '-o',
        meson.current_build_dir() + '/callbackgentest.hpp',
        '-e',
        meson.current_build_dir() + '/callbackgentest-errors.hpp',
        'generate-cpp',
    ],
    input: [
        'templates/callbackgentest.hpp.mako',
        'yaml/callbackgentest/one.yaml',
        'yaml/callbackgentest/two.yaml',
    ],
    env: sdbusplus_python_env,
    output: 'callbackgentest.hpp',
)

callbackgentest_sources = [callbackgentest_hpp, 'callbackgentest.cpp']

test(
    'callbackgentest.cpp',
    executable(
        'callbackgentest',
        callbackgentest_sources,
        build_by_default: false,
        include_directories: ['..'],
        dependencies: [gtest_dep, gmock_dep, phosphor_dbus_interfaces_dep],
    ),
)


callbackgroupgentest_hpp = custom_target(
    'callbackgroupgentest.hpp',
    command: [
        prog_python,
        meson.project_source_root() + '/src/pdmgen.py',
        '-t',
        'callbackgroupgentest.hpp.mako',
        '-p',
        meson.project_source_root() + '/src/test/templates',
        '-d',
        meson.project_source_root() + '/src/test/yaml/callbackgroupgentest',
        '-o',
        meson.current_build_dir() + '/callbackgroupgentest.hpp',
        '-e',
        meson.current_build_dir() + '/callbackgroupgentest-errors.hpp',
        'generate-cpp',
    ],
    input: [
        'templates/callbackgroupgentest.hpp.mako',
        'yaml/callbackgroupgentest/test.yaml',
    ],
    env: sdbusplus_python_env,
    output: 'callbackgroupgentest.hpp',
)

callbackgroupgentest_sources = [
    callbackgroupgentest_hpp,
    'callbackgroupgentest.cpp',
]

test(
    'callbackgroupgentest.cpp',
    executable(
        'callbackgroupgentest',
        callbackgroupgentest_sources,
        build_by_default: false,
        include_directories: ['..'],
        dependencies: [gtest_dep, gmock_dep, phosphor_dbus_interfaces_dep],
    ),
)


conditiongentest_hpp = custom_target(
    'conditiongentest.hpp',
    command: [
        prog_python,
        meson.project_source_root() + '/src/pdmgen.py',
        '-t',
        'conditiongentest.hpp.mako',
        '-p',
        meson.project_source_root() + '/src/test/templates',
        '-d',
        meson.project_source_root() + '/src/test/yaml/conditiongentest',
        '-o',
        meson.current_build_dir() + '/conditiongentest.hpp',
        '-e',
        meson.current_build_dir() + '/conditiongentest-errors.hpp',
        'generate-cpp',
    ],
    input: [
        'templates/conditiongentest.hpp.mako',
        'yaml/conditiongentest/test.yaml',
    ],
    env: sdbusplus_python_env,
    output: 'conditiongentest.hpp',
)

conditiongentest_sources = [conditiongentest_hpp, 'conditiongentest.cpp']

test(
    'conditiongentest.cpp',
    executable(
        'conditiongentest',
        conditiongentest_sources,
        build_by_default: false,
        include_directories: ['..'],
        dependencies: [gtest_dep, gmock_dep, phosphor_dbus_interfaces_dep],
    ),
)


callbacktest_sources = [
    generated_hpp,
    'callbacktest.cpp',
    '../journal.cpp',
    '../elog.cpp',
    '../resolve_errors.cpp',
    '../event_manager.cpp',
    '../event_serialize.cpp',
]

callbacktest_deps = [
    cereal_dep,
    sdbusplus_dep,
    sdeventplus_dep,
    phosphor_dbus_interfaces_dep,
    phosphor_logging_dep,
    gtest_dep,
    gmock_dep,
]

test(
    'callbacktest.cpp',
    executable(
        'callbacktest',
        callbacktest_sources,
        build_by_default: false,
        include_directories: ['..', '../../'],
        dependencies: callbacktest_deps,
    ),
)


interfaceaddtest_hpp = custom_target(
    'interfaceaddtest.hpp',
    command: [
        prog_python,
        meson.project_source_root() + '/src/pdmgen.py',
        '-t',
        'interfaceaddtest.hpp.mako',
        '-p',
        meson.project_source_root() + '/src/test/templates',
        '-d',
        meson.project_source_root() + '/src/test/yaml/interfaceaddtest',
        '-o',
        meson.current_build_dir() + '/interfaceaddtest.hpp',
        '-e',
        meson.current_build_dir() + '/interfaceaddtest-errors.hpp',
        'generate-cpp',
    ],
    input: [
        'templates/interfaceaddtest.hpp.mako',
        'yaml/interfaceaddtest/one.yaml',
    ],
    env: sdbusplus_python_env,
    output: 'interfaceaddtest.hpp',
)

interfaceaddtest_sources = [interfaceaddtest_hpp, 'interfaceaddtest.cpp']

test(
    'interfaceaddtest.cpp',
    executable(
        'interfaceaddtest',
        interfaceaddtest_sources,
        build_by_default: false,
        include_directories: ['..'],
        dependencies: [
            gtest_dep,
            gmock_dep,
            phosphor_dbus_interfaces_dep,
            sdeventplus_dep,
        ],
    ),
)
