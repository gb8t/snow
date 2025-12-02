#!/usr/bin/env python3

import sys

if len(sys.argv) < 3:
	print("Usage: %s SOURCE DEST" % sys.argv[0])
	exit(1)

name = sys.argv[1].replace(".", "_")
name = name.split("/")[-1]

with open(sys.argv[2], "w") as out:
	out.write(f"u8 {name}_data[] = {{\n	")
	size = 0
	for b in open(sys.argv[1], "rb").read():
		size += 1
		out.write("%d," % b)
		if (size % 16 == 0):
			out.write("\n	")
	out.write("\n};\n")
	out.write(f"u32 {name}_len = {str(size)};\n");
