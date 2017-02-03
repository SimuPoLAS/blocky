#ifndef BLOCKFINDING_HPP
#define BLOCKFINDING_HPP

#include <vector>

#include <blockfinding/headersizes.hpp>
#include <blockfinding/blockcalculation.hpp>
#include <blockfinding/blockreplacingcalculation.hpp>
#include <blockfinding/patternpredictor.hpp>
#include <methods/compressionmethod.hpp>
#include <methods/methods.hpp>
#include <blockynumber.hpp>
#include <block.hpp>

class Blockfinding {
private:
    vector<Block> blocks;
    int32_t index;
    BlockCalculation appendingCalculation;
    int32_t appendingCalculationSavingGrade;
    Block lastStableBlock;
    bool isAppendingCalculationValid;
    vector<BlockCalculation> calculations;
    // Length = length of SabingGrade enum
    BlockReplacingCalculation replacingCalculations[SAVINGGRADE_COUNT];
    PatternPredictor patternPredictor;
    bool hasRunningPatternCalculation;
    CompressionMethod* initializedCompressionMethods[METHODS_COUNT];

    void add_new_block(BlockCalculation const& calc);
    void replace_newest_block(BlockCalculation with);
    void transform_calcs_to_replace_cals_or_delete
    (
        Block oldConcurrent,
        int32_t oldConcurrentSavedBits,
        int32_t exclude = -1
    );
    void update_replacing_calculations
    (
        shared_ptr<BlockyNumber> value,
        int32_t bitDiffDiff = 0
    );

public:

    vector<shared_ptr<BlockyNumber>> const& Values;
    int32_t const ValueCount;
    BlockyMetadata const& Metadata;
    HeaderSizes const Headers;

    Blockfinding
    (
        vector<shared_ptr<BlockyNumber>> const& values,
        BlockyMetadata const& metadata
    );

    vector<Block>& find_all_blocks();
    CompressionMethod* initialized_method(Methods const method);
    bool process_next_value();

};

#endif /* end of include guard: BLOCKFINDING_HPP */
