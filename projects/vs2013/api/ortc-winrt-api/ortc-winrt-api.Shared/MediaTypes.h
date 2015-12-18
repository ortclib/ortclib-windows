#pragma once
#include <collection.h>

using Windows::Foundation::Collections::IVector;

namespace ortc_winrt_api
{
	public ref struct LongRange sealed
	{
		property uint32 max;
		property uint32 min;
	};

	public ref struct ConstrainLongRange sealed //: LongRange
	{
		property uint32 max;
		property uint32 min;

		property uint32 exact;
		property uint32 ideal;
	};

	public ref struct ConstrainLong sealed //: ConstrainLongRange
	{
		property uint32 max;
		property uint32 min;

		property uint32 exact;
		property uint32 ideal;

		property uint32 value;
	};

	public ref struct DoubleRange sealed
	{
		property double  max;
		property double  min;
	};

	public ref struct ConstrainDoubleRange sealed //: DoubleRange
	{
		property double  max;
		property double  min;

		property double  exact;
		property double  ideal;
	};

	public ref struct ConstrainDouble sealed //: ConstrainDoubleRange
	{
		property double  max;
		property double  min;

		property double  exact;
		property double  ideal;

		property double  value;
	};

	public ref struct StringOrStringList sealed
	{
		property Platform::String^				value;
		property IVector<Platform::String^>^		values;
	};
	public ref struct ConstrainStringParameters sealed
	{
		property StringOrStringList^ exact;
		property StringOrStringList^ ideal;
	};

	public ref struct ConstrainString sealed
	{
		property StringOrStringList^			value;
		property ConstrainStringParameters^	parameters;
	};

	public ref struct ConstrainBoolParameters sealed
	{
		property bool exact;
		property bool ideal;
	};

	public ref struct ConstrainBool sealed
	{
		property bool			value;
		property ConstrainBoolParameters^	parameters;
	};
}