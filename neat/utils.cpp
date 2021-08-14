#include "neat/utils.hpp"

#include <torch/torch.h>

float rand_uni_val() {
    return torch::rand({1})[0].item<float>();
}

bool rand_bool() {
    return rand_uni_val() <= 0.5;
}