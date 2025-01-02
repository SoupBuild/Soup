# Steps To Build A Single Operation
These steps were designed to perform the least amount of work during an incremental build that performs no evaluate operations.

1. Setup
    1. Load the previous Evaluate Operation Graph if present
    1. Load the previous Evaluate Operation Results if present
1. Incremental Generate
    1. Create an in memory Operation Graph with a single node to perform the Generate Phase.
    1. Load previous Generate Operation Results if they exist.
    1. Check if the single generate Operation is out of date and requires running
    1. Execute the Operation if required
    1. If executed, save the Operation Results for the Generate Graph for future incremental builds
1. Setup
    1. If generate executed, load new Evaluate Operation Graph and map any Operation Results that are still relevant
    1. TODO: Generate cleanup tasks to delete any output that is no longer in the graph
1. Incremental Evaluate
    1. Iterate over every Operation, ensuring that we only attempt to evaluate an operation when all parent operations have finished.
    1. Check if the operation is out of date and requires running.
    1. Execute the Operation if required.
    1. If any operations executed, save the Operation Results for future incremental builds
