#include "neat/phenotype/feed_forward.hpp"

#include <map>
#include <torch/torch.h>

#include "neat/activations.hpp"

FeedForwardImpl::FeedForwardImpl(const Genome& genome,  std::shared_ptr<Config> config, const torch::Device& device):genome(genome), units(build_units()),
                        lin_modules(torch::nn::ModuleList()), config(config), 
                        activation(Activations().get(config->activation())), device(device) {

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
            std::vector<int> inputs_ids = genome->get_inputs_ids(current_unit->ref_node->id);
            torch::Tensor in_vec = torch::autograd::Variable(torch::zeros({1, (long)inputs_ids.size()}, torch::TensorOptions().device(device).requires_grad(true)));
            
            int i = 0;
            for(auto it = inputs_ids.begin() ; it != inputs_ids.end() ; it++) {
                int input_id = *it;
                in_vec[0][i] = outputs[input_id];

                auto itr = std::find(units.begin(), units.end(), current_unit);
                assert(itr != end(units));
                size_t pos = itr - units.begin();
                std::shared_ptr<torch::nn::Module> linear_module = lin_modules[pos];
                // if linear_module is not None:  # TODO: Can this be avoided?
                torch::Tensor scaled = {}; // config->scale_activation() * linear_module->forward(in_vec);
                torch::Tensor out = activation(scaled);

                outputs[current_unit->ref_node->id] = out;
                i++;
            }
        }
    }

    torch::Tensor output = torch::autograd::Variable(torch::zeros({1, (long)output_units.size()}, torch::TensorOptions().device(device).requires_grad(true)));
    int i = 0;
    for(auto it = output_units.begin() ; it != output_units.end() ; it++) {
        Unit u = *it;
        output[0][i] = outputs[u->ref_node->id];
        i++;
    }
        
    return output;
}

std::vector<Unit> FeedForwardImpl::build_units() {
    std::vector<Unit> units;

    for(auto it = genome->node_genes.begin() ; it != genome->node_genes.end() ; it++) {
        NodeGene n = *it;
        std::vector<ConnectionGene> in_genes = genome->get_connections_in(n->id);
        size_t num_in = in_genes.size();
        
        std::vector<torch::Tensor> weights;
        transform(in_genes.begin(), in_genes.end(), inserter(weights, weights.begin()), [](const ConnectionGene& gene) { return gene->weight; });

        Unit new_unit(new UnitImpl(n, num_in));
        new_unit->set_weights(weights);

        units.push_back(new_unit);
    }

    return units;
}


