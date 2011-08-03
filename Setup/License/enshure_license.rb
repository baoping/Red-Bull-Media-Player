class EnsureLicense
	@license = nil
	@fileTypes = []
	@searchDirctories = []
	
	def apply( config ) 
		read_license_header config['licenceFile']
		@fileTypes = config['fileTypes']
		@searchDirctories = config['searchDirectories']
		doIt
	end

	def doIt
		@searchDirctories.each do |baseDir|
			Dir["#{baseDir}/**/*.{#{@fileTypes.join(',')}}"].each do |f|
				fileContent = read_file f
				if ! has_license_applied? fileContent
					apply_license( f, fileContent )
				else
					puts "License is already appliend to file: #{File.basename(f)}"
				end
			end	
		end

	end

	def read_file(filename) 
		data = ''
		f = File.open(filename, "r") 
		f.each_line do |line|
		    data += line
		end
		return data
	end

	def apply_license( filename, fileContent )
		puts "Applieing license to file #{File.basename(filename)}"
		newContent = "#{@license}#{fileContent}"
		f = File.new(filename, "w")
		f.write(newContent)
		f.close
	end

	def read_license_header(licenceHeaderFileName)
		puts "Read License from file: #{File.basename(licenceHeaderFileName)}"
		@license = read_file(licenceHeaderFileName)		
	end

	def has_license_applied?(fileContent)
		return fileContent.start_with? @license
	end
end

el = EnsureLicense.new 
el.apply( {
	"licenceFile" => "LICENSE_HEADER_SOURCE",
	"fileTypes" => [ "cpp", "h", "mm" ],
	"searchDirectories" => [ "../../RedbullPlayer", "../../Setup", "../../QTVLC" ]
} )
