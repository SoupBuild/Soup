# Recipe

The Recipe file is the definition for a code package and will always be located at the root of the package directory structure. It is written in the SML (Simple Markup Language) declarative language. (Note: Adopted as a variation on TOML, still iterating on design).

## Shared Properties

### Name
The **Name** property is required for all packages. It consists of a string value for the unique package name. Note: Unique here is within a build context. When local this is set of packages within a single language dependency graph. When published this is globally unique for a language.
```
Name: "MyAwesomePackage"
```

### Language
The **Language** property is required for all packages. It consists of a string value that contains the language type and minimum build version. This language tells Soup what default [Build Tasks](Build-Task.md) to inject into the build.
```
Language: "C#|0.1"
```

### Version
The **Version** property is required for all published packages. It consists of a string value that contains the semantic version of the package.
```
Version: "1.0.0"
```

### Dependencies
The **Dependencies** property is a table of different dependency types that each consist of a list of dependency values. A dependency value can either be a string value with a [Package Reference](Package-Reference.md) or a table with a required **Reference** property that contains the Package Reference. The runtime will recursively build the dependencies and inject shared properties and allow read access for builds.
```
Dependencies: {
   Runtime: [
      "../MyOtherPackage/",
      "CoolPublicPackage@1.0.1",
      { Reference: "AnotherCoolPublicPackage@2.0.1" },
   ]
}
```

#### Build Dependencies
Build dependencies are a special set that are assumed to be C# Dynamic Libraries containing [Build Tasks](Build-Task.md). These build tasks will be loaded into the build generate and allow for shared build logic.