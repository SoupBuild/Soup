# Shared State Table

The output shared state from the Generate phase that will be passed along to all of the packages that directly reference the package. Used to communicate with downstream builds to tell them what input they are required to consume from the referenced package. Can be used to specify runtime dependencies that need to be copied over or build dependencies that need to be linked against.
