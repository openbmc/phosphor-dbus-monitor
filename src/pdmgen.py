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


class Indent(object):
    '''Help templates be depth agnostic.'''

    def __init__(self, depth=0):
        self.depth = depth

    def __add__(self, depth):
        return Indent(self.depth + depth)

    def __call__(self, depth):
        '''Render an indent at the current depth plus depth.'''
        return 4*' '*(depth + self.depth)


class Everything(Renderer):
    '''Parse/render entry point.'''

    @staticmethod
    def load(args):
        '''Aggregate all the YAML in the input directory
        into a single aggregate.'''

        if os.path.exists(args.inputdir):
            yaml_files = filter(
                lambda x: x.endswith('.yaml'),
                os.listdir(args.inputdir))

            for x in yaml_files:
                with open(os.path.join(args.inputdir, x), 'r') as fd:
                    yaml.safe_load(fd.read())

        return Everything()

    def __init__(self, *a, **kw):
        super(Everything, self).__init__(**kw)

    def generate_cpp(self, loader):
        '''Render the template with the provided data.'''
        with open(args.output, 'w') as fd:
            fd.write(
                self.render(
                    loader,
                    args.template,
                    events={},
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

    function = getattr(
        Everything.load(args),
        valid_commands[args.command])
    function(lookup)
