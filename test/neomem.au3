
; run debug version, so can catch crashes
;Run("C:\Projects\neomem\neomemC\work\code\builds\debug\NeoMem.exe")
Run("..\code\builds\debug\NeoMem.exe")

WinWaitActive("NeoMem - [Document1]")

;Send("This is some text.")
;WinClose("Untitled - Notepad")
;WinWaitActive("Notepad", "Do you want to save")
;Send("!n")

; add new folder
; this works, but it's yucky. don't want to use keyboard for cmds. 
; or at least hide it in function calls. 
;Send("^w")
;Send("Fish")
;Send("{ENTER}")

AddObject("Fish")


Func AddObject($name)
  Send("^w")
  Send($name)
  Send("{ENTER}")
EndFunc
  




