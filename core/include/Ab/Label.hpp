#ifndef AB_LABEL_HPP_
#define AB_LABEL_HPP_

#include <cstddef>
#include <cstdint>
#include <vector>
#include <Ab/Assert.hpp>

namespace Ab {

using LabelId = std::size_t;

constexpr auto INVALID_LABEL_ID = SIZE_MAX;

/// Lightweight handle to a label record.
///
struct Label {
	LabelId id;
};

constexpr Label INVALID_LABEL = Label{INVALID_LABEL_ID};

/// Data associated with a label.
///
struct LabelRecord {
	unsigned offset  = 0; //< offset into the function.
	LabelId id       = INVALID_LABEL_ID;
	bool placed      = false;
};

class LabelTable {
public:
	/// allocate an unplaced label.
	///
	Label alloc() {
		LabelRecord record;
		record.id = next();
		records_.push_back(record);
		return Label{record.id};
	}

	/// allocate a label and place it immediately.
	///
	Label alloc(unsigned offset) {
		auto label = alloc();
		place(label, offset);
		return label;
	}

	/// Place a label. Associate an offset with a label.
	///
	void place(Label label, std::int32_t offset) {
		AB_ASSERT(label.id < next());
		auto& record = records_[label.id];
		AB_ASSERT(!record.placed);
		AB_ASSERT(record.id == label.id);
		record.offset = offset;
		record.placed = true;
	}

	/// Get the offset a label refers to. The label must have been placed.
	///
	unsigned target_of(Label label) {
		AB_ASSERT(label.id < next());
		const auto& record = records_[label.id];
		AB_ASSERT(record.id == label.id);
		AB_ASSERT(record.placed);
		return record.offset;
	}

private:
	/// get the next unallocated label id.
	LabelId next() const { return records_.size(); }

	std::vector<LabelRecord> records_;
};

} // namespace Ab

#endif // AB_LABEL_HPP_
