#include "mem/cache/replacement_policies/plru_rp.hh"

#include <cassert>
#include <memory>

#include "params/PLRURP.hh"

PLRURP::PLRURP(const Params *p)
    : BaseReplacementPolicy(p), numWays(p->num_ways)
{
    assert(numWays > 1 && "PLRU requires at least two ways.");
}

void
PLRURP::invalidate(const std::shared_ptr<ReplacementData>& replacement_data)
const
{
    std::shared_ptr<PLRUReplData> casted_replacement_data =
        std::static_pointer_cast<PLRUReplData>(replacement_data);

    // Reset all tree bits to false
    std::fill(casted_replacement_data->treeBits.begin(), 
              casted_replacement_data->treeBits.end(), false);
}

void
PLRURP::touch(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    // Pseudo LRU does not update on touch in this simple implementation
}

void
PLRURP::reset(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    // Pseudo LRU does not reset individual entries
}

int
PLRURP::findVictim(int idx, int level, const std::shared_ptr<PLRUReplData>& data) const
{
    if (level == numWays - 1) {
        return idx;
    }

    bool bit = data->treeBits[idx];
    data->treeBits[idx] = !bit;

    return findVictim(2 * idx + 1 + bit, level + 1, data);
}

ReplaceableEntry*
PLRURP::getVictim(const ReplacementCandidates& candidates) const
{
    assert(candidates.size() > 0);

    std::shared_ptr<PLRUReplData> data = std::static_pointer_cast<PLRUReplData>(
        candidates[0]->replacementData);

    int victimIdx = findVictim(0, 0, data);

    return candidates[victimIdx % candidates.size()];
}

std::shared_ptr<ReplacementData>
PLRURP::instantiateEntry()
{
    return std::shared_ptr<ReplacementData>(new PLRUReplData(numWays));
}

PLRURP*
PLRURPParams::create()
{
    return new PLRURP(this);
}
