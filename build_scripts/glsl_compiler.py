#!/usr/bin/env python3

#@	{"dependencies": [{"ref":"glslangValidator", "origin":"system"}]}

import sys
import json
import subprocess

def compile(args):
	targets = args['targets']
	source_file = args['source_file']
	result = subprocess.run(['glslangValidator', '-V', '-Os', '-o', targets[0], source_file]);

	for target in args['targets'][1:]:
		shutil.copyfile(args['targets'][0], target)

	return result.returncode

if __name__ == '__main__':
	if sys.argv[1] == 'compile':
		exit(compile(json.loads(sys.argv[2])))