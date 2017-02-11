#include <math.h>
#include <iostream>

#include <blockycompression.hpp>
#include <methods/patternpingpong/patternpingpongcompression.hpp>

BlockyCompression::BlockyCompression(FILE* file)
    : writer(file) { }

void BlockyCompression::report(BlockyNumber number)
{
    // TODO: this number is coming from Hooker
    // and the hooker just parses a string value
    // and allocates the number on stack, so it will be removed
    // when exiting this block
    // this will probably result in a segmentation fault
    // probably need to make a pointer out of it
    Values.push_back(make_shared<BlockyNumber>(number));
}

void BlockyCompression::report
(
    BlockyNumber* numbers,
    size_t offset,
    size_t count
)
{
    for (size_t i = offset; i < offset + count; i++)
    {
        auto&& num = numbers[i];
        report(num);
    }
}

void BlockyCompression::finish()
{
    if (Values.size() == 0) return;

    Metadata = BlockyMetadata::from_data(Values.data(), Values.size());
    Metadata.MaxNeededBitsNeededBitsNumber = uint8_t
    (
        floor(log2(abs(Metadata.MaxNeededBitsNumber))) + 1
    );

    blockfinding = make_unique<Blockfinding>(Values, Metadata);
    Blocks = blockfinding->find_all_blocks();

    std::cout << "blocks: " << Blocks.size() << std::endl;
    // for(auto value : Values)
    // {
    //     if (value->NeededBitsNumber > 200)
    //         std::cout << "value: " << value->Number << "e" << value->Exponent << '\n';
    // }

    post_compression_optimisation(); //Todo: make optional

    std::cout << "start writing" << '\n';

    write();
}

void BlockyCompression::post_compression_optimisation()
{
    auto currentBlockIndex = 0;
    auto valueCount = Values.size();
    auto blockCount = Blocks.size();
    auto nextStop = blockCount == 0
      ? valueCount : Blocks[currentBlockIndex].Index;
    auto currentBlockLength = blockCount == 0
      ? 0 : Blocks[currentBlockIndex].Length;
    auto totalBlockValues = currentBlockLength;

    if (Blocks.size() == 0)
        return;

    auto ppp = (PatternPingPongCompression*)blockfinding->initialized_method
    (
        Methods::PatternPingPong
    );
    auto recentBlock = Blocks[0];
    uint8_t lastppLength = 0;

    for (size_t i = 1; i < Blocks.size(); i++)
    {
        auto currentBlock = Blocks[i];

        if
        (
            currentBlock.HasPattern
         && currentBlock.Pattern == PatternType::Same
         && recentBlock.HasPattern
        )
        {
            if
            (
                recentBlock.Pattern == PatternType::Pingpong
             && lastppLength * recentBlock.Length + recentBlock.Index
             == currentBlock.Index
            )
            {
                auto initValue = Values[currentBlock.Index];
                auto recentBlockRealLength = lastppLength * recentBlock.Length;
                if
                (
                    currentBlock.Length != lastppLength
                 || initValue->Number
                 == Values
                    [
                        recentBlock.Index
                      + recentBlockRealLength
                      - 1
                    ]->Number
                 || initValue->Number
                 != Values
                    [
                        recentBlock.Index
                      + recentBlockRealLength
                      - 1
                      - lastppLength
                    ]->Number
                )
                    continue;
                recentBlock.Length++;
                Blocks[i - 1] = recentBlock;
                Blocks.erase(Blocks.begin() + i--);
            }
            else if
            (
                recentBlock.Pattern == PatternType::Same
                && recentBlock.Index + recentBlock.Length == currentBlock.Index
                && currentBlock.Length == recentBlock.Length
            )
            {
                recentBlock.Pattern = PatternType::Pingpong;
                recentBlock.Method = ppp;
                ppp->PingPongPatternLengths.push_back
                (
                    PatternPingPongCompression::PatternPingPongMetadata
                    (
                        recentBlock.Length,
                        recentBlock.Index
                    )
                );
                lastppLength = recentBlock.Length;
                recentBlock.Length = 2;
                Blocks[i - 1] = recentBlock;
                Blocks.erase(Blocks.begin() + i--);
            }
            else
                recentBlock = currentBlock;
        }
    }
}

void BlockyCompression::write()
{
    auto currentBlockIndex = 0;

    auto valueCount = Values.size();
    auto blockCount = Blocks.size();
    auto nextStop = blockCount == 0
      ? valueCount : Blocks[currentBlockIndex].Index;

    auto hasExponent = !Metadata.NoExponent;

    // Writing global header
    Metadata.write(writer);
    writer.write_byte(0, 1); // dont use huffman
    std::cout << "meta" << std::endl;

    for (auto i = 0; i < valueCount;)
    {
        for (; nextStop > i; i++) // writing values until the next block is here
        {
            auto value = Values[i];

            //Todo: is the loop below really nessecary with the new changes?
            while (value->NeededBitsNumber > Metadata.MaxNeededBitsNumber) //Todo: check how often this case appears. (This is a "bug" created by the blockfinding, which "corrects" the value to fit with the exp of a block that might get created ...
            {
                value->Number /= 10;
                value->Exponent++;
                value->NeededBitsNumber = int8_t
                (
                    floor(log2(abs(value->Number))) + 1
                );
            }

            writer.write_byte(0, 1);

            if (!Metadata.IsAbsolute)
                writer.write_byte(uint8_t(value->IsNegative ? 1 : 0), 1);
            writer.write(uint64_t(value->Number), Metadata.MaxNeededBitsNumber);
            if (hasExponent)
            {
                writer.write_byte(uint8_t(value->Exponent < 0 ? 1 : 0), 1);
                writer.write
                (
                    uint64_t(abs(value->Exponent)),
                    Metadata.MaxNeededBitsExponent
                );
            }

        }

        if (++currentBlockIndex < blockCount)
        {
        //    Console.WriteLine(Blocks[currentBlockIndex - 1]);
            Blocks[currentBlockIndex - 1].Method->write
            (
                writer,
                Blocks[currentBlockIndex - 1],
                i
            );
            nextStop = Blocks[currentBlockIndex].Index;
        }
        else
        {
            nextStop = valueCount;
        }
    }

    writer.flush(); // This is nessecary. Will write the last buffered byte may only be partially complete!
}
