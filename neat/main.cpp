#include <iostream>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdocumentation"
#pragma GCC diagnostic ignored "-Wcomma"
#include <torch/torch.h>
#pragma GCC diagnostic pop

int main()
{
    torch::Tensor tensor = torch::rand({2, 3});
    std::cout << tensor << std::endl;
    return 0;
}