#ifndef BLOCKREPLACINGCALCULATION_HPP
#define BLOCKREPLACINGCALCULATION_HPP

#include <block.hpp>

struct BlockReplacingCalculation
{
    int32_t SavedBits;
    Block VirtualBlock;
    Block OldConcurrentBlock;
    SavingGrade OldConcurrentSavingGrade;
    bool IsValid;

    BlockReplacingCalculation() { }

    void Initialize
    (
        int32_t savedBits,
        Block virtualBlock,
        Block oldConcurrentBlock,
        SavingGrade oldConcurrentBlockSavingGrade
    )
    {
        SavedBits = savedBits;
        VirtualBlock = virtualBlock;
        OldConcurrentBlock = oldConcurrentBlock;
        OldConcurrentSavingGrade = oldConcurrentBlockSavingGrade;
        IsValid = true;
    }

    bool process_value(BlockyNumber value, int32_t index)
    {
        return VirtualBlock.Method->process_value
        (
            VirtualBlock,
            value,
            index,
            SavedBits
        );
    }
};

#endif /* end of include guard: BLOCKREPLACINGCALCULATION_HPP */
