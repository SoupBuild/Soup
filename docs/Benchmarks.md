# Benchmarks
Current benchmark numbers on my old XPS 15 9500 with Win11

|               ns/op |                op/s |    err% |     total | benchmark
|--------------------:|--------------------:|--------:|----------:|:----------
|            4,954.59 |          201,833.06 |    1.0% |      0.59 | `PackageReference Parse Name Only`
|            9,523.36 |          105,004.95 |    1.8% |      1.13 | `PackageReference Parse Language, User, Name and Version`
|              166.71 |        5,998,483.17 |    2.0% |      0.02 | `ValueTableReader Deserialize Empty`
|            5,329.59 |          187,631.65 |    1.5% |      0.64 | `ValueTableReader Deserialize Complex`
|              354.82 |        2,818,341.50 |    1.2% |      0.04 | `OperationResultsReader Deserialize Empty`
|            1,585.87 |          630,569.30 |    0.7% |      0.19 | `OperationResultsReader Deserialize Complex`
|            3,224.06 |          310,168.03 |    1.1% |      0.40 | `RecipeSML Deserialize Simple`
|           14,332.25 |           69,772.72 |    0.5% |      1.74 | `RecipeSML Deserialize Complex`
|          349,772.30 |            2,859.00 |    1.1% |     12.51 | `BuildEngine Execute NoDependencies UpToDate`