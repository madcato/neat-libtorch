#ifndef FEED_FORWARD_HPP_
#define FEED_FORWARD_HPP_

#include <memory>
#include <torch/torch.h>
#include <vector>

#include "neat/unit.hpp"
#include "neat/experiments/config.hpp"
#include "neat/genotype/genome.hpp"

class FeedForward: public torch::nn::Module  {
public:
    FeedForward(const Genome& genome,  std::shared_ptr<Config> config);

    torch::Tensor forward(torch::Tensor x);

    std::vector<Unit> build_units();

private:
    Genome genome;
    std::vector<Unit> units;
    torch::nn::ModuleList lin_modules;
    std::shared_ptr<Config> config;
    std::function<torch::Tensor(const torch::Tensor&)> activation;
};

#endif  // FEED_FORWARD_HPP_
