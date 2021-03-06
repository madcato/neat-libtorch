#ifndef ACTIVATIONS_HPP
#define ACTIVATIONS_HPP

#include <map>
#include <string>
#include <torch/torch.h>

class Activations {
public:
    Activations();

    std::function<torch::Tensor(const torch::Tensor&)> get(std::string func_name);

private:
    std::map<std::string, std::function<torch::Tensor(const torch::Tensor&)>> functions;
};

#endif  // ACTIVATIONS_HPP
