#ifndef ACTIVATIONS_HPP
#define ACTIVATIONS_HPP

#include <map>
#include <string>
#include <torch/torch.h>

using namespace std;

class Activations {
    Activations();

    auto get(string func_name);

private:
    map<string, std::function<torch::Tensor(const torch::Tensor&)>> functions;
};

#endif  // ACTIVATIONS_HPP
