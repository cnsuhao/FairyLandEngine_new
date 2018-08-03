Option explicit
On error resume next
dim fso,oShell,fw,drive,lenth,sCurDir,sPath
set fso=createobject("scripting.filesystemobject")
Set oShell = CreateObject("Wscript.Shell")
sPath = WScript.ScriptFullName
'MsgBox sPath
sCurDir = fso.GetParentFolderName(sPath)

Dim rtnVal
rtnVal = MsgBox ("是否删除工程编译的所有临时文件?",36,"是否清除临时文件")

If rtnVal = vbYES Then

	fso.DeleteFolder(sCurDir & "\\BuildTmp\*")
	fso.DeleteFolder(sCurDir & "\\bin\Log")
	fso.DeleteFile(sCurDir & "\\bin\*.exp")
	fso.DeleteFile(sCurDir & "\\bin\*.ilk")
	fso.DeleteFile(sCurDir & "\\bin\*.lib")
	fso.DeleteFile(sCurDir & "\\bin\*.pdb")
	fso.DeleteFile(sCurDir & "\\bin\*.log")
	fso.DeleteFile(sCurDir & "\\bin\*.exe")
	fso.DeleteFile(sCurDir & "\\*.sdf" )
	fso.DeleteFile(sCurDir & "\\lib\*.lib")
	'fso.DeleteFile(sCurDir & "\\*.bat" )
	fso.DeleteFolder(sCurDir & "\\ipch")
MsgBox "临时文件已经清除！！",vbokonly,"提示"	
End If