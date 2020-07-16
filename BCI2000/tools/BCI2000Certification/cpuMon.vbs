'-------------------------
' The inputs to this function are the sigsource, sigproc, app, logFile, and period
'-------------------------

if WScript.Arguments.Count <> 5 then
	WScript.echo "Invalid number of arguments!"
	WScript.Quit
end if

source = WScript.Arguments(0)
sigproc = WScript.Arguments(1)
app = WScript.Arguments(2)
logFile = WScript.Arguments(3)
per = WScript.Arguments(4)

'initialize the log file and other windows stuff
Set filesys = CreateObject("Scripting.FileSystemObject") 
set logH = filesys.CreateTextFile(logFile,true)

strComputer = "."
Set objWMIService = GetObject("winmgmts:\\" & strComputer & "\root\cimv2")

Set objRefresher = CreateObject("WbemScripting.SWbemRefresher")
Set colItems = objRefresher.AddEnum _
	(objWMIService, "Win32_PerfFormattedData_PerfProc_Process").objectSet

opLoad = ""
sourceLoad = ""
SPload =""
appLoad = ""
totalLoad = ""
opCnt = "25"

procNotFound = 0

logH.writeline "period: " & CStr(period)
logH.writeline "operat " & source & " " & sigproc & " " & app
wScript.sleep 2000
do until procNotFound=1
	WScript.sleep per
	objRefresher.Refresh
	procNotFound = 1
	opLoad = "-1"
	sourceLoad = "-1"
	SPload = "-1"
	appLoad ="-1"

	for each objItem in colItems
		if LCase(objItem.name) = "operat" then
			opLoad = objItem.PercentProcessorTime
			
			if opLoad <> "" then
				if objItem.ThreadCount < 5 then
					procNotFound = 0
				end if
			else
				procNotFound = 0
			end if
		elseif LCase(objItem.Name) = LCase(source) then
			sourceLoad = objItem.PercentProcessorTime
			
		elseif LCase(objItem.Name) = LCase(sigproc) then
			SPload = objItem.PercentProcessorTime
			
		elseif LCase(objItem.Name) = LCase(app) then
			appLoad = objItem.PercentProcessorTime	
		elseif LCase(objItem.Name) = LCase("_total") then
			'totalLoad = CStr(100 - CInt(objItem.PercentProcessorTime))	
			totalLoad = objItem.PercentProcessorTime		
		end if
	next

	
	if procNotFound <> 1 then
		cpuLoad = opLoad & " " & sourceLoad & " " & SPload & " " & appLoad & " " & totalLoad
		logH.writeline cpuLoad
	end if
loop
logH.close
