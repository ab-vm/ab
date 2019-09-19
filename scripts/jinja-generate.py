#!/usr/bin/env python3

# jinja-generate <in> <out>
#
# A utility script for expanding jinja templates.
#

import yaml
import jinja2
import argparse
import os
import errno
import sys
import re

# split identifier on "_", ".", or " "
def identifier_subwords(identifier):
	return re.split("_|\.| ", identifier)

# convert abc_def to abc_def (left alone for var.)
def varify(name):
	return name
	# subwords = identifier_subwords(name)
	# if 0 == len(subwords):
	# 	return ""
	# result = subwords[0]
	# for subword in subwords[1:]:
	# 	result += subword.capitalize()
	# return result

# convert abc_def to AbcDef
def typeify(name):
	return "".join([word.capitalize() for word in identifier_subwords(name)])

# convert abc_def to ABC_DEF
def constify(name):
	return "_".join([word.upper() for word in identifier_subwords(name) ])

def render_template(template, data, include_dirs=[]):
	env = jinja2.Environment(
		block_start_string='@[',
		block_end_string=']',
		variable_start_string='@(',
		variable_end_string=')',
		comment_start_string="@#",
		comment_end_string="#",
		loader=jinja2.FileSystemLoader(["./", "/"] + include_dirs, followlinks=True),
		autoescape=jinja2.select_autoescape(['html', 'xml']),
		trim_blocks=True,
		keep_trailing_newline=True,
		extensions=[
			"jinja2.ext.loopcontrols",
			"jinja2.ext.do"
		]
	)
	env.filters['typeify'] = typeify
	env.filters['varify'] = varify
	env.filters['constify'] = constify
	return env.get_template(template).render(data=data)

def load_data(data_dir):
	data = {}
	for dirname, subdirnames, filenames in os.walk(data_dir):
		for filename in filenames:
			f = os.path.join(dirname, filename)
			e = os.path.splitext(filename)[0]
			data[e] = yaml.load(open(f, "r"))
	return data

def parse_args():
	parser = argparse.ArgumentParser(description="A Jinja template processor for C++")
	parser.add_argument("IN", help="input jinja template (required)")
	parser.add_argument("OUT", default="-", nargs="?", help="output filename (default stdout)")
	parser.add_argument("-d", "--data-dir", action="append", help="directory containing yaml datafiles")
	parser.add_argument("-i", "--include-dir", action="append", help="directory containing yaml datafiles")
	return parser.parse_args()

def ensure_dirname_exists(filename):
	dirname = os.path.dirname(filename)
	os.makedirs(dirname, exist_ok = True)

def main():
	cfg = parse_args()

	data = {}

	if cfg.data_dir:
		for dir in cfg.data_dir:
			data.update(load_data(dir))

	include_dirs = cfg.include_dir or []

	try:
		render = render_template(cfg.IN, data, include_dirs)
	except jinja2.TemplateSyntaxError as err:
		print("error: {}:{}: {}".format(err.filename, err.lineno, err.message))
		exit(1)

	if cfg.OUT == "-":
		sys.stdout.write(render)
	else:
		ensure_dirname_exists(cfg.OUT)
		with open(cfg.OUT, "w") as outfile:
			outfile.write(render)

main()
