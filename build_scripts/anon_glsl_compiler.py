#!/usr/bin/env python3

#@	{"dependencies": [{"ref":"glslangValidator", "origin":"system"}, {"ref":"anon", "origin":"system"}]}

import sys
import json
import subprocess
import tempfile
import os
import anonpy
from string import Template

def gen_header(src, prog_name, task_id):
	header_source = Template('''#ifndef $include_guard
#define $include_guard

#include "engine/shaders/shader_source.hpp"
#include "engine/utils/vec_t.hpp"

#include <tuple>
#include <span>

namespace $namespaces
{
	struct $prog_name
	{
		using input_port_types = std::tuple<$input_port_types>;
		static ::idis::shaders::vertex_shader_source<std::span<uint32_t const>> vertex_shader();
		static ::idis::shaders::vertex_shader_source<std::span<uint32_t const>> fragment_shader();
		static constexpr auto num_inputs = std::tuple_size_v<input_port_types>;
	};
}

#endif
''')
	subst = dict()
	subst['include_guard'] = 'GEN_shader_%s_%s_HPP'%(prog_name, task_id)
	subst['namespaces'] = '::'.join(src['namespaces'])
	subst['prog_name'] = prog_name
	inputs = []
	for obj in src['vertex_shader']['inputs']:
		inputs.append('::idis::%s_t'%obj['type'])
	subst['input_port_types'] = ', '.join(inputs)

	return header_source.substitute(subst)

def write_header(src, filename):
	with open(filename, 'wb') as output:
		output.write(src.encode())
	return 0

def gen_shader_source(shader):
	glsl_input = Template('layout(location = $index) in $type $name;')
	glsl_output = Template('layout(location = $index) out $type $name;')
	glsl_source = Template('''#version 450

$inputs

$outputs

$code
''')

	inputs = []
	for k, obj in enumerate(shader['inputs']):
		obj_with_index = obj
		obj_with_index['index'] = k
		inputs.append(glsl_input.substitute(obj_with_index))

	outputs = []
	for k, obj in enumerate(shader['outputs']):
		obj_with_index = obj
		obj_with_index['index'] = k
		outputs.append(glsl_output.substitute(obj_with_index))

	subst = dict()
	subst['inputs'] = '\n'.join(inputs)
	subst['outputs'] = '\n'.join(outputs)
	subst['code'] = shader['code']

	return glsl_source.substitute(subst)

def compile_shader_src(src, name, dir, output):
	source_file = dir + '/' + name
	with open(source_file, 'wb') as tmpfile:
		tmpfile.write(src.encode())
	return subprocess.run(['glslangValidator', '-V', '-Os', '-o', output, source_file]).returncode;

def compile(args):
	targets = args['targets']
	source_file = args['source_file']

	with tempfile.TemporaryDirectory(suffix = None, prefix = 'maike_' + args['build_info']['build_id']) as tmpdir:
		src = anonpy.load_from_path(source_file)
		prog_name = os.path.basename(source_file).split('.')[0]
		results = []
		results.append(write_header(gen_header(src, prog_name, args['task_id']), targets[0]))
		results.append(compile_shader_src(gen_shader_source(src['vertex_shader']), prog_name + '.vert', tmpdir, targets[1]))
		results.append(compile_shader_src(gen_shader_source(src['fragment_shader']), prog_name + '.frag', tmpdir, targets[2]))
		if any(results):
			return 1
	return 0

def get_tags(args):
	source_file = args['source_file']
	ret = dict()
	prog_name = os.path.basename(source_file).split('.')[0]
	ret['targets'] = [{'name': prog_name + '.gen.hpp'}, {'name': prog_name + '.vert.spv'}, {'name': prog_name + '.frag.spv'}]
	print(json.dumps(ret))
	return 0

if __name__ == '__main__':
	if sys.argv[1] == 'compile':
		exit(compile(json.loads(sys.argv[2])))
	if sys.argv[1] == 'get_tags':
		exit(get_tags(json.loads(sys.argv[2])))
