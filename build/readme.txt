


different compiler/project settings or makefiles go here
for now, only vs6 is being used
but could put it in a subfolder and add others if needed

eg
* linux-gcc
* mac
* win-gcc
* win-vs6
* ...


note: the neomem.opt file stores the vs6 project settings(?). 
but absurdly, it uses hardcoded paths!
so to see it correcly must put the build folder at
  c:\projects\neomem\neomemC\work
ack!!
how do people get around this??
hmm, they don't include the .opt file. 
try renaming it and see what happens?

oh, .opt stores the project folders (ie when you add folders to organize your source code).
not actual file system folders. 


