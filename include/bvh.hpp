#ifndef BVH_HPP
#define BVH_HPP

#include <memory>
#include <vector>

#include "bvh_node.hpp"

class BVH {
private:
    std::unique_ptr<BVHNode> root_;

public:
    BVH(std::vector<Triangle> triangles)
        : root_(std::make_unique<BVHNode>(triangles)) {}
};

#endif // BVH_HPP
