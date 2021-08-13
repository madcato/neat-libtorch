#include <map>
#include <string>
#include <torch/torch.h>

#include "activations.hpp"

Activations::Activations() {
    auto sigmoid = [](const torch::Tensor& x) { return torch::sigmoid(x); };
    auto tanh = [](const torch::Tensor& x) { return torch::tanh(x); };

    map<string, std::function<torch::Tensor(const torch::Tensor &)>> function_list;
    function_list.emplace("sigmoid", sigmoid);
    function_list.emplace("tanh", tanh);

    functions = function_list;
}

auto Activations::get(string func_name) {
    return functions[func_name];
}
