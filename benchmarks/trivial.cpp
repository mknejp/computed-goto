#include <benchmark/benchmark.h>

#include <algorithm>
#include <cstdint>
#include <random>
#include <vector>

namespace
{
  enum bytecode : int8_t
  {
    add1,
    add2,
    add3,
    add5,
    add7,

    sub1,
    sub2,
    sub3,
    sub5,
    sub7,

    mul2,
    mul3,
    mul5,

    div2,
    div3,
    div5,

    halt,
  };

  auto run_with_switch_loop(bytecode const* instructions)
  {
    for(auto value = 0.0;;)
    {
      switch(*instructions++)
      {
        case bytecode::add1:
          value += 1.0;
          break;
        case bytecode::add2:
          value += 2.0;
          break;
        case bytecode::add3:
          value += 3.0;
          break;
        case bytecode::add5:
          value += 5.0;
          break;
        case bytecode::add7:
          value += 7.0;
          break;
        case bytecode::sub1:
          value -= 1.0;
          break;
        case bytecode::sub2:
          value -= 2.0;
          break;
        case bytecode::sub3:
          value -= 3.0;
          break;
        case bytecode::sub5:
          value -= 5.0;
          break;
        case bytecode::sub7:
          value -= 7.0;
          break;
        case bytecode::mul2:
          value *= 2.0;
          break;
        case bytecode::mul3:
          value *= 3.0;
          break;
        case bytecode::mul5:
          value *= 5.0;
          break;
        case bytecode::div2:
          value /= 2.0;
          break;
        case bytecode::div3:
          value /= 3.0;
          break;
        case bytecode::div5:
          value /= 5.0;
          break;
        case bytecode::halt:
          return value;
      }
    }
  }

  auto run_with_computed_goto(bytecode const* instructions)
  {
    auto value = 0.0;

    constexpr void* labels[] = {
      [bytecode::add1] = &&add1_label,
      [bytecode::add2] = &&add2_label,
      [bytecode::add3] = &&add3_label,
      [bytecode::add5] = &&add5_label,
      [bytecode::add7] = &&add7_label,
      [bytecode::sub1] = &&sub1_label,
      [bytecode::sub2] = &&sub2_label,
      [bytecode::sub3] = &&sub3_label,
      [bytecode::sub5] = &&sub5_label,
      [bytecode::sub7] = &&sub7_label,
      [bytecode::mul2] = &&mul2_label,
      [bytecode::mul3] = &&mul3_label,
      [bytecode::mul5] = &&mul5_label,
      [bytecode::div2] = &&div2_label,
      [bytecode::div3] = &&div3_label,
      [bytecode::div5] = &&div5_label,
      [bytecode::halt] = &&halt_label,
    };

    auto const next = [&] { return labels[*instructions++]; };
    goto* next();

  add1_label:
    value += 1.0;
    goto* next();
  add2_label:
    value += 2.0;
    goto* next();
  add3_label:
    value += 3.0;
    goto* next();
  add5_label:
    value += 5.0;
    goto* next();
  add7_label:
    value += 7.0;
    goto* next();
  sub1_label:
    value -= 1.0;
    goto* next();
  sub2_label:
    value -= 2.0;
    goto* next();
  sub3_label:
    value -= 3.0;
    goto* next();
  sub5_label:
    value -= 5.0;
    goto* next();
  sub7_label:
    value -= 7.0;
    goto* next();
  mul2_label:
    value *= 2.0;
    goto* next();
  mul3_label:
    value *= 3.0;
    goto* next();
  mul5_label:
    value *= 5.0;
    goto* next();
  div2_label:
    value /= 2.0;
    goto* next();
  div3_label:
    value /= 3.0;
    goto* next();
  div5_label:
    value /= 5.0;
    goto* next();
  halt_label:
    return value;
  }

  auto run_with_tail_recursion(bytecode const* instructions) {
    static double (* const funs[17])(double, bytecode const*) = {
      [bytecode::add1] = [](auto value, auto* inst) { return funs[*inst](value + 1.0, inst + 1); },
      [bytecode::add2] = [](auto value, auto* inst) { return funs[*inst](value + 2.0, inst + 1); },
      [bytecode::add3] = [](auto value, auto* inst) { return funs[*inst](value + 3.0, inst + 1); },
      [bytecode::add5] = [](auto value, auto* inst) { return funs[*inst](value + 5.0, inst + 1); },
      [bytecode::add7] = [](auto value, auto* inst) { return funs[*inst](value + 7.0, inst + 1); },
      [bytecode::sub1] = [](auto value, auto* inst) { return funs[*inst](value - 1.0, inst + 1); },
      [bytecode::sub2] = [](auto value, auto* inst) { return funs[*inst](value - 2.0, inst + 1); },
      [bytecode::sub3] = [](auto value, auto* inst) { return funs[*inst](value - 3.0, inst + 1); },
      [bytecode::sub5] = [](auto value, auto* inst) { return funs[*inst](value - 5.0, inst + 1); },
      [bytecode::sub7] = [](auto value, auto* inst) { return funs[*inst](value - 7.0, inst + 1); },
      [bytecode::mul2] = [](auto value, auto* inst) { return funs[*inst](value * 2.0, inst + 1); },
      [bytecode::mul3] = [](auto value, auto* inst) { return funs[*inst](value * 3.0, inst + 1); },
      [bytecode::mul5] = [](auto value, auto* inst) { return funs[*inst](value * 5.0, inst + 1); },
      [bytecode::div2] = [](auto value, auto* inst) { return funs[*inst](value / 2.0, inst + 1); },
      [bytecode::div3] = [](auto value, auto* inst) { return funs[*inst](value / 3.0, inst + 1); },
      [bytecode::div5] = [](auto value, auto* inst) { return funs[*inst](value / 5.0, inst + 1); },
      [bytecode::halt] = [](auto value, auto*) { return value; }
    };

    return funs[*instructions](0.0, instructions + 1);
  }

  auto create_instructions(std::size_t count, std::uint_fast64_t seed = std::mt19937_64::default_seed)
  {
    auto instructions = std::vector<bytecode>();
    instructions.reserve(count);

    auto urng = std::mt19937_64(seed);
    auto dist = std::uniform_int_distribution<int>(0, static_cast<int>(bytecode::halt) - 1);
    std::generate_n(std::back_inserter(instructions), count - 1, [&] { return static_cast<bytecode>(dist(urng)); });
    instructions.push_back(bytecode::halt);
    return instructions;
  }
}

static auto BM_switch_loop(benchmark::State& state, int)
{
  auto const instructions = create_instructions(static_cast<std::size_t>(state.range(0)));

  for(auto _: state)
  {
    (void)_;
    benchmark::DoNotOptimize(run_with_switch_loop(instructions.data()));
  }
}

static auto BM_computed_goto(benchmark::State& state, int)
{
  auto const instructions = create_instructions(static_cast<std::size_t>(state.range(0)));

  for(auto _: state)
  {
    (void)_;
    benchmark::DoNotOptimize(run_with_computed_goto(instructions.data()));
  }
}

static auto BM_tail_recursion(benchmark::State& state, int)
{
  auto const instructions = create_instructions(static_cast<std::size_t>(state.range(0)));

  for(auto _: state)
  {
    (void)_;
    benchmark::DoNotOptimize(run_with_tail_recursion(instructions.data()));
  }
}

BENCHMARK_CAPTURE(BM_switch_loop, switch_loop, 0)->Range(1000, 100'000'000);
BENCHMARK_CAPTURE(BM_computed_goto, computed_goto, 0)->Range(1000, 100'000'000);
BENCHMARK_CAPTURE(BM_tail_recursion, tail_recursion, 0)->Range(1000, 100'000'000);

BENCHMARK_MAIN();
