
# this will see if a given window name is open
# it actually works!
# but only for hardcoded window names, ie no wildcards.
# so using perl for that

name = "test win.rb - SciTE"


require 'Win32API'

FindWindow = Win32API.new("user32","FindWindow",["p","p"], "L")
hwnd = FindWindow.call(nil, name)
if hwnd != 0 then
  puts "window found!"
else
  puts "window not found!"
end


 