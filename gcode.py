#!/usr/bin/python

import os, sys

f = open(sys.argv[1],'r')

lines = f.readlines()

allCommands = dict()

for line in lines:
  line.strip()
  commands = line.split()
  
  if len(commands) < 1: continue
  
  command = commands[0]
  
  if len(command)>1: 
    if not allCommands.has_key(command):
      allCommands[command] = line


for command in allCommands.items():
  print command
