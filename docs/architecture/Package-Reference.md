# Package Reference

A **Package Reference** is a special format string that can either be a directory Path reference or a unique package Name and Version separated by an '@' character.

> Note: Path references are not allowed in published packages.

> Note2: While absolute paths are allowed, they are strongly discouraged as they do not lend themselves to sharing code with others.

```
"../../OtherPackage/"
```

```
"OtherPackage@1.2.3"
```