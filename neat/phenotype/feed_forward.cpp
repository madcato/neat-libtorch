#include "neat/phenotype/feed_forward.hpp"

#include <map>

#include "neat/activations.hpp"

FeedForwardImpl::FeedForwardImpl(const Genome& genome,  std::shared_ptr<Config> config):genome(genome), units(build_units()),
                        lin_modules(torch::nn::ModuleList()), config(config), 
                        activation(Activations().get(config->activation())) {

}

torch::Tensor FeedForwardImpl::forward(torch::Tensor x) {
    std::map<int, torch::Tensor> outputs;
    std::vector<Unit> input_units;
    std::copy_if (units.begin(), units.end(), std::back_inserter(input_units), [](Unit unit) { return unit->ref_node->type == "input"; });
    std::vector<Unit> output_units;
    std::copy_if (units.begin(), units.end(), std::back_inserter(output_units), [](Unit unit) { return unit->ref_node->type == "output"; });
    std::vector<Unit> bias_units;
    std::copy_if (units.begin(), units.end(), std::back_inserter(output_units), [](Unit unit) { return unit->ref_node->type == "bias"; });
    std::vector<Unit> stacked_units = genome->order_units(units);

    for(auto u = input_units.begin(); u != input_units.end() ; u++) {
        outputs[(*u)->ref_node->id] = x[0][(*u)->ref_node->id];
    }

    while (stacked_units.size() > 0) {
        Unit current_unit = stacked_units.back();
        stacked_units.pop_back();

        if (current_unit->ref_node->type != "input" && current_unit->ref_node->type != "bias") {
            // inputs_ids = genome.get_inputs_ids(current_unit.ref_node.id);
        }
    }
}

std::vector<Unit> FeedForwardImpl::build_units() {

}


