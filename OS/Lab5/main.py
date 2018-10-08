import fileinput
import sys
import operator

#a class that reflects all the stats of a process that I need
class process:
	pid = 0
	burst = 0
	arrival = 0
	waitTime = 0
	finished = False
	progress = 0
	startTime = 0
	finishTime = 0

#parse the command line arguments
if len(sys.argv) < 3 or (sys.argv[2] == "RR" and len(sys.argv) < 4):
	print("Invalid input, need a file name, scheduling method, and a quantum if the method is RR")
	exit
if sys.argv[2] != "RR" and sys.argv[2] != "FCFS" and sys.argv[2] != "SJF":
	print("Invalid scheduling method")
	exit
filename = str(sys.argv[1])
numProcessors = 0
procAddList = []
for line in fileinput.input(filename):
	if numProcessors == 0:
		numProcessors = int(line[0])
	else:
		nums = line.split()
		tempProc = process()
		tempProc.pid = int(nums[0])
		tempProc.arrival = int(nums[1])
		tempProc.burst = int(nums[2])
		procAddList.append(tempProc)
method = sys.argv[2]
procAddList = sorted(procAddList, key=operator.attrgetter('arrival'))
# if method == "RR" or method == "FCFS":
#     procList = sorted(procList, key=operator.attrgetter('arrival'))
# elif method == "SJF":
#     procList = sorted(procList, key=operator.attrgetter('burst'))

currProc = 0
nextProc = 0
timeElapsed = 0
lastFin = 0
prevFin = 0
#if the method is not RR then we use this header
if method != "RR":
	print("PID", "Arrival", "Start", "End", "Running", "Waiting")
#otherwise we use this shorter one
else:
	quantum = int(sys.argv[3])
	print("PID", "Start", "End", "Running")
procList = []
initProc = process()
while True:
	#if the list of current running processes isnt empty, grab the current process
	if procList:
		initProc = procList[currProc]
	for proc in procAddList:				#for every process in the list of processes that are to be added
		if proc.arrival == timeElapsed:		#if it is the correct time for this process to arrive
			procList.append(proc)			#add it to the list of active processes
			if method == "SJF":				#if the method is SJF, sort the list by burst time
				procList = sorted(procList, key=operator.attrgetter('burst'))
				if initProc in procList:	#and set the current process index to the location of the process we grabbed
					currProc = procList.index(initProc)
	#increment time
	timeElapsed += 1
	#if the list is empty, jump back to the start of the loop
	if not procList:
		continue
	#increment the progress of the current process
	procList[currProc].progress += 1

	#if the process finishes, grab the time it finished at
	if procList[currProc].progress == procList[currProc].burst:
		procList[currProc].finished = True
		procList[currProc].finishTime = timeElapsed
		lastFin = timeElapsed

	#for every other process in the active process list, increment the wait time
	for process in procList:
		if not process.finished and process is not procList[currProc]:
			process.waitTime += 1

	#check to see if the conditions are met to exit the while loop
	exitBool = True
	if len(procList) < len(procAddList):
		exitBool = False
	for process in procList:
		if not process.finished:
			exitBool = False

	#if the method is RR then check to see if quantum time has passed or the current process has finished
	if method == "RR" and (timeElapsed - lastFin)%quantum == 0:
		#print the next line of the report
		p = procList[currProc]
		p.finishTime = timeElapsed
		print('{:3d}'.format(p.pid), '{:5d}'.format(prevFin), '{:3d}'.format(p.finishTime), '{:7d}'.format(timeElapsed - prevFin))
		startProc = currProc
		#loop to the next processor that is not finished
		while True:
			currProc += 1
			if currProc == len(procList):
				currProc = 0
			if not procList[currProc].finished:
				break
			if currProc == startProc:
				currProc = 0
				break
		prevFin = timeElapsed
	#if the method is not RR and the process is finished
	elif procList[currProc].finished:
		#print the next line of the report
		p = procList[currProc]
		print('{:3d}'.format(p.pid), '{:7d}'.format(p.arrival), '{:5d}'.format(p.startTime), '{:3d}'.format(p.finishTime), '{:7d}'.format(p.burst), '{:7d}'.format(p.waitTime))
		currProc += 1
		if currProc < len(procList):
			procList[currProc].startTime = timeElapsed
		else:
			currProc = 0
		#if the method is SJF, find the lowest index of the unfinished active processes
		if method == "SJF":
			currProc = 0
			while currProc < len(procList):
				if procList[currProc].finished:
					currProc += 1
				else:
					procList[currProc].startTime = timeElapsed
					break

	if exitBool:
		break
#print out the final report for RR
if method == "RR":
	print("PID", "Arrival", "Running", "End", "Waiting")
	for p in procList:
		print('{:3d}'.format(p.pid), '{:7d}'.format(p.arrival), '{:7d}'.format(p.burst), '{:3d}'.format(p.finishTime), '{:7d}'.format(p.waitTime))
#print the average wait time and time elapsed
print("Average wait time:", sum(p.waitTime for p in procList)/len(procList), "units")
print("Time elapsed:", timeElapsed, "units")