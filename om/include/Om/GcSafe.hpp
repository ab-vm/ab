#ifndef OM_GCSAFE_HPP_
#define OM_GCSAFE_HPP_

namespace Om {

/// GC Safety indicator.
/// `yes` as a parameter indicates that an operation may GC.
/// `no` indicates that it is not safe to GC, operations can not GC.
/// GC unsafe allocations are more likely to fail due to OOM.
enum class GcSafe { yes = 1, no = 0 };

}  // namespace Om

#endif  // OM_GCSAFE_HPP_
