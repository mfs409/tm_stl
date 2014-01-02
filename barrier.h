#include <atomic>

/**
 * A standard sense-reversing barrier, based on pseudocode from
 *  http://www.cs.rochester.edu/research/synchronization/pseudocode/ss.html
 */
class barrier
{
    /// Number of threads currently at barrier
    std::atomic<int> count;
    /// Current sense
    std::atomic<bool> sense;
    /// per-thread senses.  Notice that they are not cache-line
    /// padded... performance shall not be great.
    bool* my_sense;
    /// count of total number of threads, for resetting
    int num_threads;

  public:
    /**
     * Construct a barrier by setting the max number of threads, and
     * indicating that nobody has arrived yet
     */
    barrier(int num) : count(num), sense(true), num_threads(num)
    {
        my_sense = new bool[num];
        for (int i = 0; i < num; ++i)
            my_sense[i] = true;
    }

    /**
     *  Arrive at a barrier, and do not return until all threads have
     *  arrived.  Note that each thread needs a unique value for id, in the
     *  range [0,num_threads).
     */
    void arrive(int id)
    {
        my_sense[id] = ! my_sense[id];
        // if I move it to zero...
        if (--count == 0) {
            count = num_threads;
            sense = my_sense[id];
        }
        else {
            while (sense != my_sense[id]);
        }
    }
};
