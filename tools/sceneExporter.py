# Export bounding boxes
for bbox in cmds.ls(sl=True):
	print "bbox ",
	for item in cmds.exactWorldBoundingBox(bbox):
		print str(item),
	print ""
