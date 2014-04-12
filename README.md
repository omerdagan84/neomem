
NeoMem 1.2d
An information manager (word processor+database)
C++/MFC 50k sloc
------------------------------------------------------------------------

The backend for the program is called Brooklyn - the BDocument, BData 
and BObject classes contain that. 

Some things to do are marked with //. comments, or //, for lower 
priority items. or //! for high priority. 

It uses Rake to build the project, except for the actual compilation, 
as calling msdev from the cmdline seems to leave out the version info. 
So install Rake [1], and say rake -T to see the list of tasks it can do, 
eg

> rake -T

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


