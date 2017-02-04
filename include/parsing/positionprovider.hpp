#ifndef POSITIONPROVIDER_HPP
#define POSITIONPROVIDER_HPP

class PositionProvider
{
public:
    uint32_t virtual get_position() = 0;
};

#endif /* end of include guard: POSITIONPROVIDER_HPP */
