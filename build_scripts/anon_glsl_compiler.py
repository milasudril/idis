#!/usr/bin/env python3

#@	{"dependencies": [{"ref":"glslangValidator", "origin":"system"}, {"ref":"anon", "origin":"system"}]}

import sys
import json
import subprocess
import tempfile
import os
import anonpy
from string import Template

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
		results.append(compile_shader_src(gen_shader_source(src['vertex_shader']), prog_name + '.vert', tmpdir, targets[1]))
		results.append(compile_shader_src(gen_shader_source(src['fragment_shader']), prog_name + '.frag', tmpdir, targets[2]))
		if any(results):
			return 1
	return 0

def get_tags(args):
	source_file = args['source_file']
	ret = dict()
	prog_name = os.path.basename(source_file).split('.')[0]
	ret['targets'] = [{'name': prog_name + '.hpp'}, {'name': prog_name + '.vert.spv'}, {'name': prog_name + '.frag.spv'}]
	print(json.dumps(ret))
	return 0

if __name__ == '__main__':
	if sys.argv[1] == 'compile':
		exit(compile(json.loads(sys.argv[2])))
	if sys.argv[1] == 'get_tags':
		exit(get_tags(json.loads(sys.argv[2])))
