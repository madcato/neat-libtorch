#ifndef CROSSOVER_HPP
#define CROSSOVER_HPP

#include <utility>

#include "neat/experiments/config.hpp"
#include "neat/genotype/genome.hpp"

Genome crossover(const Genome& genome_1, const Genome& genome_2, const std::shared_ptr<Config>& config, const torch::Device& device);

std::pair<Genome, Genome> order_parents(const Genome& parent_1, const Genome& parent_2);

#endif  // CROSSOVER_HPP