#include <benchmark/benchmark.h>
#include <cmath>


static void BM_Log_In(benchmark::State& state) {
  for (auto _ : state) {
      double x = 193773337.7;
      double y = 1.3;
      for (int i=0; i < 512; ++i) {
          x = log(x / y);
      }
  }
}

BENCHMARK(BM_Log_In);


static void BM_Log_Out(benchmark::State& state) {
  for (auto _ : state) {
      double x = 193773337.7;
      double y = 1.3;
      for (int i=0; i < 512; ++i) {
        x = log(x) - log(y);
      }
  }
}

BENCHMARK(BM_Log_Out);


BENCHMARK_MAIN();

