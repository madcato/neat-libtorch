#include "neat/phenotype/feed_forward.hpp"

#include <map>

#include "neat/activations.hpp"

FeedForward::FeedForward(const Genome& genome,  std::shared_ptr<Config> config):genome(genome), units(build_units()),
                        lin_modules(torch::nn::ModuleList()), config(config), 
                        activation(Activations().get(config->activation())) {

}

torch::Tensor FeedForward::forward(torch::Tensor x) {
    std::map<int, torch::Tensor> outputs;
    std::vector<Unit> input_units;
    std::copy_if (units.begin(), units.end(), std::back_inserter(input_units), [](Unit unit) { return unit.ref_node.type == "input"; });
    std::vector<Unit> output_units;
    std::copy_if (units.begin(), units.end(), std::back_inserter(output_units), [](Unit unit) { return unit.ref_node.type == "output"; });
    std::vector<Unit> bias_units;
    std::copy_if (units.begin(), units.end(), std::back_inserter(output_units), [](Unit unit) { return unit.ref_node.type == "bias"; });
    std::vector<Unit> stacked_units = genome.order_units(units);
}

std::vector<Unit> FeedForward::build_units() {

}


