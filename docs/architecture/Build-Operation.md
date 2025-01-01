# Build Operation

The smallest unit of work that makes up the build graph produced during the generate phase. The operation contains the working folder, executable, command line arguments and declared input and output files. The input/output set are used to combine individual operations into a Directed Acyclic Graph (DAG) that is required for a well structured build.
