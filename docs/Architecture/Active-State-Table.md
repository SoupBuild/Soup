# Active State Table

The root table that provides input and output state from individual build tasks during the Generate phase. Used for inter-task communication within the same package generate phase.

## Known Keys

### "Parameters"
The [Parameters Table](Parameters-Table.md) that contains the input from the Command Line Application.

### "Recipe"
The duplicated state of the [Recipe](Recipe.md) file.

### "Dependencies"
The table which a key for each direct dependency referenced from the Recipe file. The value contains the [Shared State Table](Shared-State-Table.md) from the resulting outputs from the generate phase of the referenced package build.

