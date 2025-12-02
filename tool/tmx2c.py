#!/usr/bin/env python3

import sys
import xml.etree.ElementTree as et

if len(sys.argv) < 3:
	print("Usage: %s SOURCE DEST" % sys.argv[0])
	exit(1)

name = sys.argv[1].replace(".", "_")
name = name.split("/")[-1]

tree = et.parse(sys.argv[1])
root = tree.getroot()

width  = int(root.find("layer").get("width"))
height = int(root.find("layer").get("height"))

with open(sys.argv[2], "w") as out:
	out.write(f"u16 {name}_w = {str(width)};\n");
	out.write(f"u16 {name}_h = {str(height)};\n");
	out.write(f"u16 {name}_data[] = {{\n")

	layers = 0
	for layer in root.findall("layer"):
		data = ""
		for line in layer.find("data").text.split("\n"):
			data += f"{line}\n	"
		data = data[:-3]

		out.write(f"{data},")

		layers += 1

	out.write("};")

	out.write(f"u16 {name}_layers = {layers};\n")
