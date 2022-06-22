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
            ]
        )
        gmock_dep = gtest_proj.dependency('gmock')
    else
        assert(
            not get_option('tests').enabled(),
            'Googletest is required if tests are enabled'
        )
    endif
endif

pathgentest_hpp = custom_target(
    'pathgentest.hpp',
    command: [
        prog_python,
        meson.project_source_root() + '/src/pdmgen.py',
        '-t', 'pathgentest.mako.hpp',
        '-p', meson.project_source_root() + '/src/test/templates',
        '-d', meson.project_source_root() + '/src/test/yaml/pathgentest',
        '-o', meson.current_build_dir() + '/pathgentest.hpp',
        'generate-cpp'
    ],
    input: [
        'templates/pathgentest.mako.hpp',
        'yaml/pathgentest/groupone.yaml',
        'yaml/pathgentest/grouptwo.yaml',
    ],
    output: 'pathgentest.hpp',
)

pathgentest_sources = [
    pathgentest_hpp,
    'pathgentest.cpp',
]

test('pathgentest.cpp',
  executable(
    'pathgentest',
    pathgentest_sources,
    build_by_default: false,
    include_directories: ['..'],
    dependencies: [gtest_dep, gmock_dep]
  ))


propertygentest_hpp = custom_target(
    'propertygentest.hpp',
    command: [
        prog_python,
        meson.project_source_root() + '/src/pdmgen.py',
        '-t', 'propertygentest.mako.hpp',
        '-p', meson.project_source_root() + '/src/test/templates',
        '-d', meson.project_source_root() + '/src/test/yaml/propertygentest',
        '-o', meson.current_build_dir() + '/propertygentest.hpp',
        'generate-cpp'
    ],
    input: [
        'templates/propertygentest.mako.hpp',
        'yaml/propertygentest/groupone.yaml',
        'yaml/propertygentest/grouptwo.yaml',
    ],
    output: 'propertygentest.hpp',
)

propertygentest_sources = [
    propertygentest_hpp,
    'propertygentest.cpp',
]

test('propertygentest.cpp',
  executable(
    'propertygentest',
    propertygentest_sources,
    build_by_default: false,
    include_directories: ['..'],
    dependencies: [gtest_dep, gmock_dep]
  ))


propertywatchgentest_hpp = custom_target(
    'propertywatchgentest.hpp',
    command: [
        prog_python,
        meson.project_source_root() + '/src/pdmgen.py',
        '-t', 'propertywatchgentest.mako.hpp',
        '-p', meson.project_source_root() + '/src/test/templates',
        '-d', meson.project_source_root() + '/src/test/yaml/propertywatchgentest',
        '-o', meson.current_build_dir() + '/propertywatchgentest.hpp',
        'generate-cpp'
    ],
    input: [
        'templates/propertywatchgentest.mako.hpp',
        'yaml/propertywatchgentest/watchone.yaml',
        'yaml/propertywatchgentest/watchtwo.yaml',
    ],
    output: 'propertywatchgentest.hpp',
)

propertywatchgentest_sources = [
    propertywatchgentest_hpp,
    'propertywatchgentest.cpp',
]

test('propertywatchgentest.cpp',
  executable(
    'propertywatchgentest',
    propertywatchgentest_sources,
    build_by_default: false,
    include_directories: ['..'],
    dependencies: [gtest_dep, gmock_dep]
  ))


callbackgentest_hpp = custom_target(
    'callbackgentest.hpp',
    command: [
        prog_python,
        meson.project_source_root() + '/src/pdmgen.py',
        '-t', 'callbackgentest.mako.hpp',
        '-p', meson.project_source_root() + '/src/test/templates',
        '-d', meson.project_source_root() + '/src/test/yaml/callbackgentest',
        '-o', meson.current_build_dir() + '/callbackgentest.hpp',
        'generate-cpp'
    ],
    input: [
        'templates/callbackgentest.mako.hpp',
        'yaml/callbackgentest/one.yaml',
        'yaml/callbackgentest/two.yaml',
    ],
    output: 'callbackgentest.hpp',
)

callbackgentest_sources = [
    callbackgentest_hpp,
    'callbackgentest.cpp',
]

test('callbackgentest.cpp',
  executable(
    'callbackgentest',
    callbackgentest_sources,
    build_by_default: false,
    include_directories: ['..'],
    dependencies: [gtest_dep, gmock_dep]
  ))


callbackgroupgentest_hpp = custom_target(
    'callbackgroupgentest.hpp',
    command: [
        prog_python,
        meson.project_source_root() + '/src/pdmgen.py',
        '-t', 'callbackgroupgentest.mako.hpp',
        '-p', meson.project_source_root() + '/src/test/templates',
        '-d', meson.project_source_root() + '/src/test/yaml/callbackgroupgentest',
        '-o', meson.current_build_dir() + '/callbackgroupgentest.hpp',
        'generate-cpp'
    ],
    input: [
        'templates/callbackgroupgentest.mako.hpp',
        'yaml/callbackgroupgentest/test.yaml',
    ],
    output: 'callbackgroupgentest.hpp',
)

callbackgroupgentest_sources = [
    callbackgroupgentest_hpp,
    'callbackgroupgentest.cpp',
]

test('callbackgroupgentest.cpp',
  executable(
    'callbackgroupgentest',
    callbackgroupgentest_sources,
    build_by_default: false,
    include_directories: ['..'],
    dependencies: [gtest_dep, gmock_dep]
  ))


conditiongentest_hpp = custom_target(
    'conditiongentest.hpp',
    command: [
        prog_python,
        meson.project_source_root() + '/src/pdmgen.py',
        '-t', 'conditiongentest.mako.hpp',
        '-p', meson.project_source_root() + '/src/test/templates',
        '-d', meson.project_source_root() + '/src/test/yaml/conditiongentest',
        '-o', meson.current_build_dir() + '/conditiongentest.hpp',
        'generate-cpp'
    ],
    input: [
        'templates/conditiongentest.mako.hpp',
        'yaml/conditiongentest/test.yaml',
    ],
    output: 'conditiongentest.hpp',
)

conditiongentest_sources = [
    conditiongentest_hpp,
    'conditiongentest.cpp',
]

test('conditiongentest.cpp',
  executable(
    'conditiongentest',
    conditiongentest_sources,
    build_by_default: false,
    include_directories: ['..'],
    dependencies: [gtest_dep, gmock_dep]
  ))


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
    sdbusplus_dep,
    sdeventplus_dep,
    phosphor_dbus_interfaces_dep,
    phosphor_logging_dep,
    gtest_dep,
    gmock_dep,
]

test('callbacktest.cpp',
  executable(
    'callbacktest',
    callbacktest_sources,
    build_by_default: false,
    include_directories: ['..', '../../'],
    dependencies: callbacktest_deps,
  ))


interfaceaddtest_hpp = custom_target(
    'interfaceaddtest.hpp',
    command: [
        prog_python,
        meson.project_source_root() + '/src/pdmgen.py',
        '-t', 'interfaceaddtest.mako.hpp',
        '-p', meson.project_source_root() + '/src/test/templates',
        '-d', meson.project_source_root() + '/src/test/yaml/interfaceaddtest',
        '-o', meson.current_build_dir() + '/interfaceaddtest.hpp',
        'generate-cpp'
    ],
    input: [
        'templates/interfaceaddtest.mako.hpp',
        'yaml/interfaceaddtest/one.yaml',
    ],
    output: 'interfaceaddtest.hpp',
)

interfaceaddtest_sources = [
    interfaceaddtest_hpp,
    'interfaceaddtest.cpp',
]

test('interfaceaddtest.cpp',
  executable(
    'interfaceaddtest',
    interfaceaddtest_sources,
    build_by_default: false,
    include_directories: ['..'],
    dependencies: [gtest_dep, gmock_dep]
  ))
