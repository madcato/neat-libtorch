#include "neat/activations.hpp"

Activations::Activations() {
    auto sigmoid = [](const torch::Tensor& x) { return torch::sigmoid(x); };
    auto tanh = [](const torch::Tensor& x) { return torch::tanh(x); };

    std::map<std::string, std::function<torch::Tensor(const torch::Tensor &)>> function_list;
    function_list.emplace("sigmoid", sigmoid);
    function_list.emplace("tanh", tanh);

    functions = function_list;
}

std::function<torch::Tensor(const torch::Tensor &)> Activations::get(std::string func_name) {
    return functions[func_name];
}
