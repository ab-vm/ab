#ifndef AB_MODULEREADER_HPP_
#define AB_MODULEREADER_HPP_

#include <Ab/ModuleVisitation.hpp>

namespace Ab {

class InstructionListReader final : public InstructionListModel {
public:
	InstructionListReader(const Byte* start, std::size_t size) : start_(start), size_(size) {}

	virtual ~InstructionListReader() override = default;

	virtual void accept(InstructionListVisitor& visitor) override {
		std::size_t offset = 0;
		while (offset < size_) {
			const Opcode opcode = read<Opcode>(offset);
			switch (opcode) {
			case Opcode::UNREACHABLE:
				visitor.on_unreachable();
				offset += 1;
				break;
			case Opcode::NOP:
				visitor.on_nop();
				offset += 1;
				break;
			case Opcode::HALT:
				visitor.on_halt();
				offset += 1;
				break;
			}
		}
	}

private:
	template <typename T>
	T read(std::size_t offset) const {
		return *reinterpret_cast<const T*>(start_ + offset);
	}

	const Byte* start_;
	std::size_t size_;
};

class FuncReader final : public FuncModel {
public:
	FuncReader(Address address) : address_(address){};

	virtual void accept(FuncVisitor& visitor) override { accept_instruction_list(visitor); }

private:
	void accept_instruction_list(FuncVisitor& visitor) {
		// InstructionListReader reader;
		// visitor.on_instruction_list(reader);
	}

	Address address_;
};

class ModuleReader final : public ModuleModel {
public:
	~ModuleReader() = default;

	virtual void accept(ModuleVisitor& visitor) override {
		AB_ASSERT_UNIMPLEMENTED();
#if 0
		for (func : functions) {
			FuncReader func_reader(func);
			visitor.visit_function(func_reader);
		}
#endif
	}
};

} // namespace Ab

#endif // AB_MODULEREADER_HPP_
