# Local User Config

The Local User config allows for a set of properties to be set for an individual user. Currently the only state that is used from this file is the list of SDKs. The [SWhere](../Tools/SWhere.md) can be used to automatically fill in this file with the VSWhere tooling.

## "SDK"
A special table that allows for registering external SDKs. These allow for passing in Properties and a list of directories that should be allowed read access.

### "Name"
The unique name of the SDK.

### "SourceDirectories"
The list of directory strings that will be granted read access to allow the build to integrate with resources outside the normal input/output directories.

### "Properties"
The list of properties that will be passed into the build so the Generate phase can pick up information about the external SDKs.