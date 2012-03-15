
# rake install script for neomem
#
# to see the task list,
#   > rake --tasks
# or
#   > rake -T
# 
# [rake] [http://rake.rubyforge.org]
#
# $Date$
# $Author$
# $Revision$
#------------------------------------------------------------------

# set neomem version information for current release here
# give it a name like "1.2d prerelease" while you're still working on it
version_name = "1.2d prerelease"
version_number = "1,2,4" # last number is alpha order of the letter
version_name_nospace = version_name.tr(' ','') # translate spaces to nothing

# define programs
editor = "c:\\apps\\scite\\scite.exe"
subwcrev = "\"c:\\program files\\tortoiseSvn\\bin\\subwcrev.exe\""
rtf_editor = "\"c:\\program files\\microsoft office\\office11\\winword\" help\\neomem.rtf"
cc = "c:\\program files\\Microsoft visual studio\\common\\msdev98\\bin\\msdev.exe"
#cc = "msdev.exe"

# define folders
source_folder = 'code'
docs_folder = 'files'
release_folder = 'builds\release'
distributions_folder = 'distributions'
setup_folder = 'setup'

# define file names
#setupfile = "NeoMemSetup.exe"
#zipfile = "NeoMem.zip"
#sourcefile = "NeoMemSource.zip"
#setupfilearchive = "NeoMemSetup#{version_name_nospace}.exe"
#zipfilearchive = "NeoMem#{version_name_nospace}.zip"
#sourcefilearchive = "NeoMemSource#{version_name_nospace}.zip"
setupfile = "NeoMemSetup#{version_name_nospace}.exe"
zipfile = "NeoMem#{version_name_nospace}.zip"
sourcefile = "NeoMemSource#{version_name_nospace}.zip"



# prerequisites for a task are given in brackets...
# so can say ">rake default" to do all these, or just ">rake version" to update 
# version info, etc. 
# Actions are defined by passing a block to the task method.


# entering rake with no options will run the default task,
# which does everything listed here. 
#. any way to put these in this order for -T listing?
#. make a rake help task that puts them in order and explains a bit
task :default => [:intro, :version, :readme, :help, 
    :build, :compress, :install, :setup, :website]



# define some string methods
class String
  def exec
    puts "> " + self
    puts `#{self}`
    puts
  end
  def write_to_file(filename)
    file = File.open(filename,'w')
    file.write(self)
  end
end

#. uh, why doesn't this work? i thought it had been working... bombs on "a=gets"
def ask(s)
  puts s
  a = gets
  if a=='y' then true else false end
end


# to get current repository version, use 
# svnversion . 

# check in all changes
#cmd = "svn ci -m \"final checkin\""
#cmd.execute

# do update so we're not in a mixed revision thing (?)
#cmd = "svn update"
#cmd.execute



desc "gives an overview of this rakefile"
task :intro do
  puts %{
    this will update version info, etc, to neomem version #{version_name}.
    first you should exit visual studio, and make sure
    you've checked in all the latest changes.
    ie do >svn status, >svn commit -m "latest changes", >svn update, etc.
    }
end


desc "Update the project's version information"
#. surely there's a better way than this mess? svn props? uh, no. 
task :version do

  puts "about to update the version.rc file, so CLEAR OUT OF MSDEV (ie close project)!!"
  puts "also, good to do a svn status, svn commit, and svn update"
#  if ask("update version.rc?") then
  if true then
    
    # use subwcrev to update version info with latest revision # and date.
    cmd = "#{subwcrev} . resources\\version.rct resources\\version.rct2"
    cmd.exec
    
    # now update version number and name
    s = File.read("resources\\version.rct2")
    s.gsub!("$version_number$",version_number)
    s.gsub!("$version_name$",version_name)
  
    # also replace dates like 2006/01/21 with 2006-01-21, because we can...
    re = /(\d\d\d\d)\/(\d\d)\/(\d\d)/ # regular expression, matching a date
    s.gsub!(re) {|match| "#$1-#$2-#$3"} # replace with the new date format
    
    s.write_to_file("resources\\version.rc")
  
    # now must update the date of the main rc file. 
    # otherwise vc++ doesn't know to recompile it!
    # (even though version.rc is newer, and it's included in the main .rc file!)
    cmd = "touch resources\\neomem.rc"
    cmd.exec  
  end
end



desc "Update the readme file"
task :readme do
  if ask("update the readme.txt file?") then
    puts "hit enter when done..."
    cmd = "#{editor} files\\readme.txt"
    cmd.exec
  end
end

desc "Update the help file"
task :help do
  if ask("update the help.rtf file?") then
    puts "hit enter when done..."
    cmd = "#{rtf_editor} help\\neomem.rtf"
    cmd.exec
  end
end



# this just replaces the existing version definition, eg "1.2b" with the new one, eg "1.2c"
#. isn't there a way to pass this to nsis?

desc "Update the nsi setup script"
nsi_file = "installers\\neomem.nsi"
task :install do
  s = File.read(nsi_file)
  s.sub!(/(define VERSION)( ".*")(.*)/,"\\1 \"#{version_name}\" \\3")
#  print s[0..500]
  s.write_to_file(nsi_file)
end




desc "build the exe (broken!)"
task :build do
  # this doesn't work!! vc++ won't include the version info properly when built from cmd line!?

  puts "now build the .exe"
  puts "but can't do this from here - "
  puts "need to open msdev and build the release version."
  puts "(best to do clean and rebuild all)"
  puts "hit enter when done..."
  gets
  
  # now build the project
  #. this seems to be leaving out the version info!??
#  project = "NeoMem - Win32 Release"
  #rebuild = ''
#  rebuild = '/REBUILD' # for complete rebuild
#   log = "/OUT builds\\build.log"
#   options = log
  # "#{cc} project\\neomem.dsw /OUT builds\\build.log /MAKE "NeoMem - Release" /REBUILD
  #cmd = "#{cc} project\\neomem.dsw /OUT builds\\build.log /MAKE \"#{project}\" #{rebuild}"
#  cmd = "#{cc} project\\neomem.dsw /MAKE \"#{project}\" #{rebuild}"
#  cmd.exec
end



#.
desc "Compress the executable"
task :compress do
  if ask("compress the .exe?") then
    cmd = "upx --best --compress-icons=0 builds\\release\\NeoMem.exe"
    cmd.exec
  end
end



desc "make setup files and zips etc"
task :setup do

  "cd #{source_folder}".exec
  
  puts "copy all appropriate files to the NeoMemSetup folder..."
  "copy #{release_folder}\\NeoMem.exe #{setup_folder}\\NeoMem.exe".exec
  "copy #{release_folder}\\NeoMem.hlp #{setup_folder}\\NeoMem.hlp".exec
  "copy #{release_folder}\\NeoMem.cnt #{setup_folder}\\NeoMem.cnt".exec
  "copy #{docs_folder}\\Template.neo #{setup_folder}\\Template.neo".exec
  "copy #{docs_folder}\\Example.neo #{setup_folder}\\Example.neo".exec
  "copy #{docs_folder}\\Readme.txt #{setup_folder}\\Readme.txt".exec
  "copy #{docs_folder}\\License.txt #{setup_folder}\\License.txt".exec
  
  puts "now build #{setupfile} via NSIS..."
  #. this bombed being run here, but worked ok from cmdline. huh??
  "makensis installers\\neomem.nsi".exec
  
  # test it!
  puts "did this work? test it! ie run #{setupfile}"
  puts "hit enter when done..."
  gets
  
#  puts "clear existing zip and setup files (archives will still be there)..."
#  "del #{distributions_folder}\\#{setupfile}".exec
#  "del #{distributions_folder}\\#{zipfile}".exec
#  "del #{distributions_folder}\\#{sourcefile}".exec
  
  puts "move #{setupfile} to download folder..."
  "move script\\#{setupfile} #{distributions_folder}\\#{setupfile}".exec
  
  puts "zip up the neomemsetup folder to #{zipfile}..."
  "zip #{distributions_folder}\\#{zipfile} ..\\NeoMemSetup\*.*".exec
  
  puts "now zip up the source code to #{sourcefile}..."
#  "del #{sourcefile}".exec
  "zip #{sourcefile} rakefile *.txt *.dsp *.dsw *.rc *.rct *.hm *.cpp *.h documents\*.* hlp\*.* res\*.* script\*.*".exec
  "move #{sourcefile} #{distributions_folder}\\#{sourcefile}".exec
  
#  puts "and copy the files to archive versions..."
#  "copy #{distributions_folder}\\#{setupfile} #{distributions_folder}\\#{setupfilearchive}".exec
#  "copy #{distributions_folder}\\#{zipfile} #{distributions_folder}\\#{zipfilearchive}".exec
#  "copy #{distributions_folder}\\#{sourcefile} #{distributions_folder}\\#{sourcefilearchive}".exec  

  puts "done!"
  
end



desc "Update the website"
task :website do
  puts "ok, now upload the files to the website/download folder"
  puts "  #{setupfile}"
  puts "  #{zipfile}"
  puts "  #{sourcefile}"
  puts "and update the html files as needed."
  puts "hit enter when done..."
  gets
end



