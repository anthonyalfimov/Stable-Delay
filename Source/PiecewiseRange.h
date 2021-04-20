/*
  ==============================================================================

    PiecewiseRange.h
    Created: 20 Apr 2021 4:31:28pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

template <typename ValueType, std::size_t Size>
class PiecewiseRange
{
public:
    using RangeType = NormalisableRange<ValueType>;
    using RangeLimitPair = std::pair<RangeType, ValueType>;

    /** Creates an object that can generate a NormalisableRange pieced together
        from multiple segments. Each segment is represented by its own
        NormalisableRange object.

        @Discussion
        NB! An object of this class contains the data necessary for the
        NormalisableRange object it generates to function. Therefore, the
        lifespan of this object should be no less than of the generated
        NormalisableRange objects.

        @param segmentList A list of std::pairs: NormalisableRange of the segment and the top limit of the segment's proportion range. The last pair's limit must be 1.
    */
    PiecewiseRange (std::initializer_list<RangeLimitPair> segmentList)
    {
        jassert (Size == segmentList.size());

        ValueType previousLimit = 0;
        ValueType previousRangeEnd;
        int index = 0;

        for (auto [range, limit] : segmentList)
        {
            jassert (isPositiveAndNotGreaterThan (limit, 1));
            jassert (limit > previousLimit);

            // Initialise segment
            mSegments[index] = range;
            mSegments[index].proportionStart = previousLimit;
            mSegments[index].proportionEnd = limit;

            // Ensure continuity of segments
            if (index != 0)
            {
                jassert (previousRangeEnd == mSegments[index].start);
                mSegments[index].start = previousRangeEnd;
            }

            // Update
            previousLimit = limit;
            previousRangeEnd = mSegments[index].end;
            ++index;

            if (index >= Size)
                break;
        }

        jassert (previousLimit == 1);

        start = mSegments[0].start;
        end = previousRangeEnd;
    }

    //==========================================================================
    RangeType getNormalisableRange() const
    {
        return RangeType (start, end,
                          convertFrom0To1Func, convertTo0To1Func,
                          snapToLegalValueFunc);
    }

    //==========================================================================
    using ValueRemapFunction = std::function<ValueType(ValueType rangeStart,
                                                       ValueType rangeEnd,
                                                       ValueType valueToRemap)>;

    ValueRemapFunction convertFrom0To1Func = [this] (ValueType /*rangeStart*/,
                                                     ValueType /*rangeEnd*/,
                                                     ValueType proportionToRemap)
    {
        for (auto& segment : mSegments)
        {
            if (proportionToRemap <= segment.proportionEnd)
                return segment.convertFrom0to1Segment (proportionToRemap);
        }

        jassertfalse;
        return start;
    };

    ValueRemapFunction convertTo0To1Func = [this] (ValueType /*rangeStart*/,
                                                   ValueType /*rangeEnd*/,
                                                   ValueType valueToRemap)
    {
        for (auto& segment : mSegments)
        {
            if (valueToRemap <= segment.end)
                return segment.convertTo0to1Segment (valueToRemap);
        }

        jassertfalse;
        return static_cast<ValueType> (0);
    };

    ValueRemapFunction snapToLegalValueFunc = [this] (ValueType /*rangeStart*/,
                                                      ValueType /*rangeEnd*/,
                                                      ValueType valueToSnap)
    {
        for (auto& segment : mSegments)
        {
            if (valueToSnap <= segment.end)
                return segment.snapToLegalValue (valueToSnap);
        }

        jassertfalse;
        return start;
    };

private:
    ValueType start = 0;
    ValueType end = 1;

    struct RangeSegment  : public RangeType
    {
        /** Creates a range segment that spans the whole 0 to 1 proportion
            range and uses the default NormalisableRange object.
        */
        RangeSegment() = default;

        RangeSegment (const RangeType& range) : RangeType (range) {}
        RangeSegment (RangeType&& range) : RangeType (range) {}

        //======================================================================
        RangeSegment (const RangeSegment&) = default;
        RangeSegment& operator= (const RangeSegment&) = default;
        RangeSegment (RangeSegment&&) = default;
        RangeSegment& operator= (RangeSegment&&) = default;

        //======================================================================
        ValueType convertTo0to1Segment (ValueType value) const noexcept
        {
            auto length = proportionEnd - proportionStart;

            return proportionStart + length * RangeType::convertTo0to1 (value);
        }

        ValueType convertFrom0to1Segment (ValueType proportion) const noexcept
        {
            jassert (proportion >= proportionStart && proportion <= proportionEnd);

            auto length = proportionEnd - proportionStart;
            proportion = (proportion - proportionStart) / length;

            return RangeType::convertFrom0to1 (proportion);
        }

        ValueType proportionStart = 0;
        ValueType proportionEnd = 1;
    };

    std::array<RangeSegment, Size> mSegments;
};
