#ifndef MUTATION_HPP_
#define MUTATION_HPP_

#include <memory>

#include "neat/experiments/config.hpp"
#include "neat/genotype/genome.hpp"

void mutate(Genome& genome,  std::shared_ptr<Config> config);

#endif  // MUTATION_HPP_