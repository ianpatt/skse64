#! python2
import os, sys, string, datetime, shutil, subprocess

def mkdir_safe(a):
	try:
		os.mkdir(a)
	except OSError:
		pass

def quote(a):
	if a.find(" ") >= 0:
		return "\"" + a + "\""
	else:
		return a

# build paths
vanillaRoot = "vanilla"
modifiedRoot = "modified"
mergedRoot = "merged"
buildRoot = "build_src"
buildBinRoot = "build_bin"
compilerPath = "compiler\\PapyrusCompiler.exe"
compilerFlagPath = os.path.join(vanillaRoot, "TESV_Papyrus_Flags.flg")
buildTime = datetime.datetime.utcnow().isoformat(" ")

if not os.access(vanillaRoot, os.F_OK):
	print "vanilla source files not found"
	sys.exit(0)

# clean
print "cleaning old files"

shutil.rmtree(mergedRoot, True)
shutil.rmtree(buildRoot, True)
shutil.rmtree(buildBinRoot, True)
mkdir_safe(mergedRoot)
mkdir_safe(buildRoot)
mkdir_safe(buildBinRoot)

mergedFiles = []

# scan modified folder, append contents to vanilla file
print "merging files"

for name in os.listdir(modifiedRoot):
	# build paths
	modifiedPath = os.path.join(modifiedRoot, name)
	vanillaPath = os.path.join(vanillaRoot, name)
	buildPath = os.path.join(buildRoot, name)
	mergedPath = os.path.join(mergedRoot, name)
	
	# open files
	modifiedSrc = open(modifiedPath, "r")
	try:
		vanillaSrc = open(vanillaPath, "r")
	except IOError:
		vanillaSrc = None
	dst = open(buildPath, "w")
	
	# copy data
	if vanillaSrc:
		dst.writelines(vanillaSrc.readlines())
		dst.write("\n")
		dst.write("\n")
		dst.write("; SKSE64 additions built " + buildTime + " UTC\n")
	dst.writelines(modifiedSrc.readlines())
	dst.close()
	
	mergedFiles.append(name.lower())
	
	shutil.copy(buildPath, mergedPath)

# copy nonmerged files
print "copying vanilla files"

for name in os.listdir(vanillaRoot):
	if name.lower() in mergedFiles:
		continue
	
	vanillaPath = os.path.join(vanillaRoot, name)
	buildPath = os.path.join(buildRoot, name)
	
	src = open(vanillaPath, "r")
	dst = open(buildPath, "w")
	
	dst.writelines(src.readlines())
	dst.close()
	src.close()

# run compiler on merged files
print "compiling"

erroringClasses = []

for name in mergedFiles:
	className = name[:name.find(".")]
	argList = [
		compilerPath,
		className,
		"-i=" + quote(buildRoot),
		"-o=" + quote(buildBinRoot),
		"-f=" + quote(compilerFlagPath),
#		"-d",
#		"-keepasm",
		"-op"
	]
	
	if subprocess.call(argList) != 0:
		erroringClasses.append(className)

if len(erroringClasses) > 0:
	print "### error building classes: ", erroringClasses
