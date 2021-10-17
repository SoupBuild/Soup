# Build Extension

A C# package that contains one or more implementations of the [Build Task](BuildTask.md) interface. When referenced as a **Build** Dependency within a [Recipe](Recipe.md) the Generate runtime will automatically discover and instantiate one instance of each public class that implements the shared interface definition. The 
