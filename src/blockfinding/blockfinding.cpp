#include <vector>

#include <blockfinding/blockfinding.hpp>

#include <methods/floatsimmilar/floatsimmilarcompression.hpp>
#include <methods/numbersnoexp/numbersnoexpcompression.hpp>
#include <methods/patternoffset/patternoffsetcompression.hpp>
#include <methods/patternpingpong/patternpingpongcompression.hpp>
#include <methods/patternsame/patternsamecompression.hpp>

Blockfinding::Blockfinding
(
    vector<shared_ptr<BlockyNumber>> const& values,
    BlockyMetadata const& metadata
)
    : Values(values)
    , ValueCount(values.size())
    , Metadata(metadata)
    , Headers(metadata)
    , patternPredictor(values)
{
    initializedCompressionMethods[(int)Methods::PatternSame] =
        new PatternSameCompression
        (
            values,
            metadata,
            Headers,
            initializedCompressionMethods
        );
    initializedCompressionMethods[(int)Methods::FloatSimmilar] =
        new FloatSimmilarCompression
        (
            values,
            metadata,
            Headers,
            initializedCompressionMethods
        );
    initializedCompressionMethods[(int)Methods::NumbersNoExp] =
        new NummbersNoExpCompression
        (
            values,
            metadata,
            Headers,
            initializedCompressionMethods
        );
    initializedCompressionMethods[(int)Methods::PatternOffset] =
        new PatternOffsetCompression
        (
            values,
            metadata,
            Headers,
            initializedCompressionMethods
        );
    initializedCompressionMethods[(int)Methods::PatternPingPong] =
        new PatternPingPongCompression
        (
            values,
            metadata,
            Headers,
            initializedCompressionMethods
        );
}

vector<Block>& Blockfinding::find_all_blocks()
{
    while(process_next_value()) { }
    return blocks;
}

CompressionMethod* Blockfinding::initialized_method(Methods const method)
{
    return initializedCompressionMethods[(int)method];
}

void Blockfinding::add_new_block(BlockCalculation const& calc)
{
    lastStableBlock = calc.VirtualBlock;
    appendingCalculation = calc;
    blocks.push_back(calc.VirtualBlock);
    isAppendingCalculationValid = true;
    appendingCalculationSavingGrade = (int)calc.VirtualBlock.savinggrade();

    for (size_t i = 1; i < SAVINGGRADE_COUNT; i++)
        replacingCalculations[i].IsValid = false;
}

void Blockfinding::replace_newest_block(BlockCalculation with)
{
    blocks[blocks.size() - 1] = with.VirtualBlock;
    lastStableBlock = with.VirtualBlock;
    appendingCalculation = with;
    appendingCalculation.SavedBits = 0;
    appendingCalculationSavingGrade = (int)with.VirtualBlock.savinggrade();

    for (size_t i = 1; i < SAVINGGRADE_COUNT; i++)
    {
        replacingCalculations[i].IsValid = false;
    }
}

void Blockfinding::transform_calcs_to_replace_cals_or_delete
(
    Block oldConcurrent,
    int32_t oldConcurrentSavedBits,
    int32_t exclude
)
{
    //Bug: we shouldn't mindlessly remove all calculations, because if there is a calc will less nb that would have been created soon, it might be better than this one in the long run
    for (size_t k = 0; k < calculations.size(); k++)
    {
        if (exclude == k)
            continue;
        auto savingGrade = (int)calculations[k].VirtualBlock.savinggrade();
        if
        (
            savingGrade > appendingCalculationSavingGrade
         && (
                !replacingCalculations[savingGrade].IsValid
                ||
                (
                    replacingCalculations[savingGrade].SavedBits
                  < calculations[k].SavedBits
                )
            )
        )
        {
            replacingCalculations[savingGrade].initialize
            (
                calculations[k].SavedBits - oldConcurrentSavedBits,
                calculations[k].VirtualBlock, oldConcurrent,
                (SavingGrade)appendingCalculationSavingGrade
            );
        }

        if (calculations[k].VirtualBlock.HasPattern)
        {
            hasRunningPatternCalculation = false;
        }
        // _calculations.RemoveAt(k--);
    }
    calculations.clear();
}

void Blockfinding::update_replacing_calculations
(
    shared_ptr<BlockyNumber> value,
    int32_t bitDiffDiff
)
{
    for
    (
        size_t j = appendingCalculationSavingGrade + 1;
        j < SAVINGGRADE_COUNT;
        j++
    )
    {
        if (!replacingCalculations[j].IsValid)
            continue;

        auto replacingCalculation = replacingCalculations[j];
        auto savingGrade = replacingCalculation.VirtualBlock.savinggrade();
        if (!replacingCalculation.process_value(value, index))
        {
            replacingCalculations[j].IsValid = false;
            continue;
        }

        if ((replacingCalculation.SavedBits += bitDiffDiff) > 0)
        {
            replacingCalculations[j].IsValid = false;

            calculations.clear(); //Todo: not jsut clear, but make a replacingCalc on this replacing calc ...
            hasRunningPatternCalculation = false;

            if (!replacingCalculation.OldConcurrentBlock.IsValid) //Todo: check what this was
            {
                replace_newest_block
                (
                    BlockCalculation::from_replacing_calculation
                    (
                        replacingCalculation
                    )
                );
            }
            else
            {
                blocks[blocks.size() - 1] =
                    replacingCalculation.OldConcurrentBlock; // set last block to old, non-overlapping state
                add_new_block
                (
                    BlockCalculation::from_replacing_calculation
                    (
                        replacingCalculation
                    )
                );
            }

            break;
        }

        auto newSavingGrade =
            replacingCalculation.VirtualBlock.savinggrade();

        if (savingGrade != newSavingGrade)
        {
            replacingCalculations[(int)savingGrade].IsValid = false;
            auto overriddenCalc = replacingCalculations[(int)newSavingGrade];
            if
            (
                overriddenCalc.IsValid
              && overriddenCalc.SavedBits > replacingCalculation.SavedBits
            )
                continue;
        }

        if (newSavingGrade <= replacingCalculation.OldConcurrentSavingGrade)
            replacingCalculation.IsValid = false;

        // replacingCalculations[j].IsValid = false;
        replacingCalculations[(int)newSavingGrade] = replacingCalculation;
    }
}

bool Blockfinding::process_next_value()
{
    auto value = Values[index];
    auto patternPred = patternPredictor.predict_next(value);

    // adding value to last block and updating replacing calculations

    if (isAppendingCalculationValid)
    {

        auto lastBitDiff = appendingCalculation.SavedBits;
        auto lastAppendingBlock = appendingCalculation.VirtualBlock;

        if (appendingCalculation.process_value(value, index))
        {
            auto bifDiffDiff = lastBitDiff - appendingCalculation.SavedBits;
            if (appendingCalculation.SavedBits > 0)
            {
                if
                (
                    appendingCalculation.VirtualBlock.Length
                  - lastStableBlock.Length > 1
                ) // we made a jump, maybe there were some calcs started in the meantime?
                {
                    transform_calcs_to_replace_cals_or_delete
                    (
                        lastAppendingBlock,
                        appendingCalculation.SavedBits
                    );
                }

                replace_newest_block(appendingCalculation);
            }

            appendingCalculationSavingGrade =
                (int)appendingCalculation.VirtualBlock.savinggrade();

            update_replacing_calculations(value, bifDiffDiff);

            if
            (
                patternPred
             && appendingCalculationSavingGrade < (int)SavingGrade::Pattern
             && !replacingCalculations[(int)SavingGrade::Pattern].IsValid
            )
            {
                Block block
                (
                    index,
                    value->Exponent,
                    value->NeededBitsNumber,
                    value->IsNegative,
                    value->Number,
                    initializedCompressionMethods,
                    Methods::PatternSame,
                    true
                );
                auto calc = replacingCalculations[(int)SavingGrade::Pattern];
                calc.initialize
                (
                    -Headers.StandardBlockPatternSame,
                    block,
                    lastAppendingBlock,
                    (SavingGrade)appendingCalculationSavingGrade
                );
                if (calc.process_value(value, index))
                    replacingCalculations[(int)SavingGrade::Pattern] = calc;
            }
        }
        else
        {
            update_replacing_calculations(value);
            isAppendingCalculationValid = false;
        }
    }
    else
    {
        update_replacing_calculations(value);
    }

    // updating current calculations if nessecary

    auto isLastBlockUp2Date =
        lastStableBlock.Index + lastStableBlock.Length - 1 == index;

    for (size_t j = 0; j < calculations.size(); j++)
    {
        auto calc = calculations[j];

        // TODO: we shouldn't only remove calculations that are super far away, we should also remove calculations that have a very negative calc.SavedBits

        auto hasPattern = calc.VirtualBlock.HasPattern;
        if (!calc.process_value(value, index))
        {
            if (hasPattern)
                hasRunningPatternCalculation = false;
            calculations.erase(calculations.begin() + j--); // There was some error / the block is impossible, e.g. the exps are different and you can't even correct the values because of overflows ...
            continue;
        }

        if (hasPattern && !calc.VirtualBlock.HasPattern)
            //throw new Exception("c#er is wrong");
            hasRunningPatternCalculation = false;

        if (calc.SavedBits > 0)
        {
            if (calc.VirtualBlock.HasPattern)
                hasRunningPatternCalculation = false;

            if
            (
                lastStableBlock.Index + lastStableBlock.Length - 1
             >= calc.VirtualBlock.Index
            )
            {
                calculations.erase(calculations.begin() + j--);
                continue;
                // throw new InvalidOperationException("Blocks sharing values! (ノ° ͜ʖ ͡°)ノ︵┻┻ ");
            }

            add_new_block(calc);

            isLastBlockUp2Date = true;

            for (size_t k = j + 1; k < calculations.size(); k++) // we need to update the rest here or they jump one update tick if they get transformed to ReplaceCalcs ...
            {
                auto calc2 = calculations[k];
                if (!calc2.process_value(value, index))
                {
                    calculations.erase(calculations.begin() + k--);
                    continue;
                }
                calculations[k] = calc2;
            }

            transform_calcs_to_replace_cals_or_delete
            (
                Block(),
                calc.SavedBits,
                j
            ); // we give an invalid block because the just created block and the already running calc could never co-exist. The just created block will get deleted if a replacing calc with an invalid oldConcurrentBLock gets founded

            break; // Bug: actually, breaking here is pretty bad. There could be multiple calculations that want to create a block, and maybe this is not the best one. Saves some performance though
        }

        calculations[j] = calc;
    }

    // starting new calculations if nessecary

    if (!hasRunningPatternCalculation && !isLastBlockUp2Date)
    {
        if (patternPred)
        {
            BlockCalculation calc
            (
                -Headers.StandardBlockPatternSame,
                Block
                (
                    index,
                    value->Exponent,
                    0,
                    false,
                    value->Number,
                    initializedCompressionMethods,
                    Methods::PatternSame,
                    true
                )
            );
            if (calc.process_value(value, index))
            {
                calculations.push_back(calc);
                hasRunningPatternCalculation = true;
            }
        }
        else
        {
            auto hasExp = false;
            for (auto runningCalculation : calculations) //Todo: pre calculate that (bool array?)
            {
                if (runningCalculation.VirtualBlock.Exponent == value->Exponent)
                {
                    hasExp = true;
                    break;
                }
            }
            if (!hasExp)
            {
                BlockCalculation calc;

                // ReSharper disable once ConvertIfStatementToConditionalTernaryExpression - You can't see **** if you do a ?:
                if (value->Exponent == 0)
                    calc = BlockCalculation
                    (
                        -Headers.StandardBlockNumbersNoExp,
                        Block
                        (
                            index,
                            0,
                            value->NeededBitsNumber,
                            value->IsNegative,
                            value->Number,
                            initializedCompressionMethods,
                            Methods::NumbersNoExp,
                            false
                        )
                    );
                else
                    calc = BlockCalculation
                    (
                        -Headers.StandardBlockFloatSimmilar,
                        Block
                        (
                            index,
                            value->Exponent,
                            value->NeededBitsNumber,
                            value->IsNegative,
                            value->Number,
                            initializedCompressionMethods,
                            Methods::FloatSimmilar,
                            false
                        )
                    );

                if (calc.process_value(value, index))
                    calculations.push_back(calc);
                else
                {
                    //TODO: throw meaningful exception, not just zero
                    throw 0;
                }
            }
        }
    }

    return ++index < ValueCount;
}
