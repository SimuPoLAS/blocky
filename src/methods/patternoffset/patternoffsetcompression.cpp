#include <algorithm>
#include <math.h>

#include <methods/patternoffset/patternoffsetcompression.hpp>

using namespace std;

bool PatternOffsetCompression::process_value
(
    Block& block,
    shared_ptr<const BlockyNumber> value,
    int32_t index,
    int32_t& bitDiff
)
{
    if (block.Length == 255)
        return false;

    auto firstValue = values[block.Index];
    auto secondValue = values[block.Index + 1];
    auto lastValue = values[index - 1];

    if
    (
        (
            firstValue->Number - secondValue->Number
         != lastValue->Number - value->Number
        )
        ||
        (
            firstValue->Exponent - secondValue->Exponent
         != lastValue->Exponent - value->Exponent
        )
    )
    {
        if (firstValue->Exponent == 0 && secondValue->Exponent == 0)
        {
            bitDiff += headers.StandardBlockPatternOffset
              - headers.StandardBlockNumbersNoExp;
            block.Method = methods[(int)Methods::NumbersNoExp];
        }
        else if
        (
            firstValue->Exponent == secondValue->Exponent
         && secondValue->Exponent == value->Exponent
        )
        {
            bitDiff += headers.StandardBlockPatternOffset
              - headers.StandardBlockFloatSimmilar;
            block.Method = methods[(int)Methods::FloatSimmilar];
            block.Exponent = firstValue->Exponent;
        }
        else
            return false; //todo: maybe the floatsimmilar algorithm could adjust that ...

        block.HasPattern = false;
        block.BiggestNumber = max
        (
            value->Number,
            max(firstValue->Number, lastValue->Number)
        );

        if
        (
            firstValue->IsNegative == lastValue->IsNegative
         && lastValue->IsNegative == value->IsNegative
        )
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

        block.Length++;
        return true;
    }

    block.Length++;
    bitDiff += headers.StandardSingleValue;
    return true;
}

void PatternOffsetCompression::write
(
    BitWriter& writer,
    Block block,
    int32_t& valueIndex
)
{
    write_default_blockheader(writer, block);

    write_single_value_without_controlbit(writer, values[block.Index]);
    write_single_value_without_controlbit(writer, values[block.Index + 1]);

    valueIndex += block.Length;
}
