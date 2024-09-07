# Benchmarks
Current benchmark numbers on my old XPS 15 9500 with Win11

|               ns/op |                op/s |    err% |     total | benchmark
|--------------------:|--------------------:|--------:|----------:|:----------
|              667.07 |        1,499,095.26 |    1.6% |      0.08 | `ValueTableReader Deserialize Empty`
|           22,420.17 |           44,602.69 |    3.9% |      2.78 | `ValueTableReader Deserialize Complex`
|            1,370.46 |          729,681.06 |    2.7% |      0.17 | `OperationResultsReader Deserialize Empty`
|            9,236.14 |          108,270.35 |    2.8% |      1.15 | `OperationResultsReader Deserialize Complex`
|           12,989.81 |           76,983.45 |    4.6% |      1.58 | `RecipeSML Deserialize Simple`
|           63,355.38 |           15,783.98 |    2.6% |      7.67 | `RecipeSML Deserialize Complex`
|        1,217,940.63 |              821.06 |    1.7% |     44.15 | `BuildEngine Execute NoDependencies UpToDate`