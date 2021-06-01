#file converter - OBJ to modified OBJ
import sys
import os

def getAverage(lst):
	count = 0.0
	current = 0.0
	
	for i in lst:
		current += i
		count += 1
	return current/count

def getAverageColor(fileName):
	os.system("c:\\users\\will\\videos\\ffmpeg -loglevel panic -nostats -hide_banner -y -i {} temp.bmp".format(fileName))
	file = open("temp.bmp", "rb")
	contents = file.read()[54:]
	file.close()
	
	redList = []
	greenList = []
	blueList = []

	#get all the reds
	i = 2
	while i < len(contents):
		redList.append(int(contents[i]))
		i += 3
	i = 1
	while i < len(contents):
		greenList.append(int(contents[i]))
		i += 3
	i = 0
	while i < len(contents):
		blueList.append(int(contents[i]))
		i += 3

	return "{} {} {}\n".format(getAverage(redList)/255, getAverage(greenList)/255, getAverage(blueList)/255)
		


def getTexture(mtlName, mtlLib):
	outstring = ""
	file = open(mtlLib, "r+")
	text = file.read()
	file.close()
	lines = text.split("\n")
	correct = 0

	for i in range(len(lines)):
		if "newmtl {}abc".format(mtlName) in (lines[i]+"abc"): correct = i + 1

	i = correct
	
	while True: 
		if "Kd" in lines[i] and "map_Kd" not in lines[i]:
			#print("\n"*100)
			thing = lines[i].split(" ")
			outstring = "{} {} {}".format(thing[1], thing[2], thing[3])
		if "map_Kd" in lines[i]:
			thing = lines[i].split(" ")[1].replace("/","\\")
			outstring = getAverageColor(thing)
			break
		if "newmtl" in lines[i]: break
		i += 1
		if i >= len(lines): break

	return outstring


def convertFile(fileName):
	file = open(fileName, "r+")
	text = file.read()
	file.close()
	outstring = ""
	x = text.split("\n")

	for i in x:
		if i == "": outstring += "\n"
		i = i.split(" ")
		if i[0] == "mtllib":
			library = i[1]
		if i[0] == "usemtl": 
			materialName = i[1]
			#outstring += "usemtl " + getAverageColor(sys.argv[1][0:-4] + "\\" + materialName + ".png") + "\n"
			outstring += "usemtl " + getTexture(materialName, library) + "\n"
		if i[0] == "vt" or i[0] == "vn": pass
		if i[0] == "v": outstring += i[0] + " " + i[1] + " " + i[2] + " " + i[3] + "\n"
		
		if i[0] == "f":
			outstring += "f "
			i.remove("f")
			nums = 0
			for o in i:
				t = o.split("/")
				if nums != 3: outstring += t[0] + " "
				nums += 1
			outstring += "\n"
	return outstring
	


newText = convertFile(sys.argv[1])
file = open(sys.argv[1][0:-4] + "_.obj", "w")
file.write(newText)
file.close()