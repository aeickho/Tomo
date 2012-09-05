#!/usr/bin/python

import os, sys

f = open(sys.argv[1],'r')

## State holders
imageName = ''
resX = 512
resY = 512

convertStr = ''

for line in f:
  elements = line.strip().split(' ')
  if len(elements) == 0 or len(line.strip()) == 0: continue

  if elements[0] == 'IMAGE' and len(elements)==4 or elements[0]=='END':
    if len(imageName) > 0 and len(convertStr) > 0:
      convertStr += ' ' + imageName 
      print convertStr
      os.system(convertStr)
      if elements[0] == 'END': exit()

    imageName = elements[1]
    resX = int(elements[2])
    resY = int(elements[3])
    convertStr = "convert -size %dx%d canvas:black -fill none " % (resX,resY) 

  else:
    color = elements[0]
    convertStr += ' -stroke ' + color

    if len(elements) == 1: continue
    convertStr += " -draw 'polyline "
    for e in elements[1:]:
      convertStr += e + ' '
    convertStr += "'"

