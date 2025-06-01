#pragma once
#include "PointSource.hpp"
#include <string>

class JsonReader : public PointSource
{
public:
    explicit JsonReader(const std::string &filepath);
    std::vector<Point> getPoints() override;

private:
    std::string filepath;
};
