/**
 * @file
 * Declaration of a Pseudo Least Recently Used replacement policy.
 */

#ifndef __MEM_CACHE_REPLACEMENT_POLICIES_PLRU_RP_HH__
#define __MEM_CACHE_REPLACEMENT_POLICIES_PLRU_RP_HH__

#include "mem/cache/replacement_policies/base.hh"

struct PLRURPParams;

class PLRURP : public BaseReplacementPolicy
{
  protected:
    /** PLRU-specific implementation of replacement data. */
    struct PLRUReplData : ReplacementData
    {
        /** PLRU tree node bits */
        std::vector<bool> treeBits;

        /**
         * Default constructor. Initialize the tree bits.
         */
        PLRUReplData(int numWays) : treeBits(numWays - 1, false) {}
    };

    /** Number of ways in the cache set */
    const int numWays;

    /**
     * Find the victim recursively.
     *
     * @param idx Index of the current node in the tree.
     * @param level Current level in the tree.
     * @param data Reference to the replacement data.
     * @return Index of the victim way.
     */
    int findVictim(int idx, int level, const std::shared_ptr<PLRUReplData>& data) const;

  public:
    /** Convenience typedef. */
    typedef PLRURPParams Params;

    /**
     * Construct and initialize this replacement policy.
     */
    PLRURP(const Params *p);

    /**
     * Destructor.
     */
    ~PLRURP() {}

    /**
     * Invalidate replacement data to set it as the next probable victim.
     * Resets the PLRU tree.
     *
     * @param replacement_data Replacement data to be invalidated.
     */
    void invalidate(const std::shared_ptr<ReplacementData>& replacement_data)
                                                              const override;

    /**
     * Touch an entry to update its replacement data.
     * Updates the PLRU tree based on the touched entry.
     *
     * @param replacement_data Replacement data to be touched.
     */
    void touch(const std::shared_ptr<ReplacementData>& replacement_data) const
                                                                     override;

    /**
     * Reset replacement data. Used when an entry is inserted.
     * Resets the PLRU tree based on the inserted entry.
     *
     * @param replacement_data Replacement data to be reset.
     */
    void reset(const std::shared_ptr<ReplacementData>& replacement_data) const
                                                                     override;

    /**
     * Find replacement victim using the PLRU tree.
     *
     * @param candidates Replacement candidates, selected by indexing policy.
     * @return Replacement entry to be replaced.
     */
    ReplaceableEntry* getVictim(const ReplacementCandidates& candidates) const
                                                                     override;

    /**
     * Instantiate a replacement data entry.
     *
     * @return A shared pointer to the new replacement data.
     */
    std::shared_ptr<ReplacementData> instantiateEntry() override;
};

#endif // __MEM_CACHE_REPLACEMENT_POLICIES_PLRU_RP_HH__
