# Language Version Resolution

The Build Extension version resolution has multiple levels depending if there is a package lock or not.

* Is a package lock present
    * Yes - Use the explicit version and fail if unable to find
    * No - Fail if any non-local references



