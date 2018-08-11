#!/usr/bin/python3

### Not gonna work! I can't get symlinks showing up.

import os
import itertools

# change to be in the dir where these will be living, so as to work relatively
workingdir = "files"
os.chdir(workingdir)

# how many threads we are forked onto
cpus = 6

# number of directories is len(characterset)**dirlength
characterset = "abcdefg"
dirlength = 3
directories = map(''.join, itertools.product(characterset, repeat=dirlength))

# these are all symlinked to the same directory pointing to these files
realdir = "realdir"
files = ["yes", "no"]

if not os.path.exists(realdir):
    os.makedirs(realdir)
    # we have to make the files (and put stuff in them, otherwise it won't work)
    for f in files:
        with open(os.path.join(realdir, f), 'w') as cfile:
            cfile.write('1')

# TODO: continue!
# TODO: symlink instead of makedirs?
for d in directories:
    if not os.path.exists(d)
        os.makedirs(d)

