#!/usr/bin/env python

'''Phosphor DBus Monitor YAML parser and code generator.

The parser workflow is broken down as follows:
  1 - Import YAML files as native python type(s) instance(s).
  2 - Create an instance of the Everything class from the
        native python type instance(s) with the Everything.load
        method.
  3 - The Everything class constructor orchestrates conversion of the
        native python type(s) instances(s) to render helper types.
        Each render helper type constructor imports its attributes
        from the native python type(s) instances(s).
  4 - Present the converted YAML to the command processing method
        requested by the script user.
'''

import os
import sys
import yaml
import mako.lookup
from argparse import ArgumentParser
from sdbusplus.renderer import Renderer
from sdbusplus.namedelement import NamedElement
import sdbusplus.property


class InvalidConfigError(BaseException):
    '''General purpose config file parsing error.'''

    def __init__(self, path, msg):
        '''Display configuration file with the syntax
        error and the error message.'''

        self.config = path
        self.msg = msg


class NotUniqueError(InvalidConfigError):
    '''Within a config file names must be unique.
    Display the config file with the duplicate and
    the duplicate itself.'''

    def __init__(self, path, cls, *names):
        fmt = 'Duplicate {0}: "{1}"'
        super(NotUniqueError, self).__init__(
            path, fmt.format(cls, ' '.join(names)))


def get_index(objs, cls, name, config=None):
    '''Items are usually rendered as C++ arrays and as
    such are stored in python lists.  Given an item name
    its class, and an optional config file filter, find
    the item index.'''

    for i, x in enumerate(objs.get(cls, [])):
        if config and x.configfile != config:
            continue
        if x.name != name:
            continue

        return i
    raise InvalidConfigError(config, 'Could not find name: "{0}"'.format(name))


def exists(objs, cls, name, config=None):
    '''Check to see if an item already exists in a list given
    the item name.'''

    try:
        get_index(objs, cls, name, config)
    except:
        return False

    return True


def add_unique(obj, *a, **kw):
    '''Add an item to one or more lists unless already present,
    with an option to constrain the search to a specific config file.'''

    for container in a:
        if not exists(container, obj.cls, obj.name, config=kw.get('config')):
            container.setdefault(obj.cls, []).append(obj)


class Indent(object):
    '''Help templates be depth agnostic.'''

    def __init__(self, depth=0):
        self.depth = depth

    def __add__(self, depth):
        return Indent(self.depth + depth)

    def __call__(self, depth):
        '''Render an indent at the current depth plus depth.'''
        return 4*' '*(depth + self.depth)


class ConfigEntry(NamedElement):
    '''Base interface for rendered items.'''

    def __init__(self, *a, **kw):
        '''Pop the configfile/class/subclass keywords.'''

        self.configfile = kw.pop('configfile')
        self.cls = kw.pop('class')
        self.subclass = kw.pop(self.cls)
        super(ConfigEntry, self).__init__(**kw)

    def factory(self, objs):
        ''' Optional factory interface for subclasses to add
        additional items to be rendered.'''

        pass

    def setup(self, objs):
        ''' Optional setup interface for subclasses, invoked
        after all factory methods have been run.'''

        pass


class Path(ConfigEntry):
    '''Path/metadata association.'''

    def __init__(self, *a, **kw):
        super(Path, self).__init__(**kw)

    def factory(self, objs):
        '''Create path and metadata elements.'''

        args = {
            'class': 'pathname',
            'pathname': 'element',
            'name': self.name['path']
        }
        add_unique(ConfigEntry(
            configfile=self.configfile, **args), objs)

        args = {
            'class': 'meta',
            'meta': 'element',
            'name': self.name['meta']
        }
        add_unique(ConfigEntry(
            configfile=self.configfile, **args), objs)

        super(Path, self).factory(objs)

    def setup(self, objs):
        '''Resolve path and metadata names to indicies.'''

        self.path = get_index(
            objs, 'pathname', self.name['path'])
        self.meta = get_index(
            objs, 'meta', self.name['meta'])

        super(Path, self).setup(objs)


class Property(ConfigEntry):
    '''Property/interface/metadata association.'''

    def __init__(self, *a, **kw):
        super(Property, self).__init__(**kw)

    def factory(self, objs):
        '''Create interface, property name and metadata elements.'''

        args = {
            'class': 'interface',
            'interface': 'element',
            'name': self.name['interface']
        }
        add_unique(ConfigEntry(
            configfile=self.configfile, **args), objs)

        args = {
            'class': 'propertyname',
            'propertyname': 'element',
            'name': self.name['property']
        }
        add_unique(ConfigEntry(
            configfile=self.configfile, **args), objs)

        args = {
            'class': 'meta',
            'meta': 'element',
            'name': self.name['meta']
        }
        add_unique(ConfigEntry(
            configfile=self.configfile, **args), objs)

        super(Property, self).factory(objs)

    def setup(self, objs):
        '''Resolve interface, property and metadata to indicies.'''

        self.interface = get_index(
            objs, 'interface', self.name['interface'])
        self.prop = get_index(
            objs, 'propertyname', self.name['property'])
        self.meta = get_index(
            objs, 'meta', self.name['meta'])

        super(Property, self).setup(objs)


class Instance(ConfigEntry):
    '''Property/Path association.'''

    def __init__(self, *a, **kw):
        super(Instance, self).__init__(**kw)

    def setup(self, objs):
        '''Resolve elements to indicies.'''

        self.interface = get_index(
            objs, 'interface', self.name['property']['interface'])
        self.prop = get_index(
            objs, 'propertyname', self.name['property']['property'])
        self.propmeta = get_index(
            objs, 'meta', self.name['property']['meta'])
        self.path = get_index(
            objs, 'pathname', self.name['path']['path'])
        self.pathmeta = get_index(
            objs, 'meta', self.name['path']['meta'])

        super(Instance, self).setup(objs)


class Group(ConfigEntry):
    '''Pop the members keyword for groups.'''

    def __init__(self, *a, **kw):
        self.members = kw.pop('members')
        super(Group, self).__init__(**kw)


class ImplicitGroup(Group):
    '''Provide a factory method for groups whose members are
    not explicitly declared in the config files.'''

    def __init__(self, *a, **kw):
        super(ImplicitGroup, self).__init__(**kw)

    def factory(self, objs):
        '''Create group members.'''

        factory = Everything.classmap(self.subclass, 'element')
        for m in self.members:
            args = {
                'class': self.subclass,
                self.subclass: 'element',
                'name': m
            }

            obj = factory(configfile=self.configfile, **args)
            add_unique(obj, objs)
            obj.factory(objs)

        super(ImplicitGroup, self).factory(objs)


class GroupOfPaths(ImplicitGroup):
    '''Path group config file directive.'''

    def __init__(self, *a, **kw):
        super(GroupOfPaths, self).__init__(**kw)

    def setup(self, objs):
        '''Resolve group members.'''

        def map_member(x):
            path = get_index(
                objs, 'pathname', x['path'])
            meta = get_index(
                objs, 'meta', x['meta'])
            return (path, meta)

        self.members = map(
            map_member,
            self.members)

        super(GroupOfPaths, self).setup(objs)


class GroupOfProperties(ImplicitGroup):
    '''Property group config file directive.'''

    def __init__(self, *a, **kw):
        self.datatype = sdbusplus.property.Property(
            name=kw.get('name'),
            type=kw.pop('type')).cppTypeName

        super(GroupOfProperties, self).__init__(**kw)

    def setup(self, objs):
        '''Resolve group members.'''

        def map_member(x):
            iface = get_index(
                objs, 'interface', x['interface'])
            prop = get_index(
                objs, 'propertyname', x['property'])
            meta = get_index(
                objs, 'meta', x['meta'])

            return (iface, prop, meta)

        self.members = map(
            map_member,
            self.members)

        super(GroupOfProperties, self).setup(objs)


class GroupOfInstances(ImplicitGroup):
    '''A group of property instances.'''

    def __init__(self, *a, **kw):
        super(GroupOfInstances, self).__init__(**kw)

    def setup(self, objs):
        '''Resolve group members.'''

        def map_member(x):
            path = get_index(objs, 'pathname', x['path']['path'])
            pathmeta = get_index(objs, 'meta', x['path']['meta'])
            interface = get_index(
                objs, 'interface', x['property']['interface'])
            prop = get_index(objs, 'propertyname', x['property']['property'])
            propmeta = get_index(objs, 'meta', x['property']['meta'])
            instance = get_index(objs, 'instance', x)

            return (path, pathmeta, interface, prop, propmeta, instance)

        self.members = map(
            map_member,
            self.members)

        super(GroupOfInstances, self).setup(objs)


class HasPropertyIndex(ConfigEntry):
    '''Handle config file directives that require an index to be
    constructed.'''

    def __init__(self, *a, **kw):
        self.paths = kw.pop('paths')
        self.properties = kw.pop('properties')
        super(HasPropertyIndex, self).__init__(**kw)

    def factory(self, objs):
        '''Create a group of instances for this index.'''

        members = []
        path_group = get_index(
            objs, 'pathgroup', self.paths, config=self.configfile)
        property_group = get_index(
            objs, 'propertygroup', self.properties, config=self.configfile)

        for path in objs['pathgroup'][path_group].members:
            for prop in objs['propertygroup'][property_group].members:
                member = {
                    'path': path,
                    'property': prop,
                }
                members.append(member)

        args = {
            'members': members,
            'class': 'instancegroup',
            'instancegroup': 'instance',
            'name': '{0} {1}'.format(self.paths, self.properties)
        }

        group = GroupOfInstances(configfile=self.configfile, **args)
        add_unique(group, objs, config=self.configfile)
        group.factory(objs)

        super(HasPropertyIndex, self).factory(objs)

    def setup(self, objs):
        '''Resolve path, property, and instance groups.'''

        self.instances = get_index(
            objs,
            'instancegroup',
            '{0} {1}'.format(self.paths, self.properties),
            config=self.configfile)
        self.paths = get_index(
            objs,
            'pathgroup',
            self.paths,
            config=self.configfile)
        self.properties = get_index(
            objs,
            'propertygroup',
            self.properties,
            config=self.configfile)
        self.datatype = objs['propertygroup'][self.properties].datatype

        super(HasPropertyIndex, self).setup(objs)


class PropertyWatch(HasPropertyIndex):
    '''Handle the property watch config file directive.'''

    def __init__(self, *a, **kw):
        super(PropertyWatch, self).__init__(**kw)


class Callback(HasPropertyIndex):
    '''Interface and common logic for callbacks.'''

    def __init__(self, *a, **kw):
        super(Callback, self).__init__(**kw)


class Journal(Callback, Renderer):
    '''Handle the journal callback config file directive.'''

    def __init__(self, *a, **kw):
        self.severity = kw.pop('severity')
        self.message = kw.pop('message')
        super(Journal, self).__init__(**kw)

    def construct(self, loader, indent):
        return self.render(
            loader,
            'journal.mako.cpp',
            c=self,
            indent=indent)


class Everything(Renderer):
    '''Parse/render entry point.'''

    @staticmethod
    def classmap(cls, sub=None):
        '''Map render item class and subclass entries to the appropriate
        handler methods.'''

        class_map = {
            'path': {
                'element': Path,
            },
            'pathgroup': {
                'path': GroupOfPaths,
            },
            'propertygroup': {
                'property': GroupOfProperties,
            },
            'property': {
                'element': Property,
            },
            'watch': {
                'property': PropertyWatch,
            },
            'instance': {
                'element': Instance,
            },
            'callback': {
                'journal': Journal,
            },
        }

        if cls not in class_map:
            raise NotImplementedError('Unknown class: "{0}"'.format(cls))
        if sub not in class_map[cls]:
            raise NotImplementedError('Unknown {0} type: "{1}"'.format(
                cls, sub))

        return class_map[cls][sub]

    @staticmethod
    def load_one_yaml(path, fd, objs):
        '''Parse a single YAML file.  Parsing occurs in three phases.
        In the first phase a factory method associated with each
        configuration file directive is invoked.  These factory
        methods generate more factory methods.  In the second
        phase the factory methods created in the first phase
        are invoked.  In the last phase a callback is invoked on
        each object created in phase two.  Typically the callback
        resolves references to other configuration file directives.'''

        factory_objs = {}
        for x in yaml.safe_load(fd.read()) or {}:

            # Create factory object for this config file directive.
            cls = x['class']
            sub = x.get(cls)
            if cls == 'group':
                cls = '{0}group'.format(sub)

            factory = Everything.classmap(cls, sub)
            obj = factory(configfile=path, **x)

            # For a given class of directive, validate the file
            # doesn't have any duplicate names (duplicates are
            # ok across config files).
            if exists(factory_objs, obj.cls, obj.name, config=path):
                raise NotUniqueError(path, cls, obj.name)

            factory_objs.setdefault(cls, []).append(obj)
            objs.setdefault(cls, []).append(obj)

        for cls, items in factory_objs.items():
            for obj in items:
                # Add objects for template consumption.
                obj.factory(objs)

    @staticmethod
    def load(args):
        '''Aggregate all the YAML in the input directory
        into a single aggregate.'''

        objs = {}
        yaml_files = filter(
            lambda x: x.endswith('.yaml'),
            os.listdir(args.inputdir))

        yaml_files.sort()

        for x in yaml_files:
            path = os.path.join(args.inputdir, x)
            with open(path, 'r') as fd:
                Everything.load_one_yaml(path, fd, objs)

        # Instruct objects to resolve their
        # cross config file directive references.
        for cls, items in objs.items():
            for obj in items:
                obj.setup(objs)

        return Everything(**objs)

    def __init__(self, *a, **kw):
        self.pathmeta = kw.pop('path', [])
        self.paths = kw.pop('pathname', [])
        self.meta = kw.pop('meta', [])
        self.pathgroups = kw.pop('pathgroup', [])
        self.interfaces = kw.pop('interface', [])
        self.properties = kw.pop('property', [])
        self.propertynames = kw.pop('propertyname', [])
        self.propertygroups = kw.pop('propertygroup', [])
        self.instances = kw.pop('instance', [])
        self.instancegroups = kw.pop('instancegroup', [])
        self.watches = kw.pop('watch', [])
        self.callbacks = kw.pop('callback', [])

        super(Everything, self).__init__(**kw)

    def generate_cpp(self, loader):
        '''Render the template with the provided data.'''
        with open(args.output, 'w') as fd:
            fd.write(
                self.render(
                    loader,
                    args.template,
                    meta=self.meta,
                    properties=self.properties,
                    propertynames=self.propertynames,
                    interfaces=self.interfaces,
                    paths=self.paths,
                    pathmeta=self.pathmeta,
                    pathgroups=self.pathgroups,
                    propertygroups=self.propertygroups,
                    instances=self.instances,
                    watches=self.watches,
                    instancegroups=self.instancegroups,
                    callbacks=self.callbacks,
                    indent=Indent()))

if __name__ == '__main__':
    script_dir = os.path.dirname(os.path.realpath(__file__))
    valid_commands = {
        'generate-cpp': 'generate_cpp',
    }

    parser = ArgumentParser(
        description='Phosphor DBus Monitor (PDM) YAML '
        'scanner and code generator.')

    parser.add_argument(
        "-o", "--out", dest="output",
        default='generated.cpp',
        help="Generated output file name and path.")
    parser.add_argument(
        '-t', '--template', dest='template',
        default='generated.mako.hpp',
        help='The top level template to render.')
    parser.add_argument(
        '-p', '--template-path', dest='template_search',
        default=script_dir,
        help='The space delimited mako template search path.')
    parser.add_argument(
        '-d', '--dir', dest='inputdir',
        default=os.path.join(script_dir, 'example'),
        help='Location of files to process.')
    parser.add_argument(
        'command', metavar='COMMAND', type=str,
        choices=valid_commands.keys(),
        help='%s.' % " | ".join(valid_commands.keys()))

    args = parser.parse_args()

    if sys.version_info < (3, 0):
        lookup = mako.lookup.TemplateLookup(
            directories=args.template_search.split(),
            disable_unicode=True)
    else:
        lookup = mako.lookup.TemplateLookup(
            directories=args.template_search.split())
    try:
        function = getattr(
            Everything.load(args),
            valid_commands[args.command])
        function(lookup)
    except InvalidConfigError as e:
        sys.stdout.write('{0}: {1}\n\n'.format(e.config, e.msg))
        raise
