#ifndef PATTERNPREDICTOR_HPP
#define PATTERNPREDICTOR_HPP

#include <vector>

#include <blockynumber.hpp>

class PatternPredictor
{
private:
    vector<BlockyNumber> const& values;
    in32_t limit;

public:
    int32_t Index;

    PatternPredictor(vector<BlockyNumber> const& values, int32_t index = 0)
        : values(values)
        , limit(values.length() - 2)
        , Index(index) { }

    bool predict_next(BlockyNumber const& current)
    {
        Index++;
        if (Index >= limit)
            return false;

        auto next = values[Index];
        if (next.Number == current.Number && next.Exponent == current.Exponent) //same pattern
            return true;

        var ahead = _values[Index + 1];
        if
        (
            next.Number + (next.Number - current.Number) == ahead.Number
         && next.Exponent + (next.Exponent - current.Exponent) == ahead.Exponent
        ) // offset pattern
            return true;

        return false;
    }
};

#endif /* end of include guard: PATTERNPREDICTOR_HPP */
