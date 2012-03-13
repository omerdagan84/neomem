
NeoMem Source Code
---------------------

Here is the source code, written in Visual C++ 6.0/MFC.

The backend for the program is called Brooklyn - the BData and BObject
classes contain that. The UI did not achieve good separation from
the backend though - it accesses the internals of those classes
for speed. 

Also there is a lack of unit testing - I didn't use it when I 
was writing it, mainly due to a desire to get the thing done as
quickly as possible, but now I think it's a great idea.  

Some things to do are marked with //. comments, or //, for lower 
priority items. or //! for high priority. 

I'm using Rake to build the project. Except for the actual compilation, 
because calling msdev from the cmdline seems to leave out the version info. 
So install Rake [1], and say rake -T to see the list of tasks it can do. 

eg

>rake -T
(in C:/Projects/neomem/neomemC/work)
rake build     # build the exe (broken!)
rake compress  # Compress the executable
rake help      # Update the help file
rake install   # Update the nsi setup script
rake intro     # gives an overview of this rakefile
rake readme    # Update the readme file
rake setup     # make setup files and zips etc
rake version   # Update the project's version information
rake website   # Update the website



[1] http://rake.rubyforge.org/

