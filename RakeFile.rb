require 'rake'
require 'fileutils'
require "rexml/document"
require "open-uri"
include FileUtils

if RUBY_PLATFORM =~ /.*linux$/
    puts "Running on linux"
    UNITTEST_FOLDER = File.expand_path('bin/Linux/UnitTests')
    TESTAPP_REGEX = "*Test"
    ENV['LD_LIBRARY_PATH'] = './'
    ENV['DISPLAY'] = '127.0.0.1:1'
elsif RUBY_PLATFORM =~ /darwin/
    puts "Running on MacOSX"
    UNITTEST_FOLDER = File.expand_path('bin/MacOSX/UnitTests')
    TESTAPP_REGEX = "*Test"
    ENV['LD_LIBRARY_PATH'] = './'
else
   gem 'rubyzip'
   require 'zip/zip'
   require 'zip/zipfilesystem'
   puts "Running on Win32"
    UNITTEST_FOLDER = File.expand_path('bin/Win32/UnitTests')
    TESTAPP_REGEX = "*Test.exe"
end

QT_INSTALL_DIR = ENV['qt_dir'] || "D:\\Qt\\2009.03"

PRECOMPILED_BINARIES_HOST="http://www.irrsinnig.org/3rdParty/"

task :default => [:download_precompiled,:unpack_precompiled_dependencies,:build_installer,:clean,:qmake,:build,:setup_test_dir,:build_package,:runtests,:parsexml]

task :download_precompiled do
	if RUBY_PLATFORM =~ /.*linux$/ 
		download_file_if_required(  
			"#{PRECOMPILED_BINARIES_HOST}/Linux/PreCompiled.tar.gz", 
			"./3rdParty/Linux/PreCompiled.tar.gz" )
	elsif  RUBY_PLATFORM =~ /darwin/		
		download_file_if_required( 
			"#{PRECOMPILED_BINARIES_HOST}/MacOSX/PreCompiled.tar.gz", 
			"./3rdParty/MacOSX/PreCompiled.tar.gz" )
	else
		download_file_if_required(  
			"#{PRECOMPILED_BINARIES_HOST}/Win32/PreCompiled.zip", 
			"./3rdParty/Win32/PreCompiled.zip" )
	end
end

def download_file_if_required( url, localFileName )
	if not File.exists?(localFileName)	
		open( localFileName, 'wb') do |file|
			file << open( url,
					:content_length_proc => lambda {|t|
					      if t && 0 < t
						 size = t
					    	 puts "Downloading file: #{url} size: #{t} ..."	 
					      end
					}
			).read
		end
	end
end


desc "Build Installer Static binary"
task :build_installer do
	if RUBY_PLATFORM =~ /.*linux$/ 
		
		setup_linux_static_qt
		build_linux_iunstaller
	elsif  RUBY_PLATFORM =~ /darwin/		
		puts "Skip on OS X"
	else
		build_win32_installer	
	end
end

def setup_linux_static_qt
	qtParentDir = "#{File.expand_path(File.dirname(__FILE__))}/3rdParty/Linux"
	qtDir = "#{qtParentDir}/qt-x11-opensource-src-4.5.3"
	Dir.glob( "#{qtDir}/lib/**/*.{la,pc,prl}" ).each do |f| 
		# puts f
		text = File.read(f)
		File.open(f, "w") do |file| 
			file.puts text.gsub( "/home/hubingch/devel/RedBull/3rdParty/Linux","#{qtParentDir}" )
		end
	end
end

def build_linux_iunstaller
	qtParentDir = "#{File.expand_path(File.dirname(__FILE__))}/3rdParty/Linux"
	qtDir = "#{qtParentDir}/qt-x11-opensource-src-4.5.3"

	ENV['QTDIR'] = qtDir + "/qt"
	puts ENV['QTDIR']
	puts ENV['QMAKESPEC']
	ENV['QT_INSTALL_BINS'] = "#{qtDir}/bin/"

	cdCmd = "cd Setup/SelfExtract &&"
	mkspec =  "-spec #{qtDir}/mkspecs/linux-g++ -r"
	qmake_lib_dir =  " QMAKE_LIBDIR_QT=#{qtDir}/lib"
	cmd =  "#{cdCmd} qmake #{qmake_lib_dir} && make clean && make " 
	sh cmd
	
end


def build_win32_installer
	qtDir = "#{File.expand_path(File.dirname(__FILE__))}/3rdParty/Win32/Qt-4.5.0-static"
	qtDir = qtDir.gsub("/","\\")
	ENV['QTDIR'] = qtDir + "\\qt"
	ENV['PATH'] = ENV['PATH'] +";"+ qtDir + "\\bin;" +  QT_INSTALL_DIR + "\\mingw\\bin;"
	ENV['QMAKESPEC'] = "win32-g++"
	puts ENV['QTDIR']
	puts ENV['PATH']
	puts ENV['QMAKESPEC']
	
	ENV['QT_INSTALL_BINS'] = "#{qtDir}\\bin\\"
	cdCmd = "cd Setup\\SelfExtract &&"
	qmake = "#{qtDir}\\bin\\qmake.exe"
	mkspec =  "-spec #{qtDir}\\mkspecs\\win32-g++ -r"
	qmake_inc_dir =  " QMAKE_INCDIR_QT=#{qtDir}\\include"
	qmake_lib_dir =  " QMAKE_LIBDIR_QT=#{qtDir}\\lib"
	qmake_moc = " QMAKE_MOC=#{qtDir}\\bin\\moc.exe"
	qmake_uic = " QMAKE_UIC=#{qtDir}\\bin\\uic.exe"
	qmake_idc = " QMAKE_IDC=#{qtDir}\\bin\\idc.exe"
	qmake_rcc = " QMAKE_RCC=#{qtDir}\\bin\\rcc.exe"
	cmd = "cmd.exe /C \"#{cdCmd} #{qmake} #{qmake_inc_dir} #{qmake_lib_dir} #{qmake_moc} #{qmake_uic} #{qmake_idc} #{qmake_rcc}  #{mkspec} \""

	sh cmd
	
	make = "#{QT_INSTALL_DIR}\\mingw\\bin\\mingw32-make.exe"

	cmd = "cmd.exe /C \"#{cdCmd} #{make} clean\""	
	sh cmd
	cmd = "cmd.exe /C \"#{cdCmd} #{make} \""	
	sh cmd
end

def unzip_precompiled_win32_binaries 
   if not File.exists?("3rdParty/Win32/unpacked")
	puts "Unpacking precompiled dependencies (this may take a few minutes)..."
 
	zip_filename = '3rdParty\\Win32\\PreCompiled.zip'
   	outdir='3rdParty\\Win32\\' 
   	Zip::ZipFile::open(zip_filename) { |zf| 
		zf.each { |e| 
        		fpath = File.join(outdir, e.name) 
        		FileUtils.mkdir_p(File.dirname(fpath)) 
        		zf.extract(e, fpath) unless e.symlink?
      		} 
   	}
	File.open('3rdParty/Win32/unpacked', 'w') do |f| 
		f.puts "1" 
	end 
   end 
end

def unzip_precompiled_unix_binaries( platform )
    if not  File.exists?("3rdParty/#{platform}/unpacked")
	puts "Unpacking precompiled dependencies (this may take a few minutes)..."
	sh "cd 3rdParty/#{platform}/ && tar xvfz PreCompiled.tar.gz && touch unpacked"
    end
end

desc "Unpack Dependencies"
task :unpack_precompiled_dependencies do
	if RUBY_PLATFORM =~ /.*linux$/
		unzip_precompiled_unix_binaries( "Linux" )
	elsif RUBY_PLATFORM =~ /darwin/
		unzip_precompiled_unix_binaries( "MacOSX" )
	else
		unzip_precompiled_win32_binaries
	end
end

desc "Setup environment variables needed by QT and Friends"
task :setup_build_environment do
	ENV['QTDIR'] = QT_INSTALL_DIR + "\\qt" 
	ENV['PATH'] = QT_INSTALL_DIR + "\\bin;" +  QT_INSTALL_DIR + "\\mingw\\bin;" + ENV['SystemRoot'] + "\\System32"
	ENV['QMAKESPEC'] = "win32-g++"
	puts ENV['QTDIR']
	puts ENV['PATH']
	puts ENV['QMAKESPEC']
end

desc "Simply delete all build artefacts"
task :clean do
	FileUtils.rm_rf("bin/", :verbose => 1)
	FileUtils.rm_rf("obj/", :verbose => 1)
end

desc "Run qmake"
task :qmake  => [:setup_build_environment] do
	cmd = "cmd.exe /C " + ENV['QTDIR'] + "\\bin\\qmake.exe -spec win32-g++ -r"
	sh cmd
end


desc "Generate the selfextracting package"
task :build_package  => [:setup_build_environment] do
	cmd = "cmd.exe /C " + QT_INSTALL_DIR + "\\mingw\\bin\\mingw32-make.exe build_package"	
	sh cmd
end

desc "Perform main build"
task :build  => [:setup_build_environment] do
	cmd = "cmd.exe /C " + QT_INSTALL_DIR + "\\mingw\\bin\\mingw32-make.exe"	
	sh cmd
end	

desc "Make Tests runable in their directory"
task :setup_test_dir  => [:setup_build_environment] do
	cmd = "cmd.exe /C " + QT_INSTALL_DIR + "\\mingw\\bin\\mingw32-make.exe setup_test_dir"	
	sh cmd
end	

desc "Run qmake for Updtr application"
task :qmake_updater  => [:setup_build_environment] do
	cmd = "cmd.exe /C \"cd Setup\\Updater && " + ENV['QTDIR'] + "\\bin\\qmake.exe -spec win32-g++ -r\""
	sh cmd
end

desc "Only build the Updtr Aplication"
task :build_updater  => [:setup_build_environment, :qmake_updater] do
	cmd = "cmd.exe /C \"cd Setup\\Updater && " + QT_INSTALL_DIR + "\\mingw\\bin\\mingw32-make.exe\""	
	sh cmd
end

desc "Only clean the Updtr Aplication"
task :clean_updater  => [:setup_build_environment, :qmake_updater] do
	cmd = "cmd.exe /C \"cd Setup\\Updater && " + QT_INSTALL_DIR + "\\mingw\\bin\\mingw32-make.exe clean\""	
	sh cmd
end

desc "Build Application"
task :old_build do 
  cmd = "agentbuild.bat "
  cmd << QT_INSTALL_DIR
  puts cmd
  sh cmd
end

task :old_build_updater do
  cmd = "agentbuild_updater.bat "
  cmd << QT_INSTALL_DIR
  puts cmd
  sh cmd
end

desc "Run all tests"
task :runtests  do
    puts "Run all tests"
    if RUBY_PLATFORM =~ /.*linux$/
        puts "Start dummy x server..."
        system( "Xvfb :1 -screen 1 1024x768x24 & ")
        ENV['DISPLAY'] = '127.0.0.1:1'
    end
    Dir.chdir(UNITTEST_FOLDER)
    Dir.glob( TESTAPP_REGEX ).each do |testbin|
        logFileName=testbin + ".testresultxml"
	if RUBY_PLATFORM =~ /.*linux$/ or RUBY_PLATFORM =~ /darwin/
	    command= "chmod +x "+ testbin + ";./" + testbin + " -xml >" + logFileName 
	else
	    command= testbin + " -xml >" + logFileName
	end
        puts logFileName
        system( command)
    end

     if RUBY_PLATFORM =~ /.*linux$/
        puts "Kill dummy x server..."
        system( "killall Xvfb" )
     end
end

desc "Parse test results"
task :parsexml => [:runtests] do
 
  Dir.chdir(UNITTEST_FOLDER)
  Dir.glob("*.testresultxml").each do |testoutput|
      xml = Array.new
      xml.push("<TestDocument>") 
      puts testoutput
      lines = File.open(  testoutput ).readlines
      lines.each do |line|
        if line =~ /^\<\?.*/
  #        puts "Remove line:  " + line
        else
          xml.push(line)    
        end
      end
      xml.push("</TestDocument>") 
      
    # puts xml
      doc = REXML::Document.new xml.join
     
      doc.elements.each('/TestDocument/TestCase') do |tc|
        testcase =  tc.attributes["name"]
        puts "##teamcity[testSuiteStarted name='" + testcase + "']"
       
       puts "TestCase: " + tc.attributes["name"]
          tc.elements.each("./TestFunction") do |fn|
            testname= fn.attributes["name"]
            fqtn =  testcase + "." + testname
            puts "##teamcity[testStarted name='" + fqtn + "'  captureStandardOutput='true']"
              fn.elements.each("./Incident") do |res|
                  fn.elements.each("./Message/Description") do |msg|
                      puts msg.text.escape_msg(msg.text)
                  end
                  if res.attributes["type"] == "fail" 
                    line = res.attributes["line"]
                    file = res.attributes["file"]
                    res.elements.each("./Description") do |error|
                      message = error.text
                      puts "##teamcity[testFailed name='" + fqtn + "' message='Test Failed!' details='file: " +file+ " line: " +line + "|n" + message.escape_msg(message) + "']"
                    end
                   end
                end
                puts "##teamcity[testFinished name='" + fqtn + "']"
              end
          puts "##teamcity[testSuiteFinished name='" + testcase + "']"    
       end
    end
 end

class String
 def escape_msg(message)
    message = message.gsub( /\|/ ,"||" )
    message = message.gsub( /\'/, "|'" )
    message = message.gsub( /\n/ ,"|n" )
    message = message.gsub( /\r/ ,"|r" )
    return message  end
end
