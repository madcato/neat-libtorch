#include <map>
#include <string>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdocumentation"
#pragma GCC diagnostic ignored "-Wcomma"
#include <torch/torch.h>
#pragma GCC diagnostic pop

using namespace std;

class Activations {
    Activations();

    auto get(string func_name);

private:
    map<string, std::function<torch::Tensor(const torch::Tensor&)>> functions;
};
