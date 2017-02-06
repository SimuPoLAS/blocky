#ifndef BLOCKCALCULATION_HPP
#define BLOCKCALCULATION_HPP

#include <memory>

#include <blockfinding/blockreplacingcalculation.hpp>
#include <block.hpp>
#include <blockynumber.hpp>

struct BlockCalculation
{
    int32_t SavedBits;
    Block VirtualBlock;

    BlockCalculation() { }

    BlockCalculation(int32_t savedBits, Block virtualBlock)
        : SavedBits(savedBits)
        , VirtualBlock(virtualBlock) { }

    bool process_value(shared_ptr<BlockyNumber> value, int32_t index)
    {
        return VirtualBlock.Method->process_value
        (
            VirtualBlock,
            value,
            index,
            SavedBits
        );
    }

    static BlockCalculation from_replacing_calculation
    (
        BlockReplacingCalculation calc
    )
    {
        return BlockCalculation(0, calc.VirtualBlock);
    }
};

#endif /* end of include guard: BLOCKCALCULATION_HPP */
