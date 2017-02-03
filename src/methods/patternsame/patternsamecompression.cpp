#include <algorithm>

#include <methods/patternsame/patternsamecompression.hpp>

using namespace std;

bool PatternSameCompression::process_value
(
    Block& block,
    shared_ptr<const BlockyNumber> value,
    int32_t index,
    int32_t& bitDiff
)
{
    if (block.Length == 255)
        return false;

    if (block.Length == 0)
    {
        block.Length++;
        bitDiff += headers.StandardSingleValue;
        return true;
    }

    auto patternDiff = value->Number - values[index - 1]->Number != 0
     || value->Exponent - values[index - 1]->Exponent != 0;

    if (patternDiff)
    {
        if (block.Length == 1)
        {
            block.Length++;
            block.Pattern = PatternType::Offset;
            bitDiff += headers.StandardBlockPatternSame
              - headers.StandardBlockPatternOffset;
            block.Method = methods[(int)Methods::PatternOffset];
            return true;
        }

        auto firstValue = values[block.Index];

        //bitDiff -= _singleValueBits * block.Length - block.Length * MaxNeededBitsExponent * 2;

        block.HasPattern = false;
        block.BiggestNumber = max(firstValue->Number, value->Number);

        if (firstValue->IsNegative == value->IsNegative)
        {
            block.AbsoluteSign = true;
            block.IsSignNegative = value->IsNegative;
            //bitDiff--; commented this out, as the isNegative is now in the default header!
        }
        else
            bitDiff -= block.Length;

        auto nb = uint8_t(floor(log2(abs(block.BiggestNumber))) + 1);
        block.NeededBits = 0; // the nb per value if we still had the pattern
        bitDiff += block.difference_with_nb(metadata, nb);
        block.NeededBits = nb;

        if (firstValue->Exponent == 0 && value->Exponent == 0)
        {
            block.Length++;
            bitDiff += headers.StandardBlockPatternSame
              - headers.StandardBlockNumbersNoExp;
            block.Method = methods[(int)Methods::NumbersNoExp];
            return true;
        }
        auto oldMethod = block.Method;
        block.Method = methods[(int)Methods::FloatSimmilar];
        block.Exponent = firstValue->Exponent;
        block.HasExponent = true;
        bitDiff += headers.StandardBlockPatternSame
          - headers.StandardBlockFloatSimmilar;

        if (firstValue->Exponent == value->Exponent)
        {
            block.Length++;
            return true;
        }

        auto result = block.Method->process_value(block, value, index, bitDiff);
        if (!result)
        {
            block.HasPattern = true;
            block.Method = oldMethod;
        }
        return result;
    }

    block.Length++;

    bitDiff += headers.StandardSingleValue;
    return true;
}

void PatternSameCompression::write
(
    BitWriter writer,
    Block block,
    int32_t& valueIndex
)
{
    write_default_blockheader(writer, block);

    write_single_value_without_controlbit(writer, values[block.Index]);
    valueIndex += block.Length;
}
