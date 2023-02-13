# Parameters Table

The Parameters table contains the generated parameters from the application along with the overridden properties passed into the CLI arguments.

## Known Keys

### "PackageDirectory"
The absolute path to the active package that is getting build.

### "TargetDirectory"
The absolute path to the output target folder. This is the only location that is allowed to be written to during build evaluation.

### "Dependencies"
The table of direct dependency references containing the same information that is provided by the Application for the current package.
"Reference" and "TargetDirectory".

### "SDKs"
The SDKs table that comes from the global user config.

### "Architecture"
The known parameter for the architecture (x64, ARM, etc)

### "Compiler"
The known parameter for the compiler.

### "Flavor"
The known parameter for the flavor (Release, Debug, etc). 

### "System"
The known parameter for the system (Windows, Linux, etc).
