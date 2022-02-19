#!/usr/bin/env python3

#@	{"dependencies": [{"ref":"glslangValidator", "origin":"system"}]}

import sys
import json
import subprocess
import tempfile
import os

def compile(args):
	targets = args['targets']
	source_file = args['source_file']
	with tempfile.TemporaryDirectory(suffix = None, prefix = 'maike_' + args['build_info']['build_id']) as tmpdir:
		new_source_file = tmpdir + '/' + os.path.basename(source_file)
		with open(new_source_file, 'wb') as tmpfile:
			tmpfile.write('#version 450\n'.encode())
			cpp = subprocess.run(['cpp', '-P', source_file], stdout=subprocess.PIPE)
			tmpfile.write(cpp.stdout)

		result = subprocess.run(['glslangValidator', '-V', '-Os', '-o', targets[0], new_source_file]);

	for target in args['targets'][1:]:
		shutil.copyfile(args['targets'][0], target)

	return result.returncode

if __name__ == '__main__':
	if sys.argv[1] == 'compile':
		exit(compile(json.loads(sys.argv[2])))