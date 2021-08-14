#include "neat/mutation.hpp"

#include "neat/utils.hpp"

void mutate(const Genome& genome, std::shared_ptr<Config> config) {
    if (rand_uni_val() < config->connection_mutation_rate()) {

    }
}