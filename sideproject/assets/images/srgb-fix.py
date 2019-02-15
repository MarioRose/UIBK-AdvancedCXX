import os
import subprocess

for root, directories, filenames in os.walk('./'):
	for directory in directories:
		print os.path.join(root, directory) 
	for filename in filenames: 
		if filename.endswith(".png"):
			f = os.path.join(root, filename)
			print "convert " + f + " " + f
			subprocess.call(["convert", f, f])
