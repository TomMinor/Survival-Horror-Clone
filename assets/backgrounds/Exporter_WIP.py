import maya.cmds as cmds

print "//Bbox"
for bbox in cmds.sets( 'bbox', q=True ):
	print "bbox",
	for token in cmds.exactWorldBoundingBox(bbox):
		print token,
	print ""
	
print "//Trigger"
for i, trigger in enumerate(cmds.sets( 'trigger', q=True ), 1):
	print "trigger",
	for token in cmds.exactWorldBoundingBox(bbox):
		print token,
	print i
	
print "//Camera"
for i, camera in enumerate(cmds.sets( 'camera', q=True ), 1):
	print "camera",
	for token in cmds.xform(camera, q=True, ro=True):
		print token,
	for token in cmds.xform(camera, q=True, t=True):
		print token,
	print 75,
	print i	
	
print "spawn",
for token in cmds.xform('spawn', q=True, t=True):
	print token,
	
print "//Exit"
for exit in cmds.sets( 'exits', q=True ):
	print "exit",
	for token in cmds.xform(camera, q=True, t=True):
		print token,
	print 'nextRoom'

# Render backgrounds
