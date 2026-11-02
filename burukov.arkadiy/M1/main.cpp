#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <vector>

#include "../common/shapes.hpp"

int main(int argc, char ** argv)
{
  constexpr int MIN_ARGS = 3;
  constexpr int MAX_ARGS = 4;
  constexpr int THREADS_IDX = 1;
  constexpr int TRIES_IDX = 2;
  constexpr int SEED_IDX = 3;
  constexpr int BAD_EXIT = 1;
  constexpr int GOOD_EXIT = 0;

  if (argc < MIN_ARGS || argc > MAX_ARGS)
  {
    std::cerr << "Usage: ./lab threads tries [seed]" << '\n';
    return BAD_EXIT;
  }

  long long threads = 0;
  long long tries = 0;
  long long seed = 0;

  try
  {
    threads = burukov::parseArgument(argv[THREADS_IDX], "threads");
    tries = burukov::parseArgument(argv[TRIES_IDX], "tries");
    if (argc == MAX_ARGS)
    {
      seed = burukov::parseArgument(argv[SEED_IDX], "seed");
    }
  }
  catch (const std::invalid_argument & e)
  {
    std::cerr << e.what() << '\n';
    return BAD_EXIT;
  }
  catch (const std::out_of_range & e)
  {
    std::cerr << "Number is out of range: " << e.what() << '\n';
    return BAD_EXIT;
  }

  if (threads < 0 || tries <= 0 || seed < 0)
  {
    std::cerr << "threads and seed must be non-negative, tries must be positive" << '\n';
    return BAD_EXIT;
  }

  threads = (threads > 0) ? threads : 1;

  std::vector< std::unique_ptr< burukov::Shape > > shapes;

  double r = 0.0;
  double second = 0.0;
  double x = 0.0;
  double y = 0.0;

  while (std::cin >> r)
  {
    if (!(std::cin >> second >> x >> y))
    {
      std::cerr << "Error: cannot parse figure parameters" << '\n';
      return BAD_EXIT;
    }

    if (r <= 0.0)
    {
      std::cerr << "Radius must be positive" << '\n';
      return BAD_EXIT;
    }

    if (second == 0.0)
    {
      shapes.push_back(std::make_unique< burukov::Circle >(r, x, y));
    }
    else
    {
      const double b = (second < 0.0) ? -second : second;
      shapes.push_back(std::make_unique< burukov::Ellipse >(r, b, x, y));
    }
  }

  if (shapes.empty())
  {
    std::cout << "0 0" << '\n';
    return GOOD_EXIT;
  }

  const burukov::rectangle_t rect = burukov::findRectangleRange(shapes);

  try
  {
    const auto result = burukov::area(threads, tries, seed, shapes, rect);
    const double area_union = result.first;
    const double area_intersect = result.second;

    std::cout << std::setprecision(std::numeric_limits< double >::max_digits10);
    std::cout << area_union << " " << area_intersect << '\n';
  }
  catch (const std::invalid_argument & e)
  {
    std::cerr << e.what() << '\n';
    return BAD_EXIT;
  }
  return GOOD_EXIT;
}
